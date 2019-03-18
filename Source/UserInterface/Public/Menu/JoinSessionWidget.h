// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModalWindowWidget.h"
#include "SlateEnums.h"
#include "CUBICARStatics.h"
#include "JoinSessionWidget.generated.h"



class ULocalPlayer;
class UCUBICARButton;
class UEditableTextBox;
class UOnlineSessionWidget;
class UScrollBox;

/**
 *  Widget to Configure and set all options to host a game
 */
UCLASS()
class UJoinSessionWidget : public UModalWindowWidget
{
	GENERATED_BODY()
public :

	UJoinSessionWidget();


	virtual void SetIsEnabled(bool bInIsEnabled) override;

protected :
	/**	Class to use when spawning widgets	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly )
		TSubclassOf<UOnlineSessionWidget> SessionWidgetClass;

	UPROPERTY(meta = (BindWidget))
		UCUBICARButton  * RefreshButton;

	UPROPERTY(meta = (BindWidget))
		UEditableTextBox  * PlayerName;


	UPROPERTY(meta = (BindWidget))
		UScrollBox * SessionWidgetsContainers;
	   

	UPROPERTY(BlueprintReadOnly)
		TArray<UOnlineSessionWidget *> SessionWidgets;

	virtual void AddToScreen(ULocalPlayer * LocalPlayer, int32 ZOrder) override;
private:

	static int MaxPlayers;
	static EOnlineSSModule ModuleToUse;

	UPROPERTY()
		FServerSettings SearchCriterias;

	UPROPERTY()
		TArray<FServerStatus> Sessions;

	UPROPERTY(Transient)
		bool bLock;

	UFUNCTION()
		void NewPlayerName(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
		void UpdateSessions(const FSessionSearchResult &SearchResults);

	UFUNCTION()
		void AddSessionwidget(const FServerStatus &Status);

	virtual void BindDelegates() override;

	UFUNCTION()
		void RefreshSessions();

};
