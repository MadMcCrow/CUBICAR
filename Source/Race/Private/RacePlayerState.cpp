// Fill out your copyright notice in the Description page of Project Settings.

#include "RacePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "RaceGameMode.h"
#include "Race.h"
#include "RaceCar.h"
#include "RaceGameState.h"
#include "Checkpoint.h"
#include "UnrealNetwork.h"
#include "Engine/World.h"

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
		return true;
	}
	return true;

}

void ARacePlayerState::Server_PassedCheckpoint_Implementation(const AController* Player,ACheckpoint* PassedCheckpoint)
{
	const auto GS = URaceStatics::GetRaceGameState(this);
	const auto GM = URaceStatics::GetRaceGameMode(this);

	if(!GS || !GM)
	{
		UE_LOG(LogRace, Error, TEXT("Invalid Game Mode or Game State"));
		return;
	}
	
	if (PassedCheckpoint == (ACheckpoint*)GM->GetRaceStart()		// Are you at start ?
		&& PassedCheckpoints.Num() >= GM->GetCheckpointsNum() )		// Have you been through all of them Checkpoints ?
	{
		UE_LOG(LogRace, Display, TEXT("%s : my car finished a lap"), *GetPlayerName());
		PassedCheckpoints.Empty();
		GM->CalculateLapScore(OwningPlayer);
	}

	if (GetLastPassedCheckpoint())
	{
		if (PassedCheckpoint->GetIndex() <= GetLastPassedCheckpoint()->GetIndex())
		{
			return;
		}

		// TODO: Remove and use a contains instead
		// Double safety, slow
		for (auto it : PassedCheckpoints)
		{
			if(it.IsSameCheckpoint(PassedCheckpoint))
				return;
		}
	}


	FCheckpointScore C(PassedCheckpoint, URaceStatics::GetRaceGameState(this)->RaceTime());
	PassedCheckpoints.Add(C);
}

void ARacePlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
	OwningPlayer = C;
	bShouldResetAtPit = true;
}

void ARacePlayerState::BeginPlay()
{
	const auto Car = Cast<ARaceCar>(GetPawn());
	if(Car)
	{
		Car->CollisionAvoided.AddDynamic(this, &ARacePlayerState::PlayerCarAvoidedCollision);
	}
}

void ARacePlayerState::NotifyPassedCheckpoint() const
{
}

void ARacePlayerState::OnPassedCheckpoint(ACheckpoint* PassedCheckpoint)
{
	if (!PassedCheckpoint)
		return;
	UE_LOG(LogRace, Display, TEXT("%s : my car passed a checkpoint"), *GetPlayerName());
	
	if (!OwningPlayer)
	{
		if(GetPawn())
			OwningPlayer = GetPawn()->GetController();
	}
	if(OwningPlayer)
	{
		switch (Role)
		{
		case ROLE_AutonomousProxy:
			Server_PassedCheckpoint(OwningPlayer, PassedCheckpoint);
			Server_CalculateScore();
			return;
		default:
			Server_PassedCheckpoint_Implementation(OwningPlayer, PassedCheckpoint);
			Server_CalculateScore_Implementation();
			return;
		}
	}
	else
	{
		UE_LOG(LogRace, Error, TEXT("%s : I don't have a valid Owning Player"), *GetPlayerName());
	}
}

void ARacePlayerState::PlayerCarAvoidedCollision_Implementation()
{
	if(Role == ROLE_Authority)
	{
		UE_LOG(LogRace, Display, TEXT("Player %s Avoided Collision"), *GetPlayerName());
	}
}


bool ARacePlayerState::Server_CalculateScore_Validate() { return true; }
void ARacePlayerState::Server_CalculateScore_Implementation()
{
	const auto GM = URaceStatics::GetRaceGameMode(this);
	GM->CalculateScore(OwningPlayer);
}


bool ARacePlayerState::Server_UpdateTempDriftScore_Validate() { return true; }
void ARacePlayerState::Server_UpdateTempDriftScore_Implementation()
{
	const auto Car = Cast<ARaceCar>(GetPawn());
	if (Car->GetCarIsDrifting())
	{
		TempDriftTime += GetWorld()->GetDeltaSeconds();
	}
}

void ARacePlayerState::GetLifetimeReplicatedProps(class TArray<class FLifetimeProperty, class FDefaultAllocator> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARacePlayerState, OwningPlayer);
	DOREPLIFETIME(ARacePlayerState, bShouldResetAtPit);
	DOREPLIFETIME(ARacePlayerState, PassedCheckpoints);
	DOREPLIFETIME(ARacePlayerState, TempDriftTime);
	DOREPLIFETIME(ARacePlayerState, TempCharsimaScore);
	DOREPLIFETIME(ARacePlayerState, LapPlayerScores);
	DOREPLIFETIME(ARacePlayerState, GamePlayerScores);
	DOREPLIFETIME(ARacePlayerState, FinalPlayerScore);
	DOREPLIFETIME(ARacePlayerState, Laps);
}
