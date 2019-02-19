// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CUBICARPlayerState.generated.h"

class ACheckpoint;
/**
 * 
 */
UCLASS()
class CUBICAR_API ACUBICARPlayerState : public APlayerState
{
	GENERATED_BODY()

private:
	UPROPERTY()
		ACheckpoint * LastCheckpointPassed;

public:
	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
		void SetLastPassedCheckpoint(ACheckpoint* NewlyPassedCheckpoint);
	
	
	
	
};
