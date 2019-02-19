// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RaceTime.h"
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




private:

	/**	Offset compared to the Hour - Correspond to the difference we have to do with FRaceTime::Now	*/
	UPROPERTY()//Replicated)
		FRaceTime RaceStartOffset;

	UFUNCTION()
		FRaceTime TimeElapsedSinceMatchStart();


};
