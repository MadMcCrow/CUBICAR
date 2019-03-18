#include "CUBICARPhysicsReplication.h"
#include "PhysicsEngine/PhysicsSettings.h"


FCUBICARPhysicsReplication::FCUBICARPhysicsReplication(FPhysScene* PhysScene) : FPhysicsReplication(PhysScene)
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
                 if( OwningActor && OwningActor->Role == ROLE_AutonomousProxy )
                 {
                     if (UpdatedState.Flags & ERigidBodyFlags::NeedsUpdate)
                     {
                         const float PingSecondsOneWay = 0;
                         const bool bRestoredState = ApplyRigidBodyState( DeltaSeconds, BI, PhysicsTarget, PhysicErrorCorrection, PingSecondsOneWay );
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
 
 
 FPhysicsReplication* ICUBICARPhysicsReplicationFactory::Create(FPhysScene* OwningPhysScene)
 {
     return new FCUBICARPhysicsReplication( OwningPhysScene );
 }
 
 void ICUBICARPhysicsReplicationFactory::Destroy(FPhysicsReplication* PhysicsReplication)
 {
     delete PhysicsReplication;
 }
 