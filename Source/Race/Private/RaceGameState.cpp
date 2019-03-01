// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceGameState.h"
#include "TimerManager.h"


void ARaceGameState::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	RaceStartOffset = FDateTime::Now().GetTimeOfDay();

}

FTimespan ARaceGameState::RaceTime() const
{
	return FDateTime::Now().GetTimeOfDay() - RaceStartOffset;
}


