// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OnlineSessionSettings.h"
#include "CUBICARSingleton.h"
#include "CUBICARStatics.generated.h"



class UCUBICARStatics;
class UCUBICARGameInstance;
class UUserWidget;


UENUM(BlueprintType, meta = (DisplayName = "OnlineSubModule") )		//"BlueprintType" is essential to include
enum class EOnlineSSModule : uint8
{
	OSSM_Lan	UMETA(DisplayName = "Lan"),
	OSSM_Steam 	UMETA(DisplayName = "Steam"),
	// If you wanna add new web services, add them here and in the game instance
	OSSM_MAX = OSSM_Steam // Should Always be last and set to the last one
};


/** 
 *	Struct to store all hosting parameters
 */
USTRUCT(Blueprintable)
struct FServerSettings
{
	GENERATED_BODY()

protected :
	UPROPERTY(EditAnywhere)
		int MaxPlayers;

	UPROPERTY(EditAnywhere)
		EOnlineSSModule OnlineSystem;

	UPROPERTY(EditAnywhere)
		FName ServerName;

public :
	FServerSettings(int NewMaxPlayers = 10, EOnlineSSModule NewOnlineSystem = EOnlineSSModule::OSSM_Lan,
	                const FName& NewServerName = TEXT("ErrorServerNameNotSet")) : MaxPlayers(10),
	                                                                              OnlineSystem(NewOnlineSystem),
	                                                                              ServerName(NewServerName)
	{
	}

	int GetMaxPlayers() { return MaxPlayers; }

	bool IsLan() const 
	{
		if (OnlineSystem == EOnlineSSModule::OSSM_Lan)
			return true;
		return false;
	}

private :
	friend UCUBICARGameInstance;
	friend UCUBICARStatics;
};

/**
 *	Struct to add Server status informations
 */
USTRUCT(Blueprintable)
struct FServerStatus : public FServerSettings
{
	GENERATED_BODY()

public:

	UPROPERTY()
		int32 CurrentPlayers;

	UPROPERTY()
		int32 RegisteredPlayers;

	UPROPERTY()
		int32 CurrentSpectators;

	UPROPERTY()
		bool bIsValid;

	UPROPERTY()
		int16 ping;

private :

	// The actual id we want to use 
	TSharedPtr<const FUniqueNetId> UserId;

	UPROPERTY()
		FString SessionName;

	const FOnlineSessionSearchResult * SessionSearchResult;


public:
	FServerStatus() : FServerSettings()
	{
		SessionSearchResult = nullptr;
		CurrentPlayers = 0;
		RegisteredPlayers = 0;
		CurrentSpectators = 0;
		bIsValid = false;
		SessionName = FString(TEXT("Null"));
	}

	 FServerStatus(const FOnlineSessionSearchResult & SearchResult) : FServerSettings()
	{
		 if (!SearchResult.IsValid())
			 return;

		 SessionSearchResult = &SearchResult;
		 CurrentPlayers = SessionSearchResult->Session.SessionSettings.NumPublicConnections - SessionSearchResult->Session.NumOpenPublicConnections;
		 SessionName = SessionSearchResult->Session.OwningUserName;
		 bIsValid = true;
	}

	int GetMaxPlayers() const { return MaxPlayers; }
	int GetCurrentPlayers() const { return CurrentPlayers; }
	int GetCurrentSpectators() const { return CurrentSpectators; }
	int GetPing()const { return ping; }
	FName GetName() const { return FName(*SessionName); }
	bool IsValid() const { return bIsValid; }

	
private:
	friend UCUBICARGameInstance;
	friend UCUBICARStatics;
};


/**
 *	Struct to add Server status informations
 */
USTRUCT(Blueprintable)
struct FSessionSearchResult
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		bool bSearchIsSuccesful;

	UPROPERTY()
		uint8 SearchResults; //256 possible servers, use uint16 for more (65536 possible servers, that is quite future proof)

	UPROPERTY()
		TArray<FServerStatus> Sessions;

public:

	bool IsSuccessful() const { return bSearchIsSuccesful; }
	  TArray<FServerStatus> GetSessions() const { return Sessions; }

private:
	friend UCUBICARGameInstance;
	friend UCUBICARStatics;
};



/**
 *	Static Object to interact with CUBICAR Game Objects
 */
UCLASS()
class CUBICAR_API UCUBICARStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "CUBICAR", meta = (WorldContext = "WorldContextObject"))
		static UCUBICARGameInstance* GetCUBICARGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "CUBICAR")
		static UCUBICARSingleton* GetCUBICARData(bool &DataIsValid);

	UFUNCTION(BlueprintCallable, Category = "CUBICAR", meta = (WorldContext = "WorldContextObject"))
		static UUserWidget* AskToQuit(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "CUBICAR", meta = (WorldContext = "WorldContextObject"))
		static void Quit(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "CUBICAR", meta = (WorldContext = "WorldContextObject"))
		static void HostGame(const UObject* WorldContextObject, const FServerSettings &HostSettings);

	UFUNCTION(BlueprintCallable, Category = "CUBICAR", meta = (WorldContext = "WorldContextObject"))
		static void JoinGame(const UObject* WorldContextObject, const FServerStatus &HostSettings);

	UFUNCTION(BlueprintCallable, Category = "CUBICAR", meta = (WorldContext = "WorldContextObject"))
		static void FindGames(const UObject* WorldContextObject, const FServerSettings &HostSettings);


	UFUNCTION(BlueprintCallable, Category = "CUBICAR")
		static FServerSettings CreateSettings(int MaxPlayers,const EOnlineSSModule SubSystemtoUse, FName ServerName);

	UFUNCTION(BlueprintPure, Category = "CUBICAR|Map")
		static FName GetMainMenuMapName();

	UFUNCTION(BlueprintPure, Category = "CUBICAR|Map")
		static FName GetDefaultMapName();

	UFUNCTION(BlueprintPure, Category = "CUBICAR|Map")
		static FName GetLobbyMapName();

	UFUNCTION(BlueprintPure, Category = "CUBICAR|Map")
		static FName GetMapName(TSoftObjectPtr<UWorld> MapAsset);

	UFUNCTION(BlueprintPure, Category = "CUBICAR")
		static FName GetOnlineModuleName(EOnlineSSModule ModuleEnum);


	UFUNCTION(BlueprintPure, Category = "CUBICAR")
		static EOnlineSSModule GetOnlineModuleByName(FName ModuleName);


	UFUNCTION(BlueprintPure, Category = "CUBICAR")
	static EOnlineSSModule GetOnlineModuleByIndex(int Index);

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameRequest, const FServerSettings&, SessionParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionFound, const FSessionSearchResult&, SessionResult);
