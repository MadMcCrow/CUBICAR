// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUBICARWidget.h"
#include "MainMenuWidget.generated.h"

/**
 *  @class UMainMenuWidget class
 *  Base Class for the Main Menu, Should be overridden in Blueprints to set the look.
 */
UCLASS()
class CUBICAR_API UMainMenuWidget : public UCUBICARWidget
{
	GENERATED_BODY()

public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);


protected:
	UPROPERTY(meta = (BindWidget))
		class UCUBICARButton * QuitButton;

	UPROPERTY(meta = (BindWidget))
		class UCUBICARButton * HostButton;

	UPROPERTY(meta = (BindWidget))
		class UCUBICARButton * JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UCUBICARButton * SettingsButton;
	
	UFUNCTION()
		void QuitButtonCliqued();

	UFUNCTION(BlueprintNativeEvent) // QnD, @todo should revert to nothing
		void HostButtonCliqued();

	UFUNCTION(BlueprintNativeEvent)
		void JoinButtonCliqued();

	UFUNCTION()
		void SettingsButtonCliqued();


	virtual void BindDelegates() override;
	

	
};
