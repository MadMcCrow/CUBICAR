// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CarStatics.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "SimpleCar.generated.h"



class UAudioComponent;
class USkeletalMeshComponent;
class UArrowComponent;
class UStaticMeshComponent;
class USoundCue;
class USpringArmComponent;
class UCameraComponent;
class UCurveFloat;
class UParticleSystemComponent;




UCLASS()
class RACE_API ASimpleCar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASimpleCar();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;



private :
#if WITH_EDITOR
	//UPROPERTY( Category = VehicleSetup, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UArrowComponent* Arrow0;
	//UPROPERTY( Category = VehicleSetup, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UArrowComponent* Arrow1;
	//UPROPERTY( Category = VehicleSetup, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UArrowComponent* Arrow2;
	//UPROPERTY( Category = VehicleSetup, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		 UArrowComponent* Arrow3;

	//arrow array
	//UPROPERTY(EditAnywhere, Category = VehicleSetup)
		TArray<UArrowComponent*> ArrowArray;

#endif // WITH_EDITOR

	/**  The main skeletal mesh associated with this Vehicle */
	UPROPERTY(Category = "Vehicle", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* Mesh;


	// Car Wheels Components	---------------------------------------------------------------------- 
protected:

	UPROPERTY(Category = "Vehicle", EditDefaultsOnly, BlueprintReadOnly) //,meta = (AllowPrivateAccess = "true"))
		UStaticMesh * WheelMesh;

private :
	//wheel meshes
	UPROPERTY( Category = VehicleSetup, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Wheel0;
	UPROPERTY( Category = VehicleSetup, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Wheel1;
	UPROPERTY( Category = VehicleSetup, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Wheel2;
	UPROPERTY( Category = VehicleSetup, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Wheel3;

	TArray<UStaticMeshComponent *> WheelArray;

protected :

	UFUNCTION(BlueprintPure)
		TArray<UStaticMeshComponent *> GetWheelArray();



	// Car Physics	----------------------------------------------------------------------
private:
	FCalculateCustomPhysics OnCalculateCustomPhysics;

	void CustomPhysics(float DeltaTime, FBodyInstance* BodyInstance);
	FHitResult Trace(FVector TraceStart, FVector TraceDirection);
	FBodyInstance *MainBodyInstance;


	// Car Camera	----------------------------------------------------------------------
private :
	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;

protected :

	/** Name of the MeshComponent. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName VehicleMeshComponentName;

	void SpawnSmokeEffect(int WheelIndex);

	/** Handle pressing forwards */
	void MoveForward(float Val);

	/** Handle pressing right */
	void MoveRight(float Val);

	/** gear up */
	void ChangeUp();

	/** gear Down */
	void ChangeDown();

	FVector AddDrive(float DeltaTime, FBodyInstance* BodyInstance, FVector Loc, FVector Dir, int32 Index);
	FVector AddLatGrip(float DeltaTime, FBodyInstance* BodyInstance, FVector Loc, FVector Dir, int32 Index);

	float GetPowerToWheels(float DeltaTime, FBodyInstance* BodyInstance);

	void SpawnNewWheelEffect(int WheelIndex);

	/** update effects under wheels */
	void UpdateWheelEffects(float DeltaTime, int32 Index);


	// Car Dashboard	----------------------------------------------------------------------


protected:
	UPROPERTY(Transient)
		FCarDashBoard Dashboard;

public:

	FCarDashBoard GetCarDashboard() const { return Dashboard; }

	// Car Audio		---------------------------------------------------------------------- 
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Sound|Engine")
		FCarEngineSound CarSound;

private:
	/** audio component for engine sounds */
	UPROPERTY(Category = "Sound", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAudioComponent* EngineAC;

	// Car Engine		---------------------------------------------------------------------- 
protected :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|Engine")
	FCarEngineSetup EngineSetup;

	FCarEngineUpdate EngineUpdate;

	// Car Wheels		---------------------------------------------------------------------- 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|Wheels")
		FCarWheelsSetup WheelsSetup;

	FCarWheelsUpdate WheelsUpdate;

	// Car Suspensions	---------------------------------------------------------------------- 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|Suspension")
		FCarSuspensionsSetup SuspensionsSetup;

	FCarSuspensionsUpdate SuspensionsUpdate;

protected:

	// Car Effects		---------------------------------------------------------------------- 

	/** skid sound loop */
	UPROPERTY(Category = "Effect|Sound", EditDefaultsOnly)
		USoundCue* SkidSound;
	

	//tire smoke
	UPROPERTY(Category = "Effect|Particle", EditAnywhere)
		UParticleSystem* TireSmoke;
	UPROPERTY(Category = "Effect|Particle", EditAnywhere)
		UParticleSystem* TireMarks;

	/** dust FX components */
	UPROPERTY(Transient)
		UParticleSystemComponent* DustPSC[4];


	// Getter functions	---------------------------------------------------------------------- 

	/** Returns Mesh subobject **/
	class USkeletalMeshComponent* GetMesh() const;

	/** Returns SpringArm subobject **/
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

};
