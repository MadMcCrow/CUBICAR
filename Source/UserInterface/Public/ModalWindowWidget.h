// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUBICARWidget.h"
#include "ModalWindowWidget.generated.h"

class UCUBICARButton;

/**
 * 
 */
UCLASS()
class USERINTERFACE_API UModalWindowWidget : public UCUBICARWidget
{
	GENERATED_BODY()
public :

	UModalWindowWidget();

	virtual bool Initialize() override;

	virtual void SetIsEnabled(bool bInIsEnabled) override;

	virtual void OnWidgetRebuilt() override;


	/**	Hides the Modal menu */
	UFUNCTION(BlueprintCallable, meta=(DefaultsToSelf))
	virtual void Close();

	/**	Shows the Modal menu */
	UFUNCTION(BlueprintCallable, meta=(DefaultsToSelf))
		virtual void Open();

protected :

	UPROPERTY(meta = (BindWidget))
		UCUBICARButton * CloseButton;


	UPROPERTY(meta = (BindWidget))
		class UTextBlock * Title;

	UPROPERTY(EditAnywhere)
		FText WindowTitle;

	void BindDelegates() override;
};
