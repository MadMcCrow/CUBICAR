// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CarPartDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FRaceCarPartDataStruct
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common", meta = (DisplayName = "Part Name"))
		FName PartName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common", meta = (DisplayName = "Part Cost"))
		int Cost;
};


/**
 * 
 */
UCLASS()
class URaceCarPartDataAsset : public UDataAsset
{
	GENERATED_BODY()


	
	
	
};
