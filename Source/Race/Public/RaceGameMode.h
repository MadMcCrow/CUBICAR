// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "StartPoint.h"
#include "RaceGameMode.generated.h"

class ACheckpoint;

/**
 *  Base Game mode for the Races
 */
UCLASS()
class RACE_API ARaceGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ARaceGameMode();

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;


	UFUNCTION(BlueprintPure)
	APlayerStart * GetRaceStart() const { return RaceStart; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Checkpoint", meta = (DisplayName = "Checkpoints On Map"))
		TArray<ACheckpoint *> MapCheckpoints;

	UPROPERTY(BlueprintReadOnly, Category = "Checkpoint", meta = (DisplayName = "Race Start"))
		APlayerStart * RaceStart;


private:
	void UpdatesCheckpointList();

 };
