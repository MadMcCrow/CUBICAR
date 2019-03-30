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
public :
	URaceWidget(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

	void OnWidgetRebuilt() override;

protected:
	UPROPERTY(meta = (BindWidget))
		UTextBlock * SpeedTextBlock;

	UPROPERTY(meta = (BindWidget))
		UTextBlock * GearTextBlock;



public:
	/**	Speed Text To display on SpeedTextBlock	*/
	UFUNCTION(BlueprintPure, Category = "Binding", meta = (DisplayName = "Speed Text"))
		FText GetSpeedText();

	/**	Speed Text To display on SpeedTextBlock	*/
	UFUNCTION(BlueprintPure, Category = "Binding", meta = (DisplayName = "Gear Text"))
		FText GetGearText();

	/**	Speed Text To display on SpeedTextBlock	*/
	UFUNCTION(BlueprintPure, Category = "Binding", meta = (DisplayName = "RPM Percent"))
		float GetRPMPercent();

	/**	Speed Text To display on SpeedTextBlock	*/
	UFUNCTION(BlueprintPure, Category = "Binding", meta = (DisplayName = "RPM Text"))
		FText GetRPMText();

	UFUNCTION(BlueprintPure, Category = "Binding", meta = (DisplayName = "RPM float"))
		float GetCurrentRPM() const;

private :

	FGetText SpeedBinding;

	float RedLineRPM = 1;
	float MaxRPM = 1;
	float MinRPM = 1;



	
	
};
