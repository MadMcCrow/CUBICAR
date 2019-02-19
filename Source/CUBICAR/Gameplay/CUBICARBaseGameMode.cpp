// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CUBICARBaseGameMode.h"
#include "CUBICARBaseGameState.h"
#include "RacingCar.h"
#include "CUBICARHud.h"
#include "Kismet/GameplayStatics.h"
#include "Checkpoint.h"
#include "CUBICARPlayerState.h"

ACUBICARBaseGameMode::ACUBICARBaseGameMode()
{

}


void ACUBICARBaseGameMode::StartPlay()
{
	Super::StartPlay();
	UpdatesCheckpointList();
}

void ACUBICARBaseGameMode::UpdatesCheckpointList()
{
	TArray<AActor *> Checkpoints;
	UGameplayStatics::GetAllActorsOfClass(this, ACheckpoint::StaticClass(), Checkpoints);
	for (auto it : Checkpoints)
	{
		const auto cit = Cast<ACheckpoint>(it);
		MapCheckpoints.Add(cit);
	}
	// Sort the checkpoints by index
	MapCheckpoints.Sort([](const ACheckpoint& A, const ACheckpoint& B)
	{
		return A.GetIndex() < B.GetIndex();
	});
}
