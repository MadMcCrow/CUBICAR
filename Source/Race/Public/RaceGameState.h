// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RaceGameState.generated.h"
/**
 * 
 */
UCLASS()
class ARaceGameState : public AGameState
{
	GENERATED_BODY()

public:

	virtual void HandleMatchHasStarted() override;

	UFUNCTION()
		FTimespan RaceTime() const;


private:

	/**	Offset compared to the begin of the race since- Correspond to the difference to FTimespan::GetTimeOfDay() */
	UPROPERTY()//Replicated)
		FTimespan RaceStartOffset;

	


};
