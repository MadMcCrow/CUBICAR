// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Team.generated.h"

class UTeam;
class URaceStatics;
class UTexture2D;


USTRUCT(BlueprintType)
struct FTeamData : public FTableRowBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName TeamName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 TeamID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor TeamColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D * TeamLogo;

public:

	uint8 GetID() const { return TeamID; }
	FName GetName() const { return TeamName; }
	FColor GetColor() const { return TeamColor; }
	UTexture2D const * GetLogo() const { return TeamLogo; }

private:
	friend URaceStatics;
	friend UTeam;

};

UCLASS( ClassGroup=(GameState), meta=(BlueprintSpawnableComponent) )
class RACE_API UTeam : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTeam();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:

	UPROPERTY(Replicated)
		FTeamData Info;

	UPROPERTY(Replicated)
		uint32 Money;



public:

	UFUNCTION(BlueprintPure, Category = "Team")
		FTeamData GetTeamInfo() const { return Info; }

	UFUNCTION(BlueprintPure, Category = "Team")
		uint8 GetID() const { return Info.TeamID; }

	UFUNCTION(BlueprintCallable, Category = "Purchase")
		bool BuyItem(const UObject * ItemToPurchase);

private :
	UPROPERTY(Replicated)
		TArray<UObject *> PurchasedItems;


public:

	UFUNCTION(BlueprintPure , Category = "Purchase")
		FORCEINLINE TArray<UObject *> GetPurchasedItems() const { return PurchasedItems; }


private:

	friend URaceStatics;
	
	
	
};
