// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Team.h"
#include "RaceStatics.generated.h"


class AController;
class UDataTable;
class ARaceGameState;
class ARaceGameMode;
class UTeam;
class ARacePlayerState;



/**
 * 
 */
UCLASS()
class RACE_API URaceStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public :
	/** Template version for different datatables*/
	template <typename  T>
	static bool ValidateDataTable(const UDataTable* Table);

	UFUNCTION(BlueprintPure, Category = "Team")
	static bool ValidateTeamDataTable(const UDataTable* Table);

	template <typename  T>
		static TArray<T> GetStaticData(const UDataTable* Table);

	UFUNCTION(BlueprintPure, Category = "Team")
		static TArray<FTeamData> GetStaticTeamData(const UDataTable* Table);

	UFUNCTION(BlueprintPure, Category = "Race", meta = (WorldContext = "WorldContextObject"))
		static ARaceGameMode* GetRaceGameMode(const UObject* WorldContextObject);


	UFUNCTION(BlueprintPure, Category = "Race", meta = (WorldContext = "WorldContextObject"))
		static ARaceGameState* GetRaceGameState(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Race", meta = (WorldContext = "WorldContextObject"))
		static ARacePlayerState* GetRacePlayerState(const UObject* WorldContextObject, const AController * Owner );

	UFUNCTION(BlueprintPure, Category = "Race", meta = (WorldContext = "WorldContextObject"))
		static UTeam * GetTeamByID(const UObject* WorldContextObject, uint8 id, bool &IsValid);

	
};

