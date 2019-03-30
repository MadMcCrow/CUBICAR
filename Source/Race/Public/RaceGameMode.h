// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RaceStatics.h"
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

public:
	UFUNCTION(BlueprintPure)
		ALapStart * GetRaceStart() const { return RaceStart; }

	UFUNCTION(BlueprintPure)
		int32 GetCheckpointsNum() const { return MapCheckpoints.Num(); }

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
	
public:

	UFUNCTION(BlueprintCallable, Category = "Score")
		FRaceScore CalculateScore(AController* controller) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Score")
	float CalculateDriftScore(AController* controller) const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Score")
	float CalculateTimeScore(AController* controller) const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Score")
	float CalculateCharismaScore(AController* controller) const;

	UFUNCTION(BlueprintCallable, Category = "Score")
		FRaceScore CalculateLapScore(AController* controller) const;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Score")
		int CharismaFactor = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Score")
		int TimeFactor = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Score")
		int DriftFactor = 100;

public :

	UFUNCTION(BlueprintPure, Category = "Score")
	int GetCharismaFactor() const { return CharismaFactor; }

	UFUNCTION(BlueprintPure, Category = "Score")
	int GetTimeFactor() const { return TimeFactor; }

	UFUNCTION(BlueprintPure, Category = "Score")
	int GetDriftFactor() const { return DriftFactor; }


 };
