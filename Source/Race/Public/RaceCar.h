// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleCar.h"
#include "Parts/CarEngine.h"
#include "Parts/CarWheel.h"
#include "RaceCar.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCollisionAvoidedDelegate);

class UBoxComponent;
class UCarEngine;

/**
 *	Base Car for CUBICAR car
 */
UCLASS()
class RACE_API ARaceCar : public ASimpleCar
{
	GENERATED_BODY()

public:
	ARaceCar();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;


protected:

	/**	Team Color used for the car		*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FLinearColor TeamColor;

	/**	Wheel Setup		*/
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly)
		FWheelPart WheelUpgrade;

	/**	Engine Setup	*/
	UPROPERTY(Replicated, EditDefaultsOnly,  BlueprintReadOnly)
		FEnginePart EngineUpgrade;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh * DefaultWheelMesh;



	virtual UStaticMesh * GetWheelMesh() override;

private:

	UPROPERTY(Category = Vehicle, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCarEngine * CarEngine;

public:
	//UFUNCTION(Server, reliable, WithValidation)
		//void Server_SetWheels(URaceCarWheel * NewWheels);

	UFUNCTION(Server, reliable, WithValidation)
		void Server_SetEngine(const FEnginePart &NewEngine);


		void UpdateWheels() override;

		void UpdateEngine();

		UFUNCTION(BlueprintPure)
			FORCEINLINE bool GetCarIsDrifting() const {return bIsDrifting;}


protected:

	UFUNCTION(BlueprintPure,meta = (DisplayName = "Test if Drifting") )
	virtual bool CarIsDrifting();

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Drift")
		bool bIsDrifting;

	UPROPERTY()
		float DriftTempLength;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Drift")
		float DriftSpeedThreshold;

	UFUNCTION(BlueprintNativeEvent, Category = "Drift")
		void OnAchievedDrift(float Length);

public:

	UPROPERTY(BlueprintReadOnly, Replicated)
		mutable bool bBelowSpeedThreshold;

private:

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetIsDrifting(bool IsDrift);

	UPROPERTY(Category = "Score", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent * NearCollisionBox;

	UPROPERTY(Replicated)
		TMap<AActor*, bool> NearCollisionActors;

public:

	UFUNCTION(BlueprintPure, Category = "Collision")
		FORCEINLINE TMap<AActor*, bool> GetNearCollisionActors() const { return NearCollisionActors; }

private:


	UFUNCTION()
		void OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnNearCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnNearCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	FCollisionAvoidedDelegate CollisionAvoided;

private :
	friend class ARacePlayerState;
};

