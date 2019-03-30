// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RaceStatics.h"
#include "RacePlayerState.generated.h"



class ACheckpoint;


/**
 *	Simplified Time Date Structure
 */
USTRUCT(BlueprintType)
struct FCheckpointScore
{
	GENERATED_BODY()
protected:
	UPROPERTY()
		ACheckpoint * Checkpoint;

	UPROPERTY()
		FTimespan Time;

public :
	
	FCheckpointScore(ACheckpoint * NewCheckpoint = nullptr, FTimespan NewTime = FTimespan()) : Checkpoint(NewCheckpoint), Time(NewTime){}

	void GetCheckpointScore(ACheckpoint * &GetCheckpoint, FTimespan &GetTime) const	{ GetCheckpoint = Checkpoint; GetTime = Time; }

	bool IsSameCheckpoint(ACheckpoint * GetCheckpoint) const { return  GetCheckpoint == Checkpoint; }

	inline bool operator==(const  FCheckpointScore rhs) const { return Time == rhs.Time; }
	inline bool operator!=(const  FCheckpointScore rhs) const { return Time != rhs.Time; }
	inline bool operator< (const  FCheckpointScore rhs) const { return Time < rhs.Time;  }
	inline bool operator> (const  FCheckpointScore rhs) const { return Time > rhs.Time;  }
	inline bool operator<=(const  FCheckpointScore rhs) const { return Time <= rhs.Time; }
	inline bool operator>=(const  FCheckpointScore rhs) const { return Time >= rhs.Time; }

	friend URaceStatics;
	friend class ARaceGameMode;
};




/**
 *  Base Player state for the race. Stores your score and other things
 */
UCLASS()
class ARacePlayerState : public APlayerState
{
	GENERATED_BODY()


private :

	virtual void ClientInitialize(AController* C) override;

	virtual  void BeginPlay() override;

protected:

	UPROPERTY(BlueprintReadOnly, Replicated)
		TArray<FCheckpointScore> PassedCheckpoints;

	UPROPERTY(BlueprintReadOnly, Replicated)
		bool bShouldResetAtPit;

public:

	UFUNCTION()
		ACheckpoint * GetLastPassedCheckpoint();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_PassedCheckpoint(const AController * Player, ACheckpoint * PassedCheckpoint);

	UFUNCTION()
		void NotifyPassedCheckpoint() const;

	UFUNCTION()
		void OnPassedCheckpoint(ACheckpoint* PassedCheckpoint);

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool GetShouldRestartAtPit() const { return bShouldResetAtPit; }

private:

	UPROPERTY(Replicated)
	AController * OwningPlayer;

protected:

	UPROPERTY(Replicated, BlueprintReadOnly)
	UTeam * Team;


public:

	UTeam * GetTeam() const { return Team;}

	void SetTeam( UTeam * const NewTeam) { Team = NewTeam; }
	
protected:

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
		TArray<FRaceScore> LapPlayerScores;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
		TArray<FRaceScore> GamePlayerScores;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
		FRaceScore FinalPlayerScore;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
		uint8 Laps;


	UPROPERTY(Replicated)
		uint32 TempDriftTime;

	UPROPERTY(Replicated)
		uint32 TempCharsimaScore;


public :
	UFUNCTION(blueprintPure)
		int GetTempDriftTime() const { return TempDriftTime; }

private:
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_UpdateTempDriftScore();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_CalculateScore();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Score|Charisma")
		void PlayerCarAvoidedCollision();



	friend ARaceGameMode;

	
};
