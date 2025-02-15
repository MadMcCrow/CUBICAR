// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CUBICARBaseGameMode.generated.h"

class ACheckpoint;


UCLASS(MinimalAPI)
class ACUBICARBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACUBICARBaseGameMode();

	/** Called At Start of Game **/
	virtual void StartPlay() override;


private:

	/** Find All Checkpoints and updates Stored Array **/
	void UpdatesCheckpointList();

	UPROPERTY()
	TArray<ACheckpoint *> MapCheckpoints;
};



