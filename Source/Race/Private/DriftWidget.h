// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DriftWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class UDriftWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
		UTextBlock * DriftTimeTextBlock;

	UPROPERTY(meta = (BindWidget))
		UTextBlock * DriftTextBlock;


public :

	void OnWidgetRebuilt() override;

	UFUNCTION(BlueprintPure, Category = "Binding", meta = (DisplayName = "DriftLengh"))
		FText GetDriftLength();

	UFUNCTION(BlueprintPure, Category = "Binding", meta = (DisplayName = "VisibleDrift"))
		ESlateVisibility GetIsDriftVisible();

	UFUNCTION(BlueprintPure, Category = "Binding", meta = (DisplayName = "VisibleDrift"))
		bool IsDrifting();

	
};
