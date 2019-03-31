// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceWidget.h"
#include "RaceCar.h"
#include "Components/TextBlock.h"
#include "RaceStatics.h"
#include "RacePlayerState.h"
#include "RaceGameState.h"

URaceWidget::URaceWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URaceWidget::OnWidgetRebuilt()
{
	if (SpeedTextBlock)
		SpeedTextBlock->TextDelegate.BindDynamic(this, &URaceWidget::GetSpeedText);
	if (GearTextBlock)
		GearTextBlock->TextDelegate.BindDynamic(this, &URaceWidget::GetGearText);
	if (TimeTextBlock)
		TimeTextBlock->TextDelegate.BindDynamic(this, &URaceWidget::GetTimeText);

	const auto P = GetOwningPlayer();
	if(P)
	{
		auto Car = URaceStatics::GetControllersCar(P);
		if (Car)
		{
			RedLineRPM = Car->GetRedLineRPM();
			MaxRPM = Car->GetEngineMaxRPM();
			MinRPM = Car->GetEngineMinRPM();
		}
	}

}


FText URaceWidget::GetSpeedText()
{	
	const auto Car = Cast<ARaceCar>(GetOwningPlayerPawn());
	if (!Car)
		return FText();
	return UCarStatics::GetSpeed_textKPH(Car->GetDashboardInfo());
}

FText URaceWidget::GetGearText()
{
	const auto Car = Cast<ARaceCar>(GetOwningPlayerPawn());
	if (!Car)
		return FText();
	return UCarStatics::GetTransmission_textGear(Car->GetDashboardInfo());
}

float URaceWidget::GetRPMPercent()
{
	return (GetCurrentRPM() - MinRPM) / MaxRPM;
}

FText URaceWidget::GetRPMText()
{
	return FText::AsNumber(FMath::FloorToInt(GetCurrentRPM()));
}

float URaceWidget::GetCurrentRPM() const
{
	const auto P = GetOwningPlayer();
	if (P)
	{
		auto Car = URaceStatics::GetControllersCar(P);
		if (Car)
		{
			return Car->GetEngineCurrentRPM();
		}
	}
	return 0.f;
}

FText URaceWidget::GetTimeText()
{
	if(!GetOwningPlayer())
		return FText();

	const auto PS = GetOwningPlayerState<ARacePlayerState>();
	const auto GS = URaceStatics::GetRaceGameState(GetOwningPlayer());
	if (!PS || !GS)
		return FText();


	const auto Time = GS->RaceTime() - PS->GetLastCheckpointTime();
	FString TimeString;
	if (Time.ExportTextItem(TimeString, FTimespan::MinValue(), nullptr, 0, nullptr))
		return  FText::FromString(TimeString);


	return  FText();
}



