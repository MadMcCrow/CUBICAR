// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "RaceWheel.h"
#include "CarInfo.generated.h"


class AController;
class ARacingCar;

/**
 *	Structure to store all Car info to display to player
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Car Dashboard"))
struct FCarDashboardInfo
{
	GENERATED_BODY()
public:

	/** Engine RPM - Tachymeter */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RPM;

	/** Transmission */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Gear;

	/** Car Speed - Tachymeter */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;

	/**	Info if the car is driving	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsDriftfing;

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


	FCarStatsInfo() : MaxHP(0.f), MaxGear(0), MaxRPM(0.f), Mass(0.f), Dimensions(FVector(0.f)) {}

	FCarStatsInfo(const ARacingCar * Car);
};



// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCarInfo : public UInterface
{
	GENERATED_BODY()
};

/**
 *  This class is meant to be used to export values from the car to other modules (like user interface) 
 */
class RACE_API ICarInfo
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, Category = "Race Car|DashBoard")
	virtual FCarDashboardInfo GetPlayerCarDasboardInfo(const AController * Player) const;

	UFUNCTION(BlueprintCallable, Category = "Race Car|Statistics")
	virtual FCarStatsInfo GetPlayerCarStatisticsInfo(const AController * Player) const;

};
