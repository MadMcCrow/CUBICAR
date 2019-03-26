// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceGameMode.h"
#include "RacePlayerState.h"
#include "RaceGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Checkpoint.h"
#include "RaceStatics.h"
#include "PitLanePosition.h"


ARaceGameMode::ARaceGameMode() : Super()
{
	GameStateClass =   ARaceGameState::StaticClass();
	PlayerStateClass = ARacePlayerState::StaticClass();
}

void ARaceGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void ARaceGameMode::PopulateTeams(const UDataTable* TeamsTable)
{
	URaceStatics::GetRaceGameState(this)->SetTeams(URaceStatics::GetStaticTeamData(TeamsTable));
}

void ARaceGameMode::PopulateItems(const UDataTable* ItemsTable)
{

}

AActor* ARaceGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	auto Start = Super::ChoosePlayerStart_Implementation(Player);
	TArray<AActor *> actorpits;
	UGameplayStatics::GetAllActorsOfClass(this, APitLanePosition::StaticClass(), actorpits);
	TArray<APitLanePosition *> Pits = reinterpret_cast<TArray<APitLanePosition*>&>(actorpits);
	Pits.Sort(&APitLanePosition::Sort);
	for (auto it : Pits)
		if (it->IsAvailable())
			Start = it;

	if (!Player)
		return Start;

	const auto PState = Cast<ARacePlayerState>(Player->PlayerState);
	if (!PState)
		return Start;

	if (PState->GetShouldRestartAtPit())
		return Start;

	const auto CheckpointStart = PState->GetLastPassedCheckpoint();
	if (!CheckpointStart)
		return Start;
	return CheckpointStart;
}



#if WITH_EDITOR


void ARaceGameMode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!URaceStatics::ValidateTeamDataTable(Teams))
	{
		Teams = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Wrong Type"));
	}
}

#endif // WITH_EDITOR



void ARaceGameMode::UpdatesCheckpointList()
{
	TArray<AActor *> Checkpoints;
	UGameplayStatics::GetAllActorsOfClass(this, ACheckpoint::StaticClass(), Checkpoints);
	
	MapCheckpoints= reinterpret_cast<TArray<ACheckpoint*>&>(Checkpoints);
	
	MapCheckpoints.Sort(&ACheckpoint::SortCheckpoint);

}
