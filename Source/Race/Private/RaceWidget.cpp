// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceWidget.h"
#include "RacingCar.h"


FText URaceWidget::GetSpeedText() const
{	
	return FText::AsNumber(GetSpeed());
}

float URaceWidget::GetSpeed() const
{
	const auto Car = Cast<ARacingCar>(GetOwningPlayerPawn());
	if (!Car)
		return 0.f;
	return FMath::RoundHalfToZero(Car->GetVelocity().Size());
}
