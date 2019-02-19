// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseCar.h"
#include "RaceWheel.h"
#include "Curves/CurveFloat.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "RacingCar.generated.h"



class ACheckpoint;
class UPhysicalMaterial;
class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;
class UAudioComponent;

class USkeletalMesh;
class USoundCue;

class ARacingCar;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartSkidding);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopSkidding);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpsideDown);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDrift, float, DriftAmount);

/**
 *	Structure to store all Car info to display to player
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Car Dashboard"))
struct FCarDashboardInfo
{
	GENERATED_BODY()
public :

	/** Engine RPM - Tachymeter */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RPM;

	/** Transmission */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Gear;

	/** Car Speed - Tachymeter */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;

};

USTRUCT(BlueprintType, meta = (DisplayName = "Car Statistics"))
struct FCarStatsInfo
{
	GENERATED_BODY()
public:

	// Steering Curve
	UPROPERTY(EditAnywhere, Category = "Steering")
		FRuntimeFloatCurve SteeringCurve;

	// Transmission Data
	UPROPERTY(EditAnywhere, Category = "Transmission")
		FVehicleTransmissionData TransmissionSetup;

	// Engine Data
	UPROPERTY(EditAnywhere, Category = "Engine")
		FVehicleEngineData EngineSetup;
	
	/** Front Wheels Class **/
	UPROPERTY(EditAnywhere, Category = "Wheels")
		TSubclassOf<URaceWheel> FrontWheelClass;

	/** Rear Wheels Class **/
	UPROPERTY(EditAnywhere, Category = "Wheels")
		TSubclassOf<URaceWheel> RearWheelClass;

	/** Max Horse Power */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max Horse Power"))
		float MaxHP;

	/** Max Gear  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max Transmission Gear"))
		int MaxGear;

	/** Max RPM*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Maximum Engine RPM"))
		float MaxRPM;

	/** Max RPM*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Car Mass"))
		float Mass;

	/** Max RPM*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Car Dimensions"))
		FVector Dimensions;

	
	FCarStatsInfo() : MaxHP (0.f), MaxGear(0), MaxRPM(0.f),  Mass(0.f), Dimensions(FVector(0.f)) {}
	
	FCarStatsInfo(const ARacingCar * Car);
};


/**
 *	Configuration for the Wheel.
 *	Class to build data Asset from
 */
UCLASS()
class RACE_API UCarAsset : public UDataAsset // UPrimaryDataAsset
{
	GENERATED_BODY()

private:

	/** Front Wheels Class **/
	UPROPERTY(EditAnywhere)
		FCarStatsInfo CarStats;
public:
	/**
	* Getter for FrictionScale
	*/
	UFUNCTION(BlueprintPure)
		FCarStatsInfo GetCarConfig() const { return CarStats; }

	/**
	* Setter for FrictionScale
	*/
	UFUNCTION(BlueprintCallable)
		void SetCarConfig(FCarStatsInfo NewCarStatsInfo) { CarStats = NewCarStatsInfo; }
};



UCLASS(config=Game)
class RACE_API ARacingCar : public ABaseCar
{
	GENERATED_BODY()

protected:

	/** Car Speed, Engine Gear, etc ... All in one place **/
	UPROPERTY(Category = "Car", BlueprintReadOnly)
		FCarDashboardInfo CarInfo;


public:

	/** Get a copy of the Car in game values **/
	UFUNCTION(BlueprintPure, Category = "Car", meta = (DisplayName = "Get Car Dashboard Values"))
	FCarDashboardInfo GetCarDasboard() const { return CarInfo; }

	/** Get the Car Statistics (HorsePower, Max Gear, MaxRPM, Dimensions, Mass, ...) **/
	UFUNCTION(BlueprintPure, Category = "Car", meta = (DisplayName = "Get Car Statistics"))
		FCarStatsInfo GetCarStats();

protected:

	UFUNCTION()
		virtual void UpdateCarInfo();


public:
	ARacingCar();


	// Begin Pawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End Pawn interface

	// Begin Actor interface
	virtual void Tick(float Delta) override;
protected:
	virtual void BeginPlay() override;

protected :

	UPROPERTY(EditDefaultsOnly, Category = "Car")
	UCarAsset * CarSetup;


	// Reset --------------------------------------------------------
protected :

	/** Reset Car To it's current place **/
	UFUNCTION(BlueprintCallable, Category = "Car|Control" )
		void ResetCar();

	/** How much you have to wait before reseting again **/
	UPROPERTY(EditDefaultsOnly, Category = "Car|Control") // Replicated
		float ResetDelay;

	/** Called At Tick to ensure the car is not upside down **/
	virtual void CheckForUpsideDown();

	/**At what speed we consider the vehicle as stopped **/
	UPROPERTY(EditDefaultsOnly, Category = "Car|Control") // Replicated
		float IsStoppedThreshold;

	/**Event triggered when car is upside down **/
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Events")
		FUpsideDown OnCarIsUpsideDown;
private :

	/**	Timer Handle to handle reset delay **/
	FTimerHandle ResetDelayHandle;

	/** Boolean flag to know if you can reset **/
	UPROPERTY(Transient) // Replicated
		bool bCanReset;

	/** Change the boolean flag, called via timer **/
		FORCEINLINE void SetCanReset() { bCanReset = true; }

	/** Actual function that affect the car roation, speed , etc **/
	UFUNCTION()
	virtual void ResetCarRotation();


	// Stability -------------------------------------------------------
protected:

	UFUNCTION(BlueprintCallable, Category = "Car|Stability")
	virtual void Stabilize(const float &Stability, const float &StabilizingSpeed );

	// Drifting --------------------------------------------------------
protected :

	/** Boolean Flag  to check if you're skidding **/
	UPROPERTY(Transient)
		bool IsSkidding;

	/** Minimum Front/Back Skidding **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car|Skidding")
		float LongitudinalSlipThreshold;

	/** Minimum Left/Right Skidding **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car|Skidding")
		float LateralSlipThreshold;

public:
	/** Skidding Start Event **/
	UPROPERTY(BlueprintAssignable, Category = "Car|Events")
		FStartSkidding StartSkidding;

	/** Skidding End Event **/
	UPROPERTY(BlueprintAssignable, Category = "Car|Events")
		FStartSkidding StopSkidding;

protected:
	/** Drift Event **/
	UFUNCTION(blueprintNativeEvent, Category = "Car|Skidding")
		void Drift();

	/** End Drift Event **/
	UFUNCTION(blueprintNativeEvent, Category = "Car|Skidding")
		void EndDrift();

private :

	/** Performs Skidding Check, Called at tick **/
	UFUNCTION()
		void CheckSkidding();

// Checkpoint -------------------------------------------------------
protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Car|Checkpoint")
		void PassedCheckpoint(ACheckpoint * Checkpoint);



};
