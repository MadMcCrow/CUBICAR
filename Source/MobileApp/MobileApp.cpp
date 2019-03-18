// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#include "MobileApp.h"

DEFINE_LOG_CATEGORY(LogMobileApp);

#define LOCTEXT_NAMESPACE "FMobileApp"

void FMobileApp::StartupModule()
{
	UE_LOG(LogMobileApp, Warning, TEXT("MobileApp module has started!"));
}

void FMobileApp::ShutdownModule()
{
	UE_LOG(LogMobileApp, Warning, TEXT("MobileApp module has shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMobileApp, MobileApp)