// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PartStatics.generated.h"

class UPartStatics;

USTRUCT(BlueprintType)
struct FPurchasableItem : public FTableRowBase
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 Price;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		uint8 RarityLevel;

public:
	FPurchasableItem(int32 NewPrice = 0, uint8 Rarity = 0) : Price(NewPrice), RarityLevel(Rarity)
	{
		Price = FMath::Abs(Price);  // Objects aren't less than free
	}
private:

	friend UPartStatics;
};



UCLASS()
class RACE_API UPartStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public :
	//UFUNCTION(BlueprintPure, Category = "Purchase")
		//static TArray<FPurchasableItem> GetStaticPurchaseData(const UDataTable* Table);
	
};

