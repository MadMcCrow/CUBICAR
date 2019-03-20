// Fill out your copyright notice in the Description page of Project Settings.

#include "CUBICARGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UserWidget.h"
#include "GameMapsSettings.h"
#include "CUBICARPhysicsReplication.h"
#include "WidgetBlueprintLibrary.h"
//#include "UnrealNetwork.h"
#include "Online.h"
#include "PleaseWaitMessageWidget.h"
//#include "Menu/PleaseWaitMessageWidget.h"


UCUBICARGameInstance::UCUBICARGameInstance(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	/** Bind function for CREATING a Session */
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UCUBICARGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UCUBICARGameInstance::OnStartOnlineGameComplete);

	/** Bind function for FINDING a Session */
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UCUBICARGameInstance::OnFindSessionsComplete);

	/** Bind function for JOINING a Session */
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UCUBICARGameInstance::OnJoinSessionComplete);

	const UGameMapsSettings* GameMapsSettings = GetDefault<UGameMapsSettings>();
	const FString& DefaultMap = GameMapsSettings->GetGameDefaultMap();

	PlayerHostRequestBinding.AddDynamic(this, &UCUBICARGameInstance::HostGame);
	PlayerHostRequestBinding.AddDynamic(this, &UCUBICARGameInstance::CreatePleaseWait);
	PlayerSearchRequestBinding.AddDynamic(this, &UCUBICARGameInstance::FindGame);
	PlayerSearchRequestBinding.AddDynamic(this, &UCUBICARGameInstance::CreatePleaseWait);
	PlayerJoinRequestBinding.AddDynamic(this, &UCUBICARGameInstance::JoinGame);
	PlayerJoinRequestBinding.AddDynamic(this, &UCUBICARGameInstance::CreatePleaseWait);
}

void UCUBICARGameInstance::Init()
{
	Super::Init();
	FPhysScene::PhysicsReplicationFactory = MakeShareable(new ICUBICARPhysicsReplicationFactory());
}

void UCUBICARGameInstance::Shutdown()
{
	DestroySessionAndLeaveGame();
	Super::Shutdown();
}


void UCUBICARGameInstance::QuitGame()
{
#if UE_EDITOR
	UKismetSystemLibrary::QuitGame(GetWorld(), GetPrimaryPlayerController(), EQuitPreference::Type::Quit);
	return;
#endif // UE_EDITOR
	UKismetSystemLibrary::QuitGame(GetWorld(), GetPrimaryPlayerController(), EQuitPreference::Type::Quit);
	//FGenericPlatformMisc::RequestExit(false);
}

// ------------------------ Network exposed function

void UCUBICARGameInstance::HostGame_Implementation()
{
	// Creating a local player where we can get the UserID from
	ULocalPlayer* const Player = GetFirstGamePlayer();
	// ShowLoadingScreen();
	// Let's make sure the name is right before trying to get to Lobby
	FName LobbyMapName = UCUBICARStatics::GetLobbyMapName();
	// Call our custom HostSession function. GameSessionName is a GameInstance variable
	//HostSession(Player->GetCachedUniqueNetId().GetUniqueNetId(), GameSessionName, true, true, HostSettings.MaxPlayers);
	HostSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), RequestSettings.ServerName, true, true, RequestSettings.MaxPlayers);


}


void UCUBICARGameInstance::FindGame_Implementation()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();
	FindSessions(Player->GetPreferredUniqueNetId().GetUniqueNetId(), bIsSearchOnLan, true);
}

void UCUBICARGameInstance::RequestHostGame(const FServerSettings & HostSettings )
{
	RequestSettings = HostSettings;
	// For now just host with default Options	
	PlayerHostRequestBinding.Broadcast();
}



void UCUBICARGameInstance::LaunchLobby(const FServerSettings &ServerSettings)
{
	auto UI  = FWidgetsContainerCUBICAR();
	bool T;
	UUserInterfaceStatics::ShowLoadingScreen(this, UUserInterfaceStatics::GetCUBICARWidgetClasses(T), UI);
	// Create Session
	ULocalPlayer* const Player = GetFirstGamePlayer();
	HostSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), ServerSettings.ServerName, true, true, ServerSettings.MaxPlayers);

}

void UCUBICARGameInstance::RequestFindGame(bool bIsLan)
{
	bIsSearchOnLan = bIsLan;
	PlayerSearchRequestBinding.Broadcast();


}

void UCUBICARGameInstance::RequestJoinGame(const FServerStatus& SessionToJoin)
{
	RequestSession = SessionToJoin;
	ULocalPlayer* const Player = GetFirstGamePlayer();
	RequestSession.UserId = Player->GetPreferredUniqueNetId().GetUniqueNetId();
	PlayerJoinRequestBinding.Broadcast();
}

void UCUBICARGameInstance::JoinGame_Implementation()
{
	JoinOnlineSession(RequestSession.UserId, FName(*RequestSession.SessionName), *RequestSession.SessionSearchResult);
}

void UCUBICARGameInstance::DestroySessionAndLeaveGame()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		//IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		IOnlineSessionPtr Sessions = Online::GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			Sessions->DestroySession(GameSessionName);
		}
	}
}


void UCUBICARGameInstance::ReturnToMainMenu()
{
	Super::ReturnToMainMenu();
	const UGameMapsSettings* GameMapsSettings = GetDefault<UGameMapsSettings>();

	const auto DefaultMapName = UCUBICARStatics::GetDefaultMapName();
	const auto MainMenuName = UCUBICARStatics::GetMainMenuMapName();
	if (DefaultMapName != MainMenuName) // Sanity check
	{
		UE_LOG(LogLoad, Warning, TEXT("DefaultMap (%s) is not the same as the Specified MainMenu Map (%s)"), *DefaultMapName.ToString(), *MainMenuName.ToString());
	}
	if (MainMenuName != FName())
		UGameplayStatics::OpenLevel(GetWorld(), MainMenuName, true, FString());

	//ShowMainMenu();
}

void UCUBICARGameInstance::CreatePleaseWait()
{
	auto Widgets = UUserInterfaceStatics::FromCurrentWidgets(this, false);
	bool r = false;
	UE_LOG(LogTemp, Display, TEXT("Please Wait"));
	const auto Classes = UUserInterfaceStatics::GetCUBICARWidgetClasses(r);
	auto PleaseWait = UUserInterfaceStatics::ShowPleaseWaitMessage(this, Classes, Widgets);
	UPleaseWaitMessageWidget * PWWidget = Cast<UPleaseWaitMessageWidget>(PleaseWait);
	if (PWWidget)
	{
			EndPleaseWait.Clear();
			EndPleaseWait.AddDynamic(PWWidget, &UPleaseWaitMessageWidget::EndPleaseWait);
	}
}

///
///	Backend Function for connecting
///
bool UCUBICARGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	UE_LOG(LogNet, Display, TEXT("Host Session Begin"));
	// Get the Online Subsystem to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();


	if (OnlineSub)
	{
		// Get the Session Interface, so we can call the "CreateSession" function on it
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			/*
			Fill in all the Session Settings that we want to use.
			There are more with SessionSettings.Set(...);
			For example the Map or the GameMode/Type.
			*/

			SessionSettings = MakeShareable(new FOnlineSessionSettings());


			// Todo : Move those into the function parameters
			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SETTING_MAPNAME, UCUBICARStatics::GetLobbyMapName().ToString(), EOnlineDataAdvertisementType::ViaOnlineService);

			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Let's make a Copy for later use, if we need to;
			//NetSettings.SessionSettings = SessionSettings;

			UE_LOG(LogNet, Display, TEXT("Creating Session"));
			// Our delegate should get called when this is complete (doesn't need to be successful!)
			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
		if (!Sessions.IsValid())
		{
			UE_LOG(LogNet, Error, TEXT("Session invalid"));
		}
		else
		{
			UE_LOG(LogNet, Error, TEXT("UserID invalid"));
		}
		EndPleaseWait.Broadcast();
		DestroySessionAndLeaveGame();
	}
	else
	{
		UE_LOG(LogNet, Error, TEXT("No OnlineSubsytem found!"));
		EndPleaseWait.Broadcast();
	}
	return false;
}

void UCUBICARGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful) {
		UE_LOG(LogNet, Display, TEXT("OnCreateSessionComplete %s Succesful"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogNet, Error, TEXT("OnCreateSessionComplete %s failed"), *SessionName.ToString());
	}
	
	// Get the OnlineSubsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to call the StartSession function
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the SessionComplete delegate handle, since we finished this call
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				// Set the StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				// Our StartSessionComplete delegate should get called after this
				Sessions->StartSession(SessionName);
			}
		}
	}
	EndPleaseWait.Broadcast();
}

void UCUBICARGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful) {
		UE_LOG(LogNet, Display, TEXT("OnStartSessionComplete %s Succesful"), *SessionName.ToString());
	}
	else
		UE_LOG(LogNet, Error, TEXT("OnStartSessionComplete %s failed"), *SessionName.ToString());

	// Get the Online Subsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to clear the Delegate
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), UCUBICARStatics::GetLobbyMapName(), true, "listen");
	}

}

// Find Session ----------------------------------------------------------------------------------------------

void UCUBICARGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get the SessionInterface from our OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			/*
			Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
			*/
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;

			// We only want to set this Query Setting if "bIsPresence" is true
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			// Set the Delegate to the Delegate Handle of the FindSession function
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			// Finally call the SessionInterface function. The Delegate gets called once this is finished
			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
	}
	else
	{
		// If something goes wrong, just call the Delegate Function directly with "false".
		OnFindSessionsComplete(false);
	}
}

void UCUBICARGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	EndPleaseWait.Broadcast();

	if (bWasSuccessful) {
		UE_LOG(LogNet, Display, TEXT("OnFindSessionsComplete: Find SuccessFul"));
	}
	else
		UE_LOG(LogNet, Error, TEXT("OnFindSessionsComplete: Find failed"));

	// Session Search results
	FSessionSearchResult Result;


	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface of the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the Delegate handle, since we finished this call
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			// Just debugging the Number of Search results. Can be displayed in UMG or something later on
			UE_LOG(LogNet, Display, TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num());

			// Save the number of search result
			const uint8 SessionResultNumbers = SessionSearch->SearchResults.Num();

			// If we have found at least 1 session, we just going to debug them.
			if (SessionResultNumbers > 0)
			{
			Result.bSearchIsSuccesful = true;
			// Save the result
			Result.SearchResults = SessionResultNumbers;

				// "SessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
				// This can be customized later on with your own classes to add more information that can be set and displayed
				for (int32 SearchIdx = 0; SearchIdx < SessionResultNumbers; SearchIdx++)
				{
					// OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
					// This is something you can't do in Blueprint for example!
					UE_LOG(LogNet, Display, TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName));
					FServerStatus SessionFound(SessionSearch->SearchResults[SearchIdx]);

					//SessionFound.UnrealSession = SessionSearch->SearchResults[SearchIdx].Session;
					Result.Sessions.Add(SessionFound);
				}
			}
		}
	}
	NotifyFoundSessions.Broadcast(Result);
}

bool UCUBICARGameInstance::JoinOnlineSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult & SearchResult)
{
	// Return bool
	bool bSuccessful = false;

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			// Set the Handle again
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			// Call the "JoinSession" Function with the passed "SearchResult". The "SessionSearch->SearchResults" can be used to get such a
			// "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}
	UE_LOG(LogNet, Display, TEXT("JoinOnlineSession, success : %s"), bSuccessful ? TEXT("true") : TEXT("false"));
	return bSuccessful;
}

void UCUBICARGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{

	UE_LOG(LogNet, Warning, TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate again
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			// Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
			// This is something the Blueprint Node "Join Session" does automatically!
			APlayerController * const PlayerController = GetFirstLocalPlayerController();

			// We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
			// String for us by giving him the SessionName and an empty String. We want to do this, because
			// Every OnlineSubsystem uses different TravelURLs
			FString TravelURL;

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				// Finally call the ClienTravel. If you want, you could print the TravelURL to see
				// how it really looks like
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UCUBICARGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful) {
		UE_LOG(LogNet, Display, TEXT("OnDestroySessionComplete %s Succesful"), *SessionName.ToString());
	}
	else
		UE_LOG(LogNet, Error, TEXT("OnDestroySessionComplete %s failed"), *SessionName.ToString());

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			// If it was successful, we just load another level (could be a MainMenu!)
			if (bWasSuccessful)
			{
				ReturnToMainMenu();
				//UGameplayStatics::OpenLevel(GetWorld(), UCUBICARStatics::GetMainMenuMapName(), true);
			}
		}
	}
}
