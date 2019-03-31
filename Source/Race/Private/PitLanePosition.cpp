// Fill out your copyright notice in the Description page of Project Settings.

#include "PitLanePosition.h"
#include "Components/DecalComponent.h"
#include "Materials/Material.h"
#include "WorldCollision.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "RaceCar.h"


APitLanePosition::APitLanePosition(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	StartingDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("PitDecal"));
	if (StartingDecal && DecalMaterial)
	{
		StartingDecal->SetupAttachment(RootComponent);
		StartingDecal->SetDecalMaterial(DecalMaterial);
		StartingDecal->DecalSize = FVector(150.f, 150.f, 150.f);
		StartingDecal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f).Quaternion());
	}
	
}

bool APitLanePosition::IsAvailable_Implementation()
{
	const auto T = GetCapsuleComponent()->GetOverlapInfos();
	for (const auto& it : T)
	{
		if (it.OverlapInfo.IsValidBlockingHit())
		{
			if(Cast<ARaceCar>(it.OverlapInfo.Actor.Get()))
				return false;
				
		}
	}
	return true;
}

