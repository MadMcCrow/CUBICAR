#include "CUBICARPhysicsReplication.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"

#if 0

FCUBICARPhysicsReplication::FCUBICARPhysicsReplication(FPhysScene* InPhysicsScene) : FPhysicsReplication(InPhysicsScene), PhysScene(InPhysicsScene)
 {
 }
 
 void FCUBICARPhysicsReplication::OnTick(float DeltaSeconds, TMap<TWeakObjectPtr<UPrimitiveComponent>, FReplicatedPhysicsTarget>& ComponentsToTargets)
 {
     FPhysicsReplication::OnTick( DeltaSeconds, ComponentsToTargets );
 
     const FRigidBodyErrorCorrection& PhysicErrorCorrection = UPhysicsSettings::Get()->PhysicErrorCorrection;
     for (auto Itr = ComponentsToTargets.CreateIterator(); Itr; ++Itr)
     {
         bool bRemoveItr = false;
         if (UPrimitiveComponent* PrimComp = Itr.Key().Get())
         {
             if(FBodyInstance* BI = PrimComp->GetBodyInstance(Itr.Value().BoneName))
             {
                 FReplicatedPhysicsTarget& PhysicsTarget = Itr.Value();
                 FRigidBodyState& UpdatedState = PhysicsTarget.TargetState;
                 AActor* OwningActor = PrimComp->GetOwner();
                 if( OwningActor && OwningActor->Role == ROLE_AutonomousProxy) //ROLE_AutonomousProxy)
                 {
					 // Get the ping of the guy who owns this thing. If nobody is,
					// then it's server authoritative.
					 const float OwnerPing = GetAuthoritativeSimPing(OwningActor, PhysicsTarget);
					 const float LocalPing = GetLocalSimPing();
					 // Get the total ping - this approximates the time since the update was
					 // actually generated on the machine that is doing the authoritative sim.
					 // NOTE: We divide by 2 to approximate 1-way ping from 2-way ping.
					 const float PingSecondsOneWay = (LocalPing + OwnerPing) * 0.5f * 0.001f;

					 // force update if simulation is sleeping on server
					 if ((UpdatedState.Flags & ERigidBodyFlags::Sleeping) && BI->IsInstanceAwake())
					 {
					 	UpdatedState.Flags |= ERigidBodyFlags::NeedsUpdate;
					 }

                     if (UpdatedState.Flags & ERigidBodyFlags::NeedsUpdate)
                     {
                         const bool bRestoredState = TryApplyRigidBodyState( DeltaSeconds, BI, PhysicsTarget, PhysicErrorCorrection, PingSecondsOneWay );
                         if (bRestoredState)
                             bRemoveItr = true;
                         PrimComp->SyncComponentToRBPhysics();
                     }
                 }
             }
         }
 
         if (bRemoveItr)
             Itr.RemoveCurrent();
     }
 }

float FCUBICARPhysicsReplication::GetAuthoritativeSimPing(AActor* const Owner, const FReplicatedPhysicsTarget& Target) const
{
	if (UPlayer* OwningPlayer = Owner->GetNetOwningPlayer())
	{
		if (UWorld* World = PhysScene->GetOwningWorld())
		{
			if (APlayerController* PlayerController = OwningPlayer->GetPlayerController(World))
			{
				if (APlayerState* PlayerState = PlayerController->PlayerState)
				{
					return PlayerState->ExactPing;
				}
			}
		}
	}
	return 0.f;
}

float FCUBICARPhysicsReplication::GetLocalSimPing() const
{
	if (UWorld* World = PhysScene->GetOwningWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (APlayerState* PlayerState = PlayerController->PlayerState)
			{
				return PlayerState->ExactPing;
			}
		}
	}
	return 0.0f;
}

bool FCUBICARPhysicsReplication::TryApplyRigidBodyState(float DeltaSeconds, FBodyInstance* BI,
	FReplicatedPhysicsTarget& PhysicsTarget, const FRigidBodyErrorCorrection& ErrorCorrection,
	const float PingSecondsOneWay)
{
	const FRigidBodyState NewState = PhysicsTarget.TargetState;
	// Get Current state
	FRigidBodyState CurrentState;
	BI->GetRigidBodyState(CurrentState);
	const FQuat InvCurrentQuat = CurrentState.Quaternion.Inverse();

	/////// EXTRAPOLATE APPROXIMATE TARGET VALUES ///////

	// Starting from the last known authoritative position, and
	// extrapolate an approximation using the last known velocity
	// and ping.
	const float ExtrapolationDeltaSeconds = PingSecondsOneWay * 1;
	const FVector ExtrapolationDeltaPos = NewState.LinVel * ExtrapolationDeltaSeconds;
	const FVector_NetQuantize100 TargetPos = NewState.Position + ExtrapolationDeltaPos;
	float NewStateAngVel;
	FVector NewStateAngVelAxis;
	NewState.AngVel.FVector::ToDirectionAndLength(NewStateAngVelAxis, NewStateAngVel);
	NewStateAngVel = FMath::DegreesToRadians(NewStateAngVel);
	const FQuat ExtrapolationDeltaQuaternion = FQuat(NewStateAngVelAxis, NewStateAngVel * ExtrapolationDeltaSeconds);
	const FQuat TargetAng = ExtrapolationDeltaQuaternion * NewState.Quaternion;

	/////// COMPUTE DIFFERENCES ///////

	const FVector LinDiff = TargetPos - CurrentState.Position;
	FVector AngDiffAxis;
	float AngDiff;
	const FQuat DeltaQuat = TargetAng * InvCurrentQuat;
	DeltaQuat.ToAxisAndAngle(AngDiffAxis, AngDiff);
	AngDiff = FMath::RadiansToDegrees(FMath::UnwindRadians(AngDiff));
	/////// ACCUMULATE ERROR IF NOT APPROACHING SOLUTION ///////


		//
		// The heuristic for error accumulation here is:
		// 1. Did the physics tick from the previous step fail to
		//    move the body towards a resolved position?
		// 2. Was the linear error in the same direction as the
		//    previous frame?
		// 3. Is the linear error large enough to accumulate error?
		//
		// If these conditions are met, then "error" time will accumulate.
		// Once error has accumulated for a certain number of seconds,
		// a hard-snap to the target will be performed.
		//
		// TODO: Rotation while moving linearly can still mess up this
		// heuristic. We need to account for it.
		//

		// Project the change in position from the previous tick onto the
		// linear error from the previous tick. This value roughly represents
		// how much correction was performed over the previous physics tick.
		const float PrevProgress = FVector::DotProduct(
			FVector(CurrentState.Position) - PhysicsTarget.PrevPos,
			(PhysicsTarget.PrevPosTarget - PhysicsTarget.PrevPos).GetSafeNormal());

		// Project the current linear error onto the linear error from the
		// previous tick. This value roughly represents how little the direction
		// of the linear error state has changed, and how big the error is.
		const float PrevSimilarity = FVector::DotProduct(
			TargetPos - FVector(CurrentState.Position),
			PhysicsTarget.PrevPosTarget - PhysicsTarget.PrevPos);


	UE_LOG(LogTemp, Warning, TEXT("Error is : Similarity %f, CorrectionProgress %f"), PrevSimilarity, PrevProgress);
	return  ApplyRigidBodyState(DeltaSeconds, BI, PhysicsTarget, ErrorCorrection, PingSecondsOneWay);
}


FPhysicsReplication* ICUBICARPhysicsReplicationFactory::Create(FPhysScene* OwningPhysScene)
 {
     return new FCUBICARPhysicsReplication( OwningPhysScene );
 }
 
 void ICUBICARPhysicsReplicationFactory::Destroy(FPhysicsReplication* PhysicsReplication)
 {
     delete PhysicsReplication;
 }
 
#endif //0