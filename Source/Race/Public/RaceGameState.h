// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RaceStatics.h"
#include "RaceGameState.generated.h"

/**
 * 
 */
UCLASS()
class ARaceGameState : public AGameState
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<uint8, UTeam*> Teams;

public:

	UFUNCTION(BlueprintPure)
		UTeam* GetTeamWithID(uint8 id);

	virtual void HandleMatchHasStarted() override;

	UFUNCTION()
		FTimespan RaceTime() const;

	UFUNCTION()
		void SetTeams(const TArray<FTeamData> NewTeams);


private:

	/**	Offset compared to the begin of the race since- Correspond to the difference to FTimespan::GetTimeOfDay() */
	UPROPERTY()//Replicated)
		FTimespan RaceStartOffset;

	


};
