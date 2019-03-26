// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceGameState.h"
#include "TimerManager.h"


UTeam* ARaceGameState::GetTeamWithID(uint8 id)
{
	UTeam**  t =  Teams.Find(id);
	 if (*t)
		 return *t;
	 return nullptr;
}

void ARaceGameState::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	RaceStartOffset = FDateTime::Now().GetTimeOfDay();

}

FTimespan ARaceGameState::RaceTime() const
{
	return FDateTime::Now().GetTimeOfDay() - RaceStartOffset;
}

void ARaceGameState::SetTeams(const TArray<FTeamData> NewTeams)
{
	if (Role != ROLE_Authority) // this function should only be executed on server
		return;

	for (auto it : NewTeams)
	{
		FString TeamCompName = TEXT("TeamComp");
		TeamCompName.Append(it.GetName().ToString());
		TeamCompName.AppendInt(it.GetID());

		const uint8 NewID = it.GetID();
		//CompClass can be a BP
		UTeam* NewComp = NewObject<UTeam>(this, UTeam::StaticClass(),  FName(*TeamCompName));
		if (!NewComp)
		{
			return;
		}

		NewComp->RegisterComponent();        //You must ConstructObject with a valid Outer that has world, see above	 

		Teams.Add(NewID, NewComp);
	}
}


