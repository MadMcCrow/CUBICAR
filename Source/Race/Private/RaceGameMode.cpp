// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceGameMode.h"
#include "RacePlayerState.h"
#include "RaceGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Checkpoint.h"
#include "RaceStatics.h"
#include "RaceCar.h"
#include "PitLanePosition.h"
#include "LapStart.h"


ARaceGameMode::ARaceGameMode() : Super()
{
	GameStateClass =   ARaceGameState::StaticClass();
	PlayerStateClass = ARacePlayerState::StaticClass();
}

void ARaceGameMode::BeginPlay()
{
	Super::BeginPlay();
	UpdatesCheckpointList();
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

	TArray<AActor *> Starts;
	UGameplayStatics::GetAllActorsOfClass(this, ALapStart::StaticClass(), Starts);
	if (Starts.IsValidIndex(0))
		RaceStart = Cast<ALapStart>(Starts[0]);

}

FRaceScore ARaceGameMode::CalculateScore(AController* controller) const
{
	FRaceScore SectionScore;
	auto PS = URaceStatics::GetRacePlayerState(this, controller);
	if (!PS)
		return SectionScore;
	SectionScore.CharismaScore = FMath::CeilToInt(CharismaFactor *CalculateCharismaScore(controller));
	SectionScore.DriftScore =	 FMath::CeilToInt(DriftFactor *	CalculateDriftScore(controller));
	SectionScore.TimeScore =	 FMath::CeilToInt(TimeFactor *	CalculateTimeScore(controller));
	PS->LapPlayerScores.Add(SectionScore);
	return SectionScore;
}

FRaceScore ARaceGameMode::CalculateLapScore(AController* controller) const
{
	FRaceScore LapScore;
	auto PS = URaceStatics::GetRacePlayerState(this, controller);
	if (!PS)
		return LapScore;
	for (auto it : PS->LapPlayerScores)
	{
		LapScore.DriftScore += it.GetDriftScore();
		LapScore.CharismaScore += it.GetCharismaScore();
		LapScore.TimeScore += it.GetTimeScore();
	}
	PS->GamePlayerScores.Add(LapScore);
	return LapScore;
}




float ARaceGameMode::CalculateDriftScore_Implementation(AController* controller) const
{

	URaceStatics::GetRacePlayerState(this, controller);
	URaceStatics::GetControllersCar(controller);
	return 0.f;
}
	
float ARaceGameMode::CalculateTimeScore_Implementation(AController* controller) const
{
	const auto GS = URaceStatics::GetRaceGameState(this);

	auto PS = URaceStatics::GetRacePlayerState(this, controller);
	if (!PS)
		return 0.f;
	if (PS->PassedCheckpoints.Num() <= 0)
		return 0.f;

	const auto CheckStruct = PS->PassedCheckpoints.Last();
	FTimespan ThisPlayerTime = CheckStruct.Time;

	FTimespan bestOnSection = GS->RaceTime();
	for (auto it : GS->PlayerArray)
	{
		auto asRacePS = Cast<ARacePlayerState>(it);
		if (asRacePS->PassedCheckpoints.Num() != 0)
		{
			const auto PassC = asRacePS->PassedCheckpoints;
			for (auto c : PassC)
				if (c.Time <= bestOnSection)
				{
					bestOnSection = c.Time;
				}
		}
	}
	return (float)bestOnSection.GetSeconds() / (float)ThisPlayerTime.GetSeconds();
}
	
float ARaceGameMode::CalculateCharismaScore_Implementation(AController* controller) const
{
	URaceStatics::GetRacePlayerState(this, controller);
	return 0.f;
	
}
