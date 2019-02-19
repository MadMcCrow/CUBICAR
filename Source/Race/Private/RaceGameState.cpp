// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceGameState.h"
#include "TimerManager.h"

float ARaceGameState::GetTimeSinceStart() const
{

}

void ARaceGameState::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();


}

FRaceTime ARaceGameState::TimeElapsedSinceMatchStart()
{
	return FRaceTime();
}
