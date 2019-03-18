// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUBICARWidget.h"
//#include "CUBICARButton.h"
//#include "ModalWindowWidget.h"
#include "MainMenuWidget.generated.h"


class UCUBICARButton;
class UModalWindowWidget;

/**
 * 
 */
UCLASS()
class USERINTERFACE_API UMainMenuWidget : public UCUBICARWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(meta = (BindWidget))
		UCUBICARButton * HostContextButton;

	UPROPERTY(meta = (BindWidget))
		UCUBICARButton * JoinContextButton;

	UPROPERTY(meta = (BindWidget))
		UCUBICARButton * OptionContextButton;

	UPROPERTY(meta = (BindWidget))
		UCUBICARButton * QuitButton;


	UPROPERTY(meta = (BindWidget))
		UModalWindowWidget * OptionContextMenu;
	
	UPROPERTY(meta = (BindWidget))
		UModalWindowWidget * HostContextMenu;

	UPROPERTY(meta = (BindWidget))
		UModalWindowWidget * JoinContextMenu;


	UFUNCTION(BlueprintCallable , meta = (DefaultToSelf))
		void ShowOptions();

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf))
		void ShowHost();

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf))
		void ShowJoin();

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf))
		void ShowQuit();


	virtual void BindDelegates() override;

public :

	virtual void SetIsEnabled(bool bInIsEnabled) override;

	
};
