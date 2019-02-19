// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceGameMode.h"
#include "RacingCar.h"
#include "RacePlayerState.h"
#include "RaceGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Checkpoint.h"


ARaceGameMode::ARaceGameMode() : Super()
{
	DefaultPawnClass = ARacingCar::StaticClass();
	GameStateClass =   ARaceGameState::StaticClass();
	PlayerStateClass = ARacePlayerState::StaticClass();
}

AActor* ARaceGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	const auto Start = Super::ChoosePlayerStart_Implementation(Player);
	if (!Player)
		return Start;
	const auto PState = Cast<ARacePlayerState>(Player->PlayerState);
	if (!PState)
		return Start;

	return PState->GetLastPassedCheckpoint();
}


void ARaceGameMode::UpdatesCheckpointList()
{
	TArray<AActor *> Checkpoints;
	UGameplayStatics::GetAllActorsOfClass(this, ACheckpoint::StaticClass(), Checkpoints);
	
	MapCheckpoints= reinterpret_cast<TArray<ACheckpoint*>&>(Checkpoints);
	//Stupid iteration to cast and add
	/*
	for (auto it : Checkpoints)
		MapCheckpoints.Add(static_cast<ACheckpoint*>(it));
	*/
	/*
	// Sort the checkpoints by index
	MapCheckpoints.Sort([](const ACheckpoint& A, const ACheckpoint& B) // Sort Checkpoints by Index
	{
		return A.GetIndex() < B.GetIndex();
	});
	*/
	MapCheckpoints.Sort(&ACheckpoint::SortCheckpoint);
	TArray<AActor *> Starts;
	UGameplayStatics::GetAllActorsOfClass(this, AStartPoint::StaticClass(),Starts);

	if (Starts.Num() >= 1)
	{
		RaceStart = Cast<APlayerStart>(Starts[0]);
	}
	else
	{
		const auto PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC)
			RaceStart = Cast<APlayerStart>(ChoosePlayerStart(PC));
	}

}
