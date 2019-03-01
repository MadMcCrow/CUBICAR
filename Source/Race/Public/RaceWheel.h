// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VehicleWheel.h"
#include "TireConfig.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "RaceWheel.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "Suspension Config"))
struct FSuspensionConfig
{
	GENERATED_BODY()

	/** Vertical offset from where suspension forces are applied (along Z-axis) */
	UPROPERTY(EditAnywhere, Category = "Suspension")
		float SuspensionForceOffset;

	/** How far the wheel can go above the resting position */
	UPROPERTY(EditAnywhere, Category = "Suspension")
		float SuspensionMaxRaise; // default = 8.0f;

	/** How far the wheel can drop below the resting position */
	UPROPERTY(EditAnywhere, Category = "Suspension")
		float SuspensionMaxDrop; // default = 12.0f;

	/** Oscillation frequency of suspension. Standard cars have values between 5 and 10 */
	UPROPERTY(EditAnywhere, Category = "Suspension")
		float SuspensionNaturalFrequency; // default = 9.0f;

	/**
	 *	The rate at which energy is dissipated from the spring. Standard cars have values between 0.8 and 1.2.
	 *	values < 1 are more sluggish, values > 1 or more twitchy
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension")
		float SuspensionDampingRatio; // default = 1.05f;


	/** Default constructor set default values */
	FSuspensionConfig() : SuspensionForceOffset(-4.0f), SuspensionMaxRaise(3.0f), SuspensionMaxDrop(5.0f),
		SuspensionNaturalFrequency(12.0f), SuspensionDampingRatio(2.05f)
	{
		
	}

	virtual void SetWheel(UVehicleWheel * Wheel) const
	{
		Wheel->SuspensionDampingRatio = SuspensionDampingRatio;
		Wheel->SuspensionForceOffset = SuspensionForceOffset;
		Wheel->SuspensionMaxDrop = SuspensionMaxDrop;
		Wheel->SuspensionMaxRaise = SuspensionMaxRaise;
		Wheel->SuspensionNaturalFrequency = SuspensionNaturalFrequency;
	}
};

USTRUCT(BlueprintType, meta = (DisplayName = "Tire Config"))
struct FTireFrictionConfig
{
	GENERATED_BODY()

	/** Vertical offset from where suspension forces are applied (along Z-axis) */
	UPROPERTY(EditAnywhere, Category = "Suspension")
		float FrictionScale;

	/** Physical material for friction scale  */
	UPROPERTY(EditAnywhere, Category = "Tire")
		TArray<FTireConfigMaterialFriction>	PhysicalMaterialFrictions;

	/** Default constructor set default values */
	FTireFrictionConfig() : FrictionScale(3.f){}

	virtual void SetWheel(UVehicleWheel * Wheel) const
	{
		
	}
};


/**
 *	Structure to store  the configuration for the Wheel
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Wheel Config"))
struct FRaceWheelConfig
{
	GENERATED_BODY()


	/** Suspension Config  */
	UPROPERTY(EditAnywhere, Category = "Tire")
		FTireFrictionConfig Tire;

	/** Suspension Config  */
	UPROPERTY(EditAnywhere, Category = "Suspension")
		FSuspensionConfig Suspension;

	/** Boolean flag to enable steering  */
	UPROPERTY(EditAnywhere, Category = "Wheel")
		bool bCanSteer;

	/** max steer angle for the road, ignored if bCanSteer is set to false  */
	UPROPERTY(EditAnywhere, Category = "Wheel")
		float SteerAngle;

	/** Can Wheel block when handbrake is activated  */
	UPROPERTY(EditAnywhere, Category = "Wheel")
		bool bAffectedByHandbrake;

	/** Default constructor set default values  */
	FRaceWheelConfig() : Suspension(), bCanSteer(false), SteerAngle(40.f), bAffectedByHandbrake(false)
	{

	}
	void SetWheel(UVehicleWheel * Wheel) const
	{
		Suspension.SetWheel(Wheel);
		Tire.SetWheel(Wheel);
	}
};

/**
 *	Base Wheel Class
 */
UCLASS()
class URaceWheel : public UVehicleWheel
{
	GENERATED_BODY()

public:
	URaceWheel();

	/**	Set from a predefined struct */
	UFUNCTION(BlueprintCallable, Category = "Data", meta = (DisplayName = "Set from Data Structure"))
		void SetFromWheelConfigStruct(const FRaceWheelConfig &NewWheelConfig);
};