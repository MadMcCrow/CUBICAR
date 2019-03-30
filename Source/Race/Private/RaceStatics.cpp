// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceStatics.h"
#include "RaceGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "RacePlayerState.h"
#include "RaceGameState.h"
#include "RaceCar.h"
#include "Parts/PartStatics.h"


template <typename T>
bool URaceStatics::ValidateDataTable(const UDataTable* Table)
{
	const FString ContextString;
	TArray<T*> StaticTable;
	if (!Table)
		return false;
	Table->GetAllRows(ContextString, StaticTable);

	if (!StaticTable.IsValidIndex(0))
		return false;
	return StaticTable[0] != nullptr;
	return false;
}

template <typename T>
TArray<T> URaceStatics::GetStaticData(const UDataTable* Table)
{
	TArray<T> StaticData;
	if (!ValidateDataTable<T>(Table))
		return StaticData;

	const FString ContextString;
	const TArray<FName> RowNames = Table->GetRowNames();
	for (auto& name : RowNames)
	{
		T* row = Table->FindRow<T>(name, ContextString);
		if (row)
		{
			StaticData.Add(*row);
		}
	}
	return StaticData;
}


bool URaceStatics::ValidateTeamDataTable(const UDataTable * Table)
{
	return ValidateDataTable<FTeamData>(Table);
}


TArray<FTeamData> URaceStatics::GetStaticTeamData(const UDataTable * Table)
{
	return GetStaticData<FTeamData>(Table);
}

ARaceGameMode * URaceStatics::GetRaceGameMode( const UObject* WorldContextObject)
{
	return Cast<ARaceGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
}

ARaceGameState* URaceStatics::GetRaceGameState(const UObject* WorldContextObject)
{
	return Cast<ARaceGameState>(UGameplayStatics::GetGameState(WorldContextObject));
}

ARacePlayerState* URaceStatics::GetRacePlayerState(const UObject* WorldContextObject, const AController* Owner)
{
	if (!Owner)
		return nullptr;
	const auto PS = Owner->PlayerState;
	if (!PS)
		return nullptr;
	return  Cast<ARacePlayerState>(PS);
}

UTeam *URaceStatics::GetTeamByID(const UObject* WorldContextObject, uint8 id, bool &IsValid)
{
	IsValid = false;
	const auto  T = GetRaceGameState(WorldContextObject)->GetTeamWithID(id);
	if (T)
		IsValid = true;
	return T;

}

ARaceCar* URaceStatics::GetControllersCar(AController* Controller)
{
		if (Controller)
			return Cast<ARaceCar>(Controller->GetPawn());
		return nullptr;
}
