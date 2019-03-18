// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UnrealNetwork.h"
#include "Online.h"
#include "StreamingPauseRendering.h"
#include "CUBICARStatics.h"
#include "UserInterfaceStatics.h"
#include "CUBICARGameInstance.generated.h"



class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHostRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSearchRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJoinRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFoundSessions, const FSessionSearchResult &, SearchResults);


/**
 *	@class UCUBICARGameInstance 
 *	handles host and join request from player 
 *	
 */
UCLASS(Config = Game)
class CUBICAR_API UCUBICARGameInstance : public UGameInstance
{
	GENERATED_BODY()
public :

	UCUBICARGameInstance(const FObjectInitializer& ObjectInitializer);



protected:

	// Host Session				-----------------------------------------------------------------------------------------------------------------------

	/**
	*	Function to host a game!
	*
	*	@Param		UserID			User that started the request
	*	@Param		SessionName		Name of the Session
	*	@Param		bIsLAN			Is this is LAN Game?
	*	@Param		bIsPresence		"Is the Session to create a presence Session"
	*	@Param		MaxNumPlayers	        Number of Maximum allowed players on this "Session" (Server)
	*/
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);


	/* Delegate called when session created */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/* Delegate called when session started */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/** Handles to registered delegates for creating/starting a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	/**
*	Function fired when a session create request has completed
*
*	@param SessionName the name of the session this callback is for
*	@param bWasSuccessful true if the async action completed without error, false if there was an error
*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	Function fired when a session start request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);


	// Search and find Session	-----------------------------------------------------------------------------------------------------------------------


	/**
	*	Find an online session
	*
	*	@param UserId user that initiated the request
	*	@param bIsLAN are we searching LAN matches
	*	@param bIsPresence are we searching presence sessions
	*/
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

	/** Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	/** Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	/**
	*	Delegate fired when a session search query has completed
	*
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnFindSessionsComplete(bool bWasSuccessful);


	// Joining a Session		-----------------------------------------------------------------------------------------------------------------------


	/**
	*	Joins a session via a search result
	*
	*	@param SessionName name of session
	*	@param SearchResult Session to join
	*
	*	@return bool true if successful, false otherwise
	*/
	bool JoinOnlineSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);


	/** Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	/** Handle to registered delegate for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;


	/**
	*	Delegate fired when a session join request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);


	// Destroying a Session		-----------------------------------------------------------------------------------------------------------------------	


	/** Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	/** Handle to registered delegate for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;


	/**
	*	Delegate fired when a destroying an online session has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	friend UCUBICARStatics;

	public:

	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION()
		void QuitGame();

	UFUNCTION()
		void HostGame();

	UFUNCTION()
		void FindGame();

	UFUNCTION()
		void JoinGame();

	//UPROPERTY(Transient)
	FServerSettings RequestSettings; // for host
	//UPROPERTY(Transient)
	FServerStatus RequestSession; // for join
	//UPROPERTY(Transient)
	bool bIsSearchOnLan;


	
	void RequestHostGame(const FServerSettings & HostSettings);
	
	void LaunchLobby(const FServerSettings &ServerSettings);
	
	void RequestFindGame(bool bIsLan);

	void RequestJoinGame(const FServerStatus &SessionToJoin);
	
	void DestroySessionAndLeaveGame();
	
	void ReturnToMainMenu() override;


	protected:
		FHostRequest	 PlayerHostRequestBinding;
		FSearchRequest	 PlayerSearchRequestBinding;
		FJoinRequest	 PlayerJoinRequestBinding;

	UFUNCTION()
		virtual void CreatePleaseWait();


		FOnEndPleaseWaitEvent EndPleaseWait;

	public:

		FOnFoundSessions NotifyFoundSessions;
};
