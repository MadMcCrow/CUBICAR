// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceTime.h"
#include "Kismet/GameplayStatics.h"
#include "RaceGameState.h"


FRaceTime FRaceTime::SinceRaceStart(const UObject* WorldContextObject)
{
	const auto GS = Cast<ARaceGameState>(UGameplayStatics::GetGameState(WorldContextObject));
	return FRaceTime(GS->GetTimeSinceStart());
}
