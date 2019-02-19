// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RacingCar.h"
#include "Race.h"
#include "RaceWheel.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Sound/SoundCue.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/Engine.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"
#include "RacePlayerState.h"
#include "Kismet/KismetMathLibrary.h"

#define LOCTEXT_NAMESPACE "VehiclePawn"

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

FCarStatsInfo ARacingCar::GetCarStats()
{
	return FCarStatsInfo(this);
}

void ARacingCar::UpdateCarInfo()
{
	CarInfo.Speed = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	CarInfo.Gear = GetVehicleMovement()->GetCurrentGear();
	CarInfo.RPM = GetVehicleMovement()->GetEngineRotationSpeed();

}

ARacingCar::ARacingCar() :Super(), ResetDelay(10.f)
{
	// Bind Drift Event
	StartSkidding.AddDynamic(this, &ARacingCar::Drift);
	StopSkidding.AddDynamic(this, &ARacingCar::EndDrift);
}

void ARacingCar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("ResetCar", IE_Pressed, this, &ARacingCar::ResetCar);
}


void ARacingCar::ResetCar()
{
	if (!bCanReset)
		return;
	GetMesh()->AddImpulse(FVector(0.f, 0.f, 1000.f), NAME_None, true);
	FTimerHandle ResetEffectDelayHandle;
	float EffectDelay = 0.2;
	GetWorldTimerManager().SetTimer(ResetEffectDelayHandle, this, &ARacingCar::ResetCarRotation, EffectDelay, false, EffectDelay);
	GetWorldTimerManager().SetTimer(ResetDelayHandle, this, &ARacingCar::SetCanReset, ResetDelay, false,  ResetDelay);
	bCanReset = false;
	
}

void ARacingCar::CheckForUpsideDown()
{
	const auto VMovement = GetVehicleMovement();
	if (!VMovement)
		return;
	if( FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) > IsStoppedThreshold)
		return;

	// Then car is really upside down and we need to do something about it:
	bCanReset = true;
	GetWorldTimerManager().SetTimer(ResetDelayHandle, this, &ARacingCar::SetCanReset, 0.f, false, 0.f);
	OnCarIsUpsideDown.Broadcast();
}


void ARacingCar::ResetCarRotation()
{
	const FRotator Rot = GetActorRotation();
	SetActorRotation(FRotator(0.f,Rot.Yaw, 0.f), ETeleportType::TeleportPhysics);
	GetMesh()->SetAllPhysicsPosition(GetActorLocation());
	GetMesh()->SetAllPhysicsLinearVelocity(FVector(0.f));
	GetMesh()->SetAllPhysicsAngularVelocityInDegrees(FVector(0.f));
	//Mesh->SetAllPhysicsRotation()
	GetVehicleMovementComponent()->SetTargetGear(0, true);
}

void ARacingCar::Stabilize(const float &Stability, const float &StabilizingSpeed)
{
	const FVector Angular = GetMesh()->GetPhysicsAngularVelocityInDegrees();
	const FRotator Axis = UKismetMathLibrary::RotatorFromAxisAndAngle(Angular, Angular.Size()*Stability / StabilizingSpeed);
	const FVector Torque = (StabilizingSpeed * StabilizingSpeed) * UKismetMathLibrary::GetUpVector(FRotator(0.f, 0.f, 0.f)) * Axis.RotateVector(GetActorUpVector());
	GetMesh()->AddTorqueInDegrees(Torque,NAME_None, true); // try in radian if weird behaviour
}

void ARacingCar::Drift_Implementation()
{

}

void ARacingCar::EndDrift_Implementation()
{

}

void ARacingCar::CheckSkidding()
{
	bool SlipThresholdExceeded = GetVehicleMovement()->CheckSlipThreshold(LongitudinalSlipThreshold, LateralSlipThreshold);
	if (SlipThresholdExceeded && !IsSkidding)
	{
		StartSkidding.Broadcast();
		IsSkidding = true;
	}
	else if (!SlipThresholdExceeded && IsSkidding)
	{
		StopSkidding.Broadcast();
		IsSkidding = false;
	}
}

void ARacingCar::PassedCheckpoint_Implementation(ACheckpoint* Checkpoint)
{
	const auto PlayerState = Cast<ARacePlayerState>(GetController()->PlayerState);
	if (!PlayerState)
		return;

}

void ARacingCar::Tick(float Delta)
{
	Super::Tick(Delta);

	// Update the strings used in the hud (incar and onscreen)
	UpdateCarInfo();
	CheckSkidding();
}

void ARacingCar::BeginPlay()
{
	Super::BeginPlay();
	bCanReset = true;
}


#undef LOCTEXT_NAMESPACE
