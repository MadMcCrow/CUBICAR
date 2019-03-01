// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#include "UserInterface.h"

DEFINE_LOG_CATEGORY(LogUserInterface);

#define LOCTEXT_NAMESPACE "FUserInterface"

void FUserInterface::StartupModule()
{
	UE_LOG(LogUserInterface, Warning, TEXT("Race module has started!"));
}

void FUserInterface::ShutdownModule()
{
	UE_LOG(LogUserInterface, Warning, TEXT("Race module has shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUserInterface, UserInterface)