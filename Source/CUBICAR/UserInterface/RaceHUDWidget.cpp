// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceHUDWidget.h"
#include "RacingCar.h"


URaceHUDWidget::URaceHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CarPawn = Cast<ARacingCar>(GetOwningPlayerPawn());
}

bool URaceHUDWidget::Initialize()
{
	bool Ret = Super::Initialize();
	CarPawn = Cast<ARacingCar>(GetOwningPlayerPawn());
	return Ret;
}

void URaceHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateDashBoardValues();
}


void URaceHUDWidget::UpdateDashBoardValues()
{
	if (!CarPawn)
	{
		CarPawn = Cast<ARacingCar>(GetOwningPlayerPawn());
		return;
	}
	Speed = CarPawn->GetCarDasboard().Speed;
	RPM = CarPawn->GetCarDasboard().RPM;
	Gear = CarPawn->GetCarDasboard().Gear;
}


void URaceHUDWidget::BindDelegates()
{
	Super::BindDelegates();

}

FText URaceHUDWidget::GetCarSpeedAsText() const
{
	FNumberFormattingOptions Options;
	Options.SetRoundingMode(ERoundingMode::HalfFromZero);
	Options.SetMaximumFractionalDigits(1);
	return FText::AsNumber(Speed, &Options );
}

FText URaceHUDWidget::GetCarRPMAsText() const
{
	FNumberFormattingOptions Options;
	Options.SetRoundingMode(ERoundingMode::HalfFromZero);
	Options.SetMaximumFractionalDigits(1);
	return FText::AsNumber(RPM, &Options);
}

FText URaceHUDWidget::GetCarGearAsText() const
{
	switch (Gear)
	{
	case -1:
		return FText::FromString(TEXT("R"));
	case 0:
		return FText::FromString(TEXT("N"));
	default:
		return FText::AsNumber(Gear);
	}

}

ARacingCar* URaceHUDWidget::GetCarPawn()
{
	return  Cast<ARacingCar>(GetOwningPlayerPawn());

}

