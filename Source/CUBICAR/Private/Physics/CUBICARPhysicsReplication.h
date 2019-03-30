#pragma once
 
 #include "CoreMinimal.h"
 #include "PhysicsPublic.h"
 #include "PhysicsReplication.h"

// 4.20 version
// this does not work in 4.21

 /*
 class FCUBICARPhysicsReplication : public FPhysicsReplication
 {
 public:
	 FCUBICARPhysicsReplication(FPhysScene* InPhysicsScene);
	 virtual ~FCUBICARPhysicsReplication() {}
 
 protected:
     virtual void OnTick(float DeltaSeconds, TMap<TWeakObjectPtr<UPrimitiveComponent>, FReplicatedPhysicsTarget>& ComponentsToTargets) override;

	 virtual float GetAuthoritativeSimPing(AActor* const Owner, const FReplicatedPhysicsTarget& Target) const;

	 virtual float GetLocalSimPing() const;

	 bool TryApplyRigidBodyState(float DeltaSeconds, FBodyInstance* BI, FReplicatedPhysicsTarget& PhysicsTarget, const FRigidBodyErrorCorrection& ErrorCorrection, const float PingSecondsOneWay);



 private:
	 FPhysScene* PhysScene;
 };
 
 class ICUBICARPhysicsReplicationFactory : public IPhysicsReplicationFactory
 {
 public: 
	 FPhysicsReplication* Create(FPhysScene* OwningPhysScene) override final;
	 void Destroy(FPhysicsReplication* PhysicsReplication) override final;
	 virtual ~ICUBICARPhysicsReplicationFactory() {};
 };
 */