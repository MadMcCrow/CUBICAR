// Fill out your copyright notice in the Description page of Project Settings.

#include "PitLanePosition.h"
#include "Components/DecalComponent.h"
#include "Materials/Material.h"
#include "WorldCollision.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"


APitLanePosition::APitLanePosition(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	StartingDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("PitDecal"));
	if (StartingDecal && DecalMaterial)
	{
		StartingDecal->SetupAttachment(RootComponent);
		StartingDecal->SetDecalMaterial(DecalMaterial);
		StartingDecal->DecalSize = FVector(32.f, 32.f, 32.f);
		StartingDecal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f).Quaternion());
		
	}
	
}

bool APitLanePosition::IsAvailable()
{
	FVector OrigLocation = GetActorLocation();
	bool bResult = true;
	const float Radius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	FVector const Slice(Radius, Radius, 1.f);
	// Check for adjustment
	FHitResult Hit(ForceInit);
	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = GetActorLocation() - FVector(0.f, 0.f, 4.f * GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeBox(Slice), FCollisionQueryParams(SCENE_QUERY_STAT(NavObjectBase_Validate), false, this));
	if (Hit.bBlockingHit)
	{
		const FVector HitLocation = TraceStart + (TraceEnd - TraceStart) * Hit.Time;
		FVector Dest = HitLocation + FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 2.f);

		// Move actor (TEST ONLY) to see if navigation point moves
		TeleportTo(Dest, GetActorRotation(), false, true);

		// If only adjustment was down towards the floor, then it is a valid placement
		FVector NewLocation = GetActorLocation();
		bResult = (NewLocation.X == OrigLocation.X &&
			NewLocation.Y == OrigLocation.Y &&
			NewLocation.Z <= OrigLocation.Z);
		
		// Move actor back to original position
		TeleportTo(OrigLocation, GetActorRotation(), false, true);
		return bResult;
	}
	return bResult;
}

