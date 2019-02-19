// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceTireConfig.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ConstructorHelpers.h"


URaceTireConfig::URaceTireConfig()
{
	
SetFrictionScale(3.f);

// Setup friction materials
static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> SlipperyMat(TEXT("/Game/PhysicsMaterials/Slippery.Slippery"));
UPhysicalMaterial * SlipperyMaterial = SlipperyMat.Object;

static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> NonSlipperyMat(TEXT("/Game/PhysicsMaterials/NonSlippery.NonSlippery"));
UPhysicalMaterial * NonSlipperyMaterial = NonSlipperyMat.Object;

	SetPerMaterialFrictionScale(SlipperyMaterial, 0.7);
	SetPerMaterialFrictionScale(NonSlipperyMaterial, 1.f);	

}
