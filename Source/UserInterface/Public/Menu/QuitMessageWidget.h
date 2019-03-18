// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModalWindowWidget.h"
#include "QuitMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class USERINTERFACE_API UQuitMessageWidget : public UModalWindowWidget
{
	GENERATED_BODY()

protected:
	

	UPROPERTY(meta = (BindWidget))
		UTextBlock * QuitMessage;

public:

	UPROPERTY(EditAnywhere)
		TArray<FText> QuitMessages;

	UFUNCTION(BlueprintPure)
		FText GetRandomQuitMessage();

	void OnWidgetRebuilt() override;




	
	
	
	
};
