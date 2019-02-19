// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint.h"
#include "StartPoint.generated.h"

/**
 *
 */
UCLASS()
class AStartPoint : public ACheckpoint
{
	GENERATED_BODY()

protected:
	void CarMadeLoop(const APawn* Car);


	
	
	
	
};
