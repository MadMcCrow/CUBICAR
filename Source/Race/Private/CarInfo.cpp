// Fill out your copyright notice in the Description page of Project Settings.

#include "CarInfo.h"
#include "RacingCar.h"
#include "GameFramework/Controller.h"

// Add default functionality here for any ICarInfo functions that are not pure virtual.

FCarStatsInfo::FCarStatsInfo(const ARacingCar * Car)
{
	if (!Car)
		return;

	const auto mesh = Car->GetMesh();

	if (!mesh)
		return;

	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(Car->GetVehicleMovement());
	if (!Vehicle4W)
		return;

	Dimensions = mesh->CalcBounds(Car->GetActorTransform()).BoxExtent;
	Mass = mesh->GetMass();

	MaxRPM = Vehicle4W->MaxEngineRPM;
	MaxGear = Vehicle4W->TransmissionSetup.ForwardGears.Num();


	// Find max torque ( solve compilation error that occured when Using FindPeakTorque)
	float PeakTorque = 0.f;
	TArray<FRichCurveKey> TorqueKeys = Vehicle4W->EngineSetup.TorqueCurve.GetRichCurveConst()->GetCopyOfKeys();
	for (int32 KeyIdx = 0; KeyIdx < TorqueKeys.Num(); KeyIdx++)
	{
		FRichCurveKey& Key = TorqueKeys[KeyIdx];
		PeakTorque = FMath::Max(PeakTorque, Key.Value);
	}

	MaxHP = PeakTorque * MaxRPM / 5252.0;

}
FCarDashboardInfo ICarInfo::GetPlayerCarDasboardInfo(const AController * Player) const
{
	const auto racingcar = Cast<ARacingCar>(Player->GetPawn());
	if (racingcar)
		return racingcar->GetCarDasboard();
	return FCarDashboardInfo();
}

FCarStatsInfo ICarInfo::GetPlayerCarStatisticsInfo(const AController * Player) const
{
	const auto racingcar = Cast<ARacingCar>(Player->GetPawn());
	if (racingcar)
		return racingcar->GetCarStats();
	return FCarStatsInfo();
}
