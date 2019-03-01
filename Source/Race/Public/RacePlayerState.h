// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
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
		FTimespan Time;

public :
	
	FCheckpointScore(ACheckpoint * NewCheckpoint = nullptr, FTimespan NewTime = FTimespan()) : Checkpoint(NewCheckpoint), Time(NewTime){}

	void GetCheckpointScore(ACheckpoint * &GetCheckpoint, FTimespan &GetTime) const	{ GetCheckpoint = Checkpoint; GetTime = Time; }

	inline bool operator==(const  FCheckpointScore rhs) const { return Time == rhs.Time; }
	inline bool operator!=(const  FCheckpointScore rhs) const { return Time != rhs.Time; }
	inline bool operator< (const  FCheckpointScore rhs) const { return Time < rhs.Time;  }
	inline bool operator> (const  FCheckpointScore rhs) const { return Time > rhs.Time;  }
	inline bool operator<=(const  FCheckpointScore rhs) const { return Time <= rhs.Time; }
	inline bool operator>=(const  FCheckpointScore rhs) const { return Time >= rhs.Time; }
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

protected:

	UPROPERTY()
		TArray<FCheckpointScore> PassedCheckpoints;

public:

	UFUNCTION()
		ACheckpoint * GetLastPassedCheckpoint();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_PassedCheckpoint(const AController * Player, ACheckpoint * PassedCheckpoint);

	UFUNCTION()
		void NotifyPassedCheckpoint() const;

	UFUNCTION()
		void OnPassedCheckpoint(ACheckpoint* PassedCheckpoint);

private:

	AController * OwningPlayer;

	
};
