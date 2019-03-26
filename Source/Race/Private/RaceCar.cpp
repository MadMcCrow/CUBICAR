// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceCar.h"
#include "RaceStatics.h"
#include "RacePlayerState.h"
#include "UnrealNetwork.h"


void ARaceCar::BeginPlay()
{
	//TeamColor = URaceStatics::GetRacePlayerState(this, GetController())->GetTeam()->GetTeamInfo().GetColor();

}

void ARaceCar::UpdateWheels()
{
}

void ARaceCar::UpdateEngine()
{
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
}
