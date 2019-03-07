// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RaceWheel.h"
#include "RaceTireConfig.h"


URaceWheel::URaceWheel() :Super()
{
	ShapeRadius = 35.f;
	ShapeWidth = 10.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 50.f;

	// Setup suspension forces
	SuspensionForceOffset = -4.0f;
	SuspensionMaxRaise = 8.0f;
	SuspensionMaxDrop = 12.0f;
	SuspensionNaturalFrequency = 9.0f;
	SuspensionDampingRatio = 1.05f;

	// Find the tire object and set the data for it
	//static ConstructorHelpers::FObjectFinder<UTireConfig> TireData(TEXT("/Game/VehicleAdv/Vehicle/WheelData/Vehicle_FrontTireConfig.Vehicle_FrontTireConfig"));
	//TireConfig = TireData.Object;
	ShapeRadius = 18.f;
	ShapeWidth = 15.0f;

	TireConfig = CreateDefaultSubobject<URaceTireConfig>(TEXT("RaceTireConfig"));

	// Setup suspension forces
	const FRaceWheelConfig DefaultConfig;

	SetFromWheelConfigStruct(DefaultConfig);
}


void URaceWheel::SetFromWheelConfigStruct(const FRaceWheelConfig& NewWheelConfig)
{
	// Setup suspension forces
	SuspensionForceOffset =			NewWheelConfig.Suspension.SuspensionForceOffset;
	SuspensionMaxRaise =			NewWheelConfig.Suspension.SuspensionMaxRaise;
	SuspensionMaxDrop =				NewWheelConfig.Suspension.SuspensionMaxDrop;
	SuspensionNaturalFrequency =	NewWheelConfig.Suspension.SuspensionNaturalFrequency;
	SuspensionDampingRatio =		NewWheelConfig.Suspension.SuspensionDampingRatio;


	// Setup tire
	if (!TireConfig)
		return;

	TireConfig->SetFrictionScale(NewWheelConfig.Tire.FrictionScale);

	for (const auto it : NewWheelConfig.Tire.PhysicalMaterialFrictions)
		TireConfig->SetPerMaterialFrictionScale(it.PhysicalMaterial, it.FrictionScale);

	// Wheel setup
	bAffectedByHandbrake = NewWheelConfig.bAffectedByHandbrake;
	SteerAngle = NewWheelConfig.SteerAngle;

	// remove steering based on boolean
	if (NewWheelConfig.bCanSteer)
		SteerAngle = 0.f;
}
