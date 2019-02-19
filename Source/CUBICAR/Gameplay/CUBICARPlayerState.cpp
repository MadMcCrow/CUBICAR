// Fill out your copyright notice in the Description page of Project Settings.

#include "CUBICARPlayerState.h"




void ACUBICARPlayerState::SetLastPassedCheckpoint(ACheckpoint* NewlyPassedCheckpoint)
{
	if (!NewlyPassedCheckpoint)
		return;
	LastCheckpointPassed = NewlyPassedCheckpoint;
}

