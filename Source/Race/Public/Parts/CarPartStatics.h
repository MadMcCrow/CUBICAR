// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
#include "PurchaseInterface.h"
#include "UpgradeInterface.h"
#include "RaceCarWheel.generated.h"


class ARaceCar;
class UStaticMesh;


/**
 *	Wheel Upgrade Base Class 
 */
UCLASS()
class RACE_API URaceCarWheel : public UObject, public IPurchaseInterface, public IUpgradeInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMesh * VisibleMesh;

	// UpgradeInterface
	virtual void ApplyToCar_Implementation(const ARaceCar * Car) override;

	
};
