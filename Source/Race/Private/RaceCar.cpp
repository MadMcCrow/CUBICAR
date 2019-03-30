// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceCar.h"
#include "RaceStatics.h"
#include "RacePlayerState.h"
#include "UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"


ARaceCar::ARaceCar() : Super()
{
	CarEngine = CreateDefaultSubobject<UCarEngine>(TEXT("CarEngineMesh"));
	CarEngine->SetupAttachment(RootComponent);
	TeamColor = FLinearColor(255, 0, 255);

	GetMesh()->OnComponentHit.AddDynamic(this, &ARaceCar::OnCollision);
	NearCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("NearCollision"));
	NearCollisionBox->SetupAttachment(RootComponent);
	FVector Extent, Center;
	GetActorBounds(false, Center, Extent);
	NearCollisionBox->SetBoxExtent(Extent *1.1); // Add 10 Percents
	NearCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ARaceCar::OnNearCollisionBeginOverlap);
	NearCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ARaceCar::OnNearCollisionEndOverlap);
	
}

void ARaceCar::BeginPlay()
{
	Super::BeginPlay();
	const auto PS = URaceStatics::GetRacePlayerState(this, GetController());
	if (PS)
		if (PS->GetTeam())
			TeamColor = PS->GetTeam()->GetTeamInfo().GetColor();
	else
		TeamColor = FLinearColor(255,0,255);
	UpdateWheels();

}

void ARaceCar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(CarIsDrifting())
	{
		if (Role == ROLE_AutonomousProxy)
			Server_SetIsDrifting(true);
		DriftTempLength += DeltaSeconds;
	}
	else if (DriftTempLength!= 0)
	{
		if (Role == ROLE_AutonomousProxy)
			Server_SetIsDrifting(false);
		OnAchievedDrift(DriftTempLength);
		DriftTempLength = 0;
	}
}

UStaticMesh* ARaceCar::GetWheelMesh()
{
	const auto WM = WheelUpgrade.GetMesh();
	if (WM)
		return WM;
	if (DefaultWheelMesh)
		return DefaultWheelMesh;
	return nullptr;
}

void ARaceCar::UpdateWheels()
{
	const auto M = GetWheelMesh();
	if (M)
	Super::UpdateWheels();
}

void ARaceCar::UpdateEngine()
{
	const auto M = EngineUpgrade.GetMesh();
	if (M)
		CarEngine->SetStaticMesh(M);
}

bool ARaceCar::Server_SetIsDrifting_Validate(bool IsDrift) { return true; }
void ARaceCar::Server_SetIsDrifting_Implementation(bool IsDrift)
{
	bIsDrifting = IsDrift;
}

bool ARaceCar::CarIsDrifting()
{
	const bool Sliding = WheelsUpdate.GetIsSlidingFront() || WheelsUpdate.GetIsSlidingBack();
	const auto FastEnough = GetDashboardInfo().GetSpeed() >= DriftSpeedThreshold;
	bBelowSpeedThreshold = Sliding || !FastEnough;
	return Sliding && FastEnough;
}

void ARaceCar::OnAchievedDrift_Implementation(float Length)
{

}

void ARaceCar::OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
		if (NearCollisionActors.Contains(OtherActor))
		{
			NearCollisionActors[OtherActor] = true;
		}
}

void ARaceCar::OnNearCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
		NearCollisionActors.Add(OtherActor, false);
}

void ARaceCar::OnNearCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
		if (NearCollisionActors.Contains(OtherActor))
		{
			if (NearCollisionActors[OtherActor] == true)
			{
				CollisionAvoided.Broadcast();
			}
		
			NearCollisionActors.Remove(OtherActor);
		}
	
}

/*
bool ARaceCar::Server_SetWheels_Validate(FRaceCarWheel * NewWheels)
{
	return true;
}

void ARaceCar::Server_SetWheels_Implementation(URaceCarWheel * NewWheels)
{
	if (!NewWheels)
		return;
	WheelUpgrade = NewWheels;
}
*/

bool ARaceCar::Server_SetEngine_Validate(const FEnginePart &NewEngine)
{
	return true;
}

void ARaceCar::Server_SetEngine_Implementation(const FEnginePart &NewEngine)
{
	EngineUpgrade = NewEngine;
}

void ARaceCar::GetLifetimeReplicatedProps(class TArray<class FLifetimeProperty>  &OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARaceCar, WheelUpgrade);
	DOREPLIFETIME(ARaceCar, EngineUpgrade);
	DOREPLIFETIME_CONDITION(ARaceCar, bIsDrifting, COND_SkipOwner);
	DOREPLIFETIME(ARaceCar, DriftSpeedThreshold);
	DOREPLIFETIME(ARaceCar, bBelowSpeedThreshold);
	
}
