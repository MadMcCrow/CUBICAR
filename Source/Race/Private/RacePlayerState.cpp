// Fill out your copyright notice in the Description page of Project Settings.

#include "RacePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "RaceGameMode.h"
#include "Race.h"

ACheckpoint* ARacePlayerState::GetLastPassedCheckpoint()
{
	ACheckpoint * RetCheckpoint;
	FRaceTime TimeScore;
	PassedCheckpoints.Sort([](FCheckpointScore A, FCheckpointScore B) {
		return A < B;
	});

	if (PassedCheckpoints.Num() >= 1)
	{
		PassedCheckpoints[PassedCheckpoints.Num() - 1].GetCheckpointScore(RetCheckpoint, TimeScore);
		return RetCheckpoint;
	};

	const auto PC = Cast<APlayerController>(GetOwner());
	if (!PC)
		return nullptr;
	const auto GM =UGameplayStatics::GetGameMode(this);
	if (!GM)
		return nullptr;

	return  Cast<ACheckpoint>(GM->ChoosePlayerStart(PC));
	
}


// Ask Server if we really passed Checkpoint
bool ARacePlayerState::PassedCheckpoint_Validate(const APlayerController* Player, ACheckpoint* PassedCheckpoint)
{
	if (!Player)
		return false;
	if (!PassedCheckpoint)
		return false;
	if (GetOwner() != Player)
	{
		UE_LOG(LogRace, Warning, TEXT("Warning: Wrong Controller used for Passed Checkpoint "));
		return false;
	}
	return true;

}

void ARacePlayerState::PassedCheckpoint_Implementation(const APlayerController* Player,ACheckpoint* PassedCheckpoint)
{
	const FCheckpointScore CP(PassedCheckpoint, FRaceTime::SinceRaceStart(this));
	PassedCheckpoints.Add(CP);
}
