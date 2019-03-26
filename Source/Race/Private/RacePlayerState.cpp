// Fill out your copyright notice in the Description page of Project Settings.

#include "RacePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "RaceGameMode.h"
#include "Race.h"
#include "RaceGameState.h"
#include "Checkpoint.h"
#include "UnrealNetwork.h"

ACheckpoint* ARacePlayerState::GetLastPassedCheckpoint()
{
	ACheckpoint * RetCheckpoint;
	FTimespan TimeScore;

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
	return nullptr;
	
}


// Ask Server if we really passed Checkpoint
bool ARacePlayerState::Server_PassedCheckpoint_Validate(const AController* Player, ACheckpoint* PassedCheckpoint)
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

void ARacePlayerState::Server_PassedCheckpoint_Implementation(const AController* Player,ACheckpoint* PassedCheckpoint)
{
	const auto GS = Cast<ARaceGameState>(UGameplayStatics::GetGameState(this));
	PassedCheckpoints.Add(FCheckpointScore(PassedCheckpoint, GS->RaceTime()));
	bShouldResetAtPit = false;
}

void ARacePlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
	OwningPlayer = C;
	bShouldResetAtPit = true;
}

void ARacePlayerState::NotifyPassedCheckpoint() const
{
}

void ARacePlayerState::OnPassedCheckpoint(ACheckpoint* PassedCheckpoint)
{
	if (!PassedCheckpoint)
		return;

	if(OwningPlayer)
		Server_PassedCheckpoint(OwningPlayer, PassedCheckpoint);
}

void ARacePlayerState::GetLifetimeReplicatedProps(class TArray<class FLifetimeProperty, class FDefaultAllocator> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARacePlayerState, bShouldResetAtPit);
	DOREPLIFETIME(ARacePlayerState, PassedCheckpoints);
}
