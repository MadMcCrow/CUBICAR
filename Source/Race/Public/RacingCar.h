// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseCar.h"
#include "RaceWheel.h"
#include "Curves/CurveFloat.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "CarInfo.h"
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


USTRUCT(BlueprintType, meta = (DisplayName = "Wheel Physics Info"))
struct FWheelSlipInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		FName WheelName;

	UPROPERTY(BlueprintReadWrite)
		float LatSlipPercentage;

	UPROPERTY(BlueprintReadWrite)
		float LongSlipPercentage;

	UPROPERTY(BlueprintReadWrite)
		float LatSlipForce;

	UPROPERTY(BlueprintReadWrite)
		float LongSlipForce;

	FWheelSlipInfo(FName NewWheelName = FName(), float NewLatSlipPercentage = 0, float NewLongSlipPercentage = 0,
	               float NewLatSlipForce = 0, float NewLongSlipForce = 0) : WheelName(NewWheelName),
	                                                                        LatSlipPercentage(NewLatSlipPercentage),
	                                                                        LongSlipPercentage(NewLatSlipPercentage),
	                                                                        LatSlipForce(NewLatSlipForce),
	                                                                        LongSlipForce(NewLongSlipForce)
	{}
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

	void SetupWheels(UWheeledVehicleMovementComponent4W* Vehicle4W) override;

protected :


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
		bool IsLongSkidding;

	/** Boolean Flag  to check if you're skidding **/
	UPROPERTY(Transient)
		bool IsLatSkidding;

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

	UFUNCTION(BlueprintPure, Category = "Car|Skidding")
		TArray<FWheelSlipInfo> GetWheelsPhysInfo() const;

private :

	/** Performs Skidding Check, Called at tick **/
	UFUNCTION()
		void CheckSkidding();


// Checkpoint -------------------------------------------------------
protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Car|Checkpoint")
		void PassedCheckpoint(ACheckpoint * Checkpoint);



};
