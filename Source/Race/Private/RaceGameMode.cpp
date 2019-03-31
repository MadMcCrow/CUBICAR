// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceGameMode.h"
#include "RacePlayerState.h"
#include "RaceGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Checkpoint.h"
#include "RaceStatics.h"
#include "RaceCar.h"
#include "EngineUtils.h"
#include "PitLanePosition.h"
#include "LapStart.h"


ARaceGameMode::ARaceGameMode() : Super()
{
	GameStateClass =   ARaceGameState::StaticClass();
	PlayerStateClass = ARacePlayerState::StaticClass();
}

/*
void ARaceGameMode::BeginPlay()
{
	Super::BeginPlay();
	UpdatesCheckpointList();
}
*/

void ARaceGameMode::PopulateTeams(const UDataTable* TeamsTable)
{
	URaceStatics::GetRaceGameState(this)->SetTeams(URaceStatics::GetStaticTeamData(TeamsTable));
}

void ARaceGameMode::PopulateItems(const UDataTable* ItemsTable)
{

}
/*
AActor* ARaceGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// Choose a player start
	APlayerStart* FoundPlayerStart = nullptr;
	UClass* PawnClass = GetDefaultPawnClassForController(Player);
	APawn* PawnToFit = PawnClass ? PawnClass->GetDefaultObject<APawn>() : nullptr;
	TArray<APlayerStart*> UnOccupiedStartPoints;
	TArray<APlayerStart*> OccupiedStartPoints;
	UWorld* World = GetWorld();
	for (TActorIterator<APitLanePosition> It(World); It; ++It)
	{
		APlayerStart* PlayerStart = *It;

		if (PlayerStart->IsA<APitLanePosition>())
		{
			// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			FoundPlayerStart = PlayerStart;
			break;
		}
		else
		{
			FVector ActorLocation = PlayerStart->GetActorLocation();
			const FRotator ActorRotation = PlayerStart->GetActorRotation();
			if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
			{
				UnOccupiedStartPoints.Add(PlayerStart);
			}
			else if (World->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
			{
				OccupiedStartPoints.Add(PlayerStart);
			}
		}
	}
	if (FoundPlayerStart == nullptr)
	{
		if (UnOccupiedStartPoints.Num() > 0)
		{
			FoundPlayerStart = UnOccupiedStartPoints[FMath::RandRange(0, UnOccupiedStartPoints.Num() - 1)];
		}
		else if (OccupiedStartPoints.Num() > 0)
		{
			FoundPlayerStart = OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
		}
	}
	return FoundPlayerStart;
}

*/


#if 0


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
	if (Role == ROLE_Authority)
	{
		if (!controller)
			return SectionScore;
		auto PS = URaceStatics::GetRacePlayerState(this, controller);
		if (!PS)
			return SectionScore;

		int time = CalculateTimeScore(controller);
		SectionScore.CharismaScore = CharismaFactor *CalculateCharismaScore(controller);
		SectionScore.DriftScore =	 DriftFactor *	CalculateDriftScore(controller);
		SectionScore.SetTimeScore(time * TimeFactor);
		PS->LapPlayerScores.Add(SectionScore);
	}
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


/*

float ARaceGameMode::CalculateDriftScore_Implementation(AController* controller) const
{

	URaceStatics::GetRacePlayerState(this, controller);
	URaceStatics::GetControllersCar(controller);
	return 0.f;
}
	
float ARaceGameMode::CalculateTimeScore(AController* controller) const
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
	//URaceStatics::GetRacePlayerState(this, controller);
	return 0.f;
	
}
*/