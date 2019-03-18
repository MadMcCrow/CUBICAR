// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUBICARWidget.h"
#include "SlateEnums.h"
#include "ModalWindowWidget.h"
#include "HostSessionWidget.generated.h"



class UCUBICARButton;
class UEditableTextBox;
class UComboBoxString;

/**
 *  Widget to Configure and set all options to host a game
 */
UCLASS()
class UHostSessionWidget : public UModalWindowWidget
{
	GENERATED_BODY()

public :

	UHostSessionWidget();

	void OnWidgetRebuilt() override;

protected :

	UPROPERTY(meta = (BindWidget))
		UCUBICARButton * Host;

	UPROPERTY(meta = (BindWidget))
		UEditableTextBox  * ServerName;

	UPROPERTY(meta = (BindWidget))
		UEditableTextBox  * MaxPlayers;

	UPROPERTY(meta = (BindWidget))
		UCUBICARButton * MorePlayers;

	UPROPERTY(meta = (BindWidget))
		UCUBICARButton * LessPlayers;

	UPROPERTY(meta = (BindWidget))
		UComboBoxString  * Connection;


private :
	UPROPERTY()
		int Players;

	UPROPERTY()
		FName Name;

	

	

	UFUNCTION()
		void NewMaxPlayer(const FText & Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
		void NewServerName(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
		void UpdateServerName() const;

	UFUNCTION()
		void UpdateMaxPlayers() const;

	UFUNCTION()
		void IncrementPlayers() { Players++; UpdateMaxPlayers(); }

	UFUNCTION()
		void DecrementPlayers() { Players--; UpdateMaxPlayers(); }

	virtual void BindDelegates() override;

	UFUNCTION()
		void RequestHost();



};
