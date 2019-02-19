// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RaceTime.h"
#include "RacePlayerState.generated.h"


class ACheckpoint;


/**
 *	Simplified Time Date Structure
 */
USTRUCT()
struct FCheckpointScore
{
	GENERATED_BODY()
protected:
	UPROPERTY()
		ACheckpoint * Checkpoint;

	UPROPERTY()
		FRaceTime Time;

public :
	
	FCheckpointScore(ACheckpoint * NewCheckpoint = nullptr, FRaceTime NewTime = FRaceTime()) : Checkpoint(NewCheckpoint), Time(NewTime){}

	void GetCheckpointScore(ACheckpoint * &GetCheckpoint, FRaceTime &GetTime) const	{ GetCheckpoint = Checkpoint; GetTime = Time; }

	inline bool operator==(const  FCheckpointScore rhs) const { return Time.GetRealTime() == rhs.Time.GetRealTime(); }
	inline bool operator!=(const  FCheckpointScore rhs) const { return Time.GetRealTime() != rhs.Time.GetRealTime(); }
	inline bool operator< (const  FCheckpointScore rhs) const { return Time.GetRealTime() < rhs.Time.GetRealTime();  }
	inline bool operator> (const  FCheckpointScore rhs) const { return Time.GetRealTime() > rhs.Time.GetRealTime();  }
	inline bool operator<=(const  FCheckpointScore rhs) const { return Time.GetRealTime() <= rhs.Time.GetRealTime(); }
	inline bool operator>=(const  FCheckpointScore rhs) const { return Time.GetRealTime() >= rhs.Time.GetRealTime(); }
};




/**
 *  Base Player state for the race. Stores your score and other things
 */
UCLASS()
class ARacePlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY()
		TArray<FCheckpointScore> PassedCheckpoints;

public:

	UFUNCTION()
		ACheckpoint * GetLastPassedCheckpoint();

	UFUNCTION(Server, Reliable, WithValidation)
		void PassedCheckpoint(const APlayerController * Player, ACheckpoint * PassedCheckpoint);


	
};
