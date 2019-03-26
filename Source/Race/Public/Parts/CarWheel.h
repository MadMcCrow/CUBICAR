// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartStatics.h"
#include "CarWheel.generated.h"



USTRUCT(BlueprintType)
struct FWheelPart : public FPurchasableItem
{
	GENERATED_BODY()
protected:

	UPROPERTY()
		UStaticMesh * Mesh;


public:

	UStaticMesh * GetMesh() const { return Mesh; }
};
