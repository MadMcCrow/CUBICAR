// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaceWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class RACE_API URaceWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	UPROPERTY(meta = (BindWidget))
		UTextBlock * SpeedTextBlock;

public:
	/**	Speed Text To display on SpeedTextBlock	*/
	UFUNCTION(BlueprintCallable, Category = "Binding", meta = (DisplayName = "Speed Text"))
		FText GetSpeedText() const;

	/**	Speed Value retrieved from Car 	*/
	UFUNCTION(BlueprintCallable, Category = "Binding", meta = (DisplayName = "Speed"))
		float GetSpeed() const;

	
	
};
