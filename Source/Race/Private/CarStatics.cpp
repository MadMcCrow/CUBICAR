// Fill out your copyright notice in the Description page of Project Settings.

#include "CarStatics.h"
#include "Internationalization/Text.h"
#include "Engine/Engine.h"
#include "RaceCar.h"

#define LOCTEXT_NAMESPACE "VehiclePawn" /// todo: evaluate this  

FCarEngineSetup::FCarEngineSetup(): TorqueCurve(nullptr)
{
	//gear ratios
	Gears.Emplace(-2.90); //reverse
	Gears.Emplace(2.66); //1st
	Gears.Emplace(1.78);
	Gears.Emplace(1.30);
	Gears.Emplace(1.0);
	Gears.Emplace(0.74); //5th
}

 void FCarWheelsUpdate::GetIsSliding(TArray<bool> &Out) const
{
	 Out.Empty();
	 Out.Init(false, 4);
	 Out[0] = bIsSliding[0] != 0;
	 Out[1] = bIsSliding[1] != 0;
	 Out[2] = bIsSliding[2] != 0;
	 Out[3] = bIsSliding[3] != 0;
}

FCarWheelsSetup::FCarWheelsSetup()
{
	bIsPowered.Init(true, 4);
	bIsPowered[0] = bIsPowered[1] = false;
}

void FCarSuspensionsSetup::Init()
{
	SpringTopLocation.Empty();
	SpringTopLocation.Add(GetTopLocation(true, false)); //(FVector(120.0f, 90.0f, 20.0f));
	SpringTopLocation.Add(GetTopLocation(true, true)); //(FVector(120.0f, -90.0f, 20.0f));
	SpringTopLocation.Add(GetTopLocation(false, false)); //(FVector(-120.0f, 90.0f, 20.0f));
	SpringTopLocation.Add(GetTopLocation(false, true)); //(FVector(-120.0f, -90.0f, 20.0f));
}

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

FText UCarStatics::GetEngine_textRPM(const FCarDashBoard& Infos)
{
	return FText::AsNumber(FMath::FloorToInt(Infos.Engine_RPM));
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

void UCarStatics::GetSlidingWheels(const ARaceCar * Car, TArray<bool> &Out)
{
	if(Car)
	Car->WheelsUpdate.GetIsSliding(Out);
}

bool UCarStatics::GetSlidingFront(const ARaceCar* Car)
{
	return Car->WheelsUpdate.GetIsSlidingFront();
}

bool UCarStatics::GetSlidingBack(const ARaceCar* Car)
{
	return Car->WheelsUpdate.GetIsSlidingBack();
}