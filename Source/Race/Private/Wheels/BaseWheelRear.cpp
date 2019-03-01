// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BaseWheelRear.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"

UBaseWheelRear::UBaseWheelRear()
{
	ShapeRadius = 18.0f;
	ShapeWidth = 15.0f;
	bAffectedByHandbrake = true;
	SteerAngle = 0.f;

	// Setup suspension forces
	SuspensionForceOffset = -4.0f;
	SuspensionMaxRaise = 3.0f;
	SuspensionMaxDrop = 5.0f;
	SuspensionNaturalFrequency = 9.0f;
	SuspensionDampingRatio = 2.05f;

	// Find the tire object and set the data for it
	static ConstructorHelpers::FObjectFinder<UTireConfig> TireData(TEXT("/Game/Vehicles/WheelData/Vehicle_BackTireConfig.Vehicle_BackTireConfig"));
	TireConfig = TireData.Object;
}
