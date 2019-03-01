// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "CUBICARButton.h"
//#include "ModalWindowWidget.h"
#include "MainMenuWidget.generated.h"

class UCUBICARButton;
class UModalWindowWidget;

/**
 * 
 */
UCLASS()
class CUBICAR_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
		UCUBICARButton * HostButton;

	UPROPERTY(meta = (BindWidget))
		UCUBICARButton * FindMatchButton;


	UPROPERTY(meta = (BindWidget))
		UModalWindowWidget * OptionContextMenu;
	
	UPROPERTY(meta = (BindWidget))
		UModalWindowWidget * HostContextMenu;

	UPROPERTY(meta = (BindWidget))
		UModalWindowWidget * JoinContextMenu;


	
};
