// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint.h"
#include "LapStart.generated.h"

class APitLanePosition;

/**
 * 
 */
UCLASS()
class RACE_API ALapStart : public ACheckpoint
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pit")
	TArray<APitLanePosition *> PitPositions;
	
	public:
	
	UFUNCTION(BlueprintPure, Category= "Pit")
	FORCEINLINE APitLanePosition* GetPolePosition(){return PitPositions.IsValidIndex(0) ? PitPositions[0] : nullptr;}

	
};
