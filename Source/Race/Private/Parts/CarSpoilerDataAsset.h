// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CarPartDataAsset.h"
#include "CarSpoilerDataAsset.generated.h"

class UStaticMesh;

USTRUCT()
struct FRaceCarSpoilerDataStruct : public FRaceCarPartDataStruct
{
	GENERATED_BODY()

	/**	The more you add the more traction the car gains	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spoiler", meta = (DisplayName = "Down Force") )
		float TireFrictionOffset;

	/**	TheMesh To Use	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spoiler",  meta = (DisplayName = "SpoilerDownForce"))
		UStaticMesh * SpoilerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spoiler", meta = (DisplayName = "Style Multiplayer"))
		float StyleMultiplayer;

};


/**
 * 
 */
UCLASS()
class URaceCarSpoilerDataAsset : public URaceCarPartDataAsset
{
	GENERATED_BODY()
	
	
	
	
};
