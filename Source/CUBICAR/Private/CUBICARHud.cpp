// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CUBICARHud.h"

// Needed for VR Headset
#include "Engine.h"

#define LOCTEXT_NAMESPACE "VehicleHUD"

#ifndef HMD_MODULE_INCLUDED
#define HMD_MODULE_INCLUDED 0
#endif

ACUBICARHud::ACUBICARHud()
{
}

void ACUBICARHud::DrawHUD()
{
	Super::DrawHUD();
}

#undef LOCTEXT_NAMESPACE
