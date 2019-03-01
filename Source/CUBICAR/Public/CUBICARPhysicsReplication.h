#pragma once
 
 #include "CoreMinimal.h"
 #include "PhysicsPublic.h"
 #include "PhysicsReplication.h"
 
 class CUBICAR_API FCUBICARPhysicsReplication : public FPhysicsReplication
 {
 public:
	 FCUBICARPhysicsReplication(FPhysScene* PhysScene);
 
 protected:
     virtual void OnTick(float DeltaSeconds, TMap<TWeakObjectPtr<UPrimitiveComponent>, FReplicatedPhysicsTarget>& ComponentsToTargets) override;
 };
 
 class CUBICAR_API ICUBICARPhysicsReplicationFactory : public IPhysicsReplicationFactory
 {
 public:
     ICUBICARPhysicsReplicationFactory();
 
     virtual FPhysicsReplication* Create(FPhysScene* OwningPhysScene) override;
     virtual void Destroy(FPhysicsReplication* PhysicsReplication) override;
 };
 