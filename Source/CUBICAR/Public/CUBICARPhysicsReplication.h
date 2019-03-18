#pragma once
 
 #include "CoreMinimal.h"
 #include "PhysicsPublic.h"
 #include "PhysicsReplication.h"
 
 class FCUBICARPhysicsReplication : public FPhysicsReplication
 {
 public:
	 FCUBICARPhysicsReplication(FPhysScene* PhysScene);
	 virtual ~FCUBICARPhysicsReplication() {}
 
 protected:
     virtual void OnTick(float DeltaSeconds, TMap<TWeakObjectPtr<UPrimitiveComponent>, FReplicatedPhysicsTarget>& ComponentsToTargets) override;

 };
 
 class ICUBICARPhysicsReplicationFactory : public IPhysicsReplicationFactory
 {
 public: 
	 FPhysicsReplication* Create(FPhysScene* OwningPhysScene) override final;
	 void Destroy(FPhysicsReplication* PhysicsReplication) override final;
	 virtual ~ICUBICARPhysicsReplicationFactory() {};
 };
 