// Fill out your copyright notice in the Description page of Project Settings.

#include "CarStatics.h"
#include "Internationalization/Text.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "VehiclePawn" /// todo: evaluate this  

float UCarStatics::GetSpeed_KPH(const FCarDashBoard& Infos)
{
	return Infos.Speed_KPH;
}

int UCarStatics::GetSpeed_intKPH(const FCarDashBoard& Infos)
{
	return FMath::FloorToInt(Infos.Speed_KPH);
}

FText UCarStatics::GetSpeed_textKPH(const FCarDashBoard &Infos)
{
	return FText::AsNumber(FMath::FloorToInt(Infos.Speed_KPH));
}

float UCarStatics::GetEngine_RPM(const FCarDashBoard& Infos)
{
	return Infos.Engine_RPM;
}

int UCarStatics::GetEngine_intRPM(const FCarDashBoard& Infos)
{
	return FMath::FloorToInt(Infos.Engine_RPM);
}
int UCarStatics::GetTransmission_intGear(const FCarDashBoard& Infos)
{
	return Infos.Transmission_Gear;
}

FText UCarStatics::GetTransmission_textGear(const FCarDashBoard& Infos)
{
	if (Infos.bIsInReverse)
		return FText(LOCTEXT("ReverseGear", "R"));
	return (Infos.Transmission_Gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(Infos.Transmission_Gear);
}
