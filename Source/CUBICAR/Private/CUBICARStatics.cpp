// Fill out your copyright notice in the Description page of Project Settings.

#include "CUBICARStatics.h"
#include "Engine/Engine.h"
#include "CUBICARGameInstance.h"
#include "UserInterfaceStatics.h"
#include "GameMapsSettings.h"

UCUBICARGameInstance * UCUBICARStatics::GetCUBICARGameInstance(const UObject * WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	return World ? Cast<UCUBICARGameInstance>(World->GetGameInstance()) : nullptr;
}

UCUBICARSingleton* UCUBICARStatics::GetCUBICARData(bool& DataIsValid)
{
	DataIsValid = false;
	UCUBICARSingleton* DataInstance = Cast<UCUBICARSingleton>(GEngine->GameSingleton);

	if (!DataInstance) return NULL;
	if (!DataInstance->IsValidLowLevel()) return NULL;

	DataIsValid = true;
	return DataInstance;
}


UUserWidget* UCUBICARStatics::AskToQuit(const UObject* WorldContextObject)
{
	const auto GI = GetCUBICARGameInstance(WorldContextObject);
	if (!GI)
		return nullptr;
	auto Container = FWidgetsContainerCUBICAR();
	bool Result;

	return UUserInterfaceStatics::ShowQuitGameConfirmationMessage(WorldContextObject, UUserInterfaceStatics::GetCUBICARWidgetClasses(Result), Container);
}

void UCUBICARStatics::Quit(const UObject* WorldContextObject)
{
	const auto GI = GetCUBICARGameInstance(WorldContextObject);
	if (!GI)
		return;
	GI->QuitGame();
}

void UCUBICARStatics::HostGame(const UObject* WorldContextObject, const FServerSettings &HostSettings)
{
	const auto GI = GetCUBICARGameInstance(WorldContextObject);
	if (!GI)
		return;
	GI->RequestHostGame(HostSettings);
}

void UCUBICARStatics::JoinGame(const UObject* WorldContextObject, const FServerStatus& HostSettings)
{
	const auto GI = GetCUBICARGameInstance(WorldContextObject);
	if (!GI)
		return;
	GI->RequestJoinGame(HostSettings);
}

void UCUBICARStatics::FindGames(const UObject* WorldContextObject, const FServerSettings& HostSettings)
{
	const auto GI = GetCUBICARGameInstance(WorldContextObject);
	if (!GI)
		return;
	GI->RequestFindGame(HostSettings.IsLan());
}


FServerSettings UCUBICARStatics::CreateSettings(int MaxPlayers,
	const EOnlineSSModule SubSystemtoUse, FName ServerName)
{
	return FServerSettings(MaxPlayers, SubSystemtoUse, ServerName);
}



 FName UCUBICARStatics::GetMainMenuMapName()
{
	 bool T;
	 return GetMapName(GetCUBICARData(T)->MainMenuUMap);
}


 FName UCUBICARStatics::GetDefaultMapName()
{
	const UGameMapsSettings* GameMapsSettings = GetDefault<UGameMapsSettings>();
	const FString& DefaultMap = GameMapsSettings->GetGameDefaultMap();
	return FName(*DefaultMap);
}


 FName UCUBICARStatics::GetLobbyMapName()
{
	 bool T;
	 return GetMapName(GetCUBICARData(T)->LobbyUMap);
}
 
 FName UCUBICARStatics::GetMapName(TSoftObjectPtr<UWorld> MapAsset)
{
	return *MapAsset.GetLongPackageName();
}

FName UCUBICARStatics::GetOnlineModuleName(EOnlineSSModule ModuleEnum)
{
	switch (ModuleEnum) { 
	case EOnlineSSModule::OSSM_Lan:		return TEXT("Lan");
	case EOnlineSSModule::OSSM_Steam:	return TEXT("Steam");
	}
	return FName(); // Just a safety measure;
}

EOnlineSSModule UCUBICARStatics::GetOnlineModuleByName(FName ModuleName)
{
	// Cannot use switch on FName
	if(ModuleName == TEXT("Lan"))
		return EOnlineSSModule::OSSM_Lan;
	if (ModuleName == TEXT("Steam"))
		return EOnlineSSModule::OSSM_Steam;
	//Default case
	return EOnlineSSModule::OSSM_Lan;
}

EOnlineSSModule UCUBICARStatics::GetOnlineModuleByIndex(int Index)
{
	switch (Index) 
	{
	case 0:		return EOnlineSSModule::OSSM_Lan;
	case 1:		return EOnlineSSModule::OSSM_Steam;
	default:	return EOnlineSSModule::OSSM_Lan;
	}
}
