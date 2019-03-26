// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CarComponent.h"
#include "PartStatics.h"
#include "CarEngine.generated.h"

USTRUCT(BlueprintType)
struct FEnginePart : public FPurchasableItem
{
	GENERATED_BODY()
protected:

	UPROPERTY()
		UStaticMesh * Mesh;


public :

	UStaticMesh * GetMesh() const { return Mesh; }

};

/**
 *	Car Engine Upgrade Base Class 
 */
UCLASS()
class UCarEngine : public UCarComponent
{
	GENERATED_BODY()
	
	
	
	
};
