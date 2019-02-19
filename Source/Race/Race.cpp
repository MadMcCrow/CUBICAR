// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#include "Race.h"

DEFINE_LOG_CATEGORY(LogRace);

#define LOCTEXT_NAMESPACE "FRace"

void FRace::StartupModule()
{
	UE_LOG(LogRace, Warning, TEXT("Race module has started!"));
}

void FRace::ShutdownModule()
{
	UE_LOG(LogRace, Warning, TEXT("Race module has shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRace, Race)