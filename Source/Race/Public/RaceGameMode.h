// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RaceGameMode.generated.h"


class UDataTable;
class ACheckpoint;
class ALapStart;


/**
 *  Base Game mode for the Races
 */
UCLASS()
class RACE_API ARaceGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ARaceGameMode();


	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* Teams;

private:


	UFUNCTION()
		void PopulateTeams(const UDataTable* TeamsTable);

	UFUNCTION()
		void PopulateItems(const UDataTable* ItemsTable);

protected:

	//	Checkpoints		------------------------------------------------------------------------------------------

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;


	UFUNCTION(BlueprintPure)
		ALapStart * GetRaceStart() const { return RaceStart; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Checkpoint", meta = (DisplayName = "Checkpoints On Map"))
		TArray<ACheckpoint *> MapCheckpoints;

	UPROPERTY(BlueprintReadOnly, Category = "Checkpoint", meta = (DisplayName = "Race Start"))
		ALapStart * RaceStart;

public:

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR


private:
	void UpdatesCheckpointList();

 };
