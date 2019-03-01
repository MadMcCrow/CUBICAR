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

protected :

	void OnWidgetRebuilt() override;

	UPROPERTY(meta = (BindWidget))
		UCUBICARButton * CloseButton;


	UPROPERTY(meta = (BindWidget))
		class UTextBlock * Title;
	
};
