// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpleCar.h"
#include "CoreUObject.h"
#include "Engine.h"
#include "Race.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "PhysicsPublic.h"
#include "Particles/ParticleSystem.h"
//#include "math.h"

FName ASimpleCar::VehicleMeshComponentName(TEXT("VehicleMesh"));

// Sets default values
ASimpleCar::ASimpleCar()
{

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(VehicleMeshComponentName);
	GetMesh()->SetCollisionProfileName(UCollisionProfile::Vehicle_ProfileName);
	//GetMesh()->SetVisibility(false , false);
	GetMesh()->BodyInstance.bSimulatePhysics = true;
	GetMesh()->BodyInstance.bNotifyRigidBodyCollision = true;
	GetMesh()->BodyInstance.bUseCCD = true;
	GetMesh()->bBlendPhysics = true;
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCanEverAffectNavigation(false);
	RootComponent = GetMesh();

	//camera stuff //
	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 50.f);
	SpringArm->SetRelativeRotation(FRotator(-12.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent); // FAttachmentTransformRules::KeepWorldTransform);
	SpringArm->TargetArmLength = 750.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // FAttachmentTransformRules::KeepWorldTransform, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	//arrows used as trace start locations
	SuspensionsSetup.SpringTopLocation.Add(SuspensionsSetup.SpringConfiguration.GetLocation(true, false));
	SuspensionsSetup.SpringTopLocation.Add(SuspensionsSetup.SpringConfiguration.GetLocation(true, true));
	SuspensionsSetup.SpringTopLocation.Add(SuspensionsSetup.SpringConfiguration.GetLocation(false, false));
	SuspensionsSetup.SpringTopLocation.Add(SuspensionsSetup.SpringConfiguration.GetLocation(false, true));

	/*
	SuspensionsSetup.SpringTopLocation.Add(FVector(120.0f, 90.0f, 20.0f));
	SuspensionsSetup.SpringTopLocation.Add(FVector(120.0f, -90.0f, 20.0f));
	SuspensionsSetup.SpringTopLocation.Add(FVector(-120.0f, 90.0f, 20.0f));
	SuspensionsSetup.SpringTopLocation.Add(FVector(-120.0f, -90.0f, 20.0f));
	*/

#if WITH_EDITOR

	Arrow0 = CreateDefaultSubobject<UArrowComponent>(TEXT("arrow0"));
	Arrow1 = CreateDefaultSubobject<UArrowComponent>(TEXT("arrow1"));
	Arrow2 = CreateDefaultSubobject<UArrowComponent>(TEXT("arrow2"));
	Arrow3 = CreateDefaultSubobject<UArrowComponent>(TEXT("arrow3"));
	
	ArrowArray.Emplace(Arrow0);
	ArrowArray.Emplace(Arrow1);
	ArrowArray.Emplace(Arrow2);
	ArrowArray.Emplace(Arrow3);

	for (int c = 0; c <4; c++)
	{
		ArrowArray[c]->SetupAttachment(RootComponent);//AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		ArrowArray[c]->SetRelativeLocation(SuspensionsSetup.SpringTopLocation[c]);
	}

#endif // WITH_EDITOR


	//wheel meshes
	Wheel0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel0"));
	WheelArray.Emplace(Wheel0);

	Wheel1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel1"));
	WheelArray.Emplace(Wheel1);

	Wheel2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel2"));
	WheelArray.Emplace(Wheel2);

	Wheel3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel3"));
	WheelArray.Emplace(Wheel3);

	
	for (auto it : WheelArray)
	{
		it->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		it->SetupAttachment(RootComponent);//AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		if (WheelMesh)
		{
			it->SetStaticMesh(WheelMesh);
		}
	}

	//engine sound
	EngineAC = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineAudio"));
	if (CarSound.EngineSound)
		EngineAC->SetSound(CarSound.EngineSound);
	EngineAC->SetupAttachment(GetMesh()); // AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);




	SuspensionsUpdate.SpringForceArray.Init(FVector(0.0f, 0.0f, 0.0f), 4);
	//TireForceArray.Init(FVector(0.0f, 0.0f, 0.0f), 4);
	//WheelForwardArray.Init(FVector(0.0f, 0.0f, 0.0f), 4);
	//WheelRightArray.Init(FVector(0.0f, 0.0f, 0.0f), 4);
	SuspensionsUpdate.SuspForceLocation.Init(FVector(0.0f, 0.0f, 0.0f), 4);

	SuspensionsUpdate.SpringLengthArray.Init(0.0f, 4);

	SuspensionsUpdate.PreviousPosition.Init(0.0f, 4);

	SuspensionsUpdate.bOnGround.Init(false, 4);
	WheelsSetup.bIsPowered.Init(true, 4);

	WheelsUpdate.Init();

	//gear ratios
	EngineSetup.Gears.Emplace(-2.90);//reverse
	EngineSetup.Gears.Emplace(2.66);//1st
	EngineSetup.Gears.Emplace(1.78);
	EngineSetup.Gears.Emplace(1.30);
	EngineSetup.Gears.Emplace(1.0);
	EngineSetup.Gears.Emplace(0.74);//5th


	// Bind function delegate
	OnCalculateCustomPhysics.BindUObject(this, &ASimpleCar::CustomPhysics);

}

// Called when the game starts or when spawned
void ASimpleCar::BeginPlay()
{
	Super::BeginPlay();

	if (EngineAC)
	{
		EngineAC->SetSound(CarSound.EngineSound);
		EngineAC->Play();
	}

	if (GetMesh() != NULL) {
		MainBodyInstance = GetMesh()->GetBodyInstance();
	}

	for (auto it : WheelArray)
	{
		it->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (WheelMesh)
			it->SetStaticMesh(WheelMesh);
	}
}

// Called every frame
void ASimpleCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WheelArray.Num() == 0)
	{
		UE_LOG(LogRace, Error, TEXT("Wheel Array not set"));
		return;
	}

	//work out wheel center locations from visiblemesh instead of physics body to avoid wheel lag
	FTransform BodyTransform = GetMesh()->GetComponentTransform();
	FVector BodyLocation = BodyTransform.GetLocation();

	FVector BodyForwardVector = BodyTransform.GetUnitAxis(EAxis::X);
	FVector BodyRightVector = BodyTransform.GetUnitAxis(EAxis::Y);
	FVector BodyUpVector = BodyTransform.GetUnitAxis(EAxis::Z);

	for (int32 b = 0; b < WheelArray.Num(); b++)
	{
		// Store SpringTopLocation
		FVector SpringTopLocation = FVector();

		if (SuspensionsSetup.SpringTopLocation.IsValidIndex(b))
			SpringTopLocation = SuspensionsSetup.SpringTopLocation[b];

		// Calculate suspension location
		FVector SuspensionLocation = BodyLocation + BodyForwardVector * SpringTopLocation.X + BodyRightVector * SpringTopLocation.Y + BodyUpVector * SpringTopLocation.Z;
		
		FVector TempVel(0.f);
		if(MainBodyInstance)
			TempVel = MainBodyInstance->GetUnrealWorldVelocityAtPoint(SuspensionLocation);

		int WheelsNum = WheelsUpdate.WheelCenterLocation.Num();

		if (WheelsNum != 4)
		{
			UE_LOG(LogTemp, Warning, TEXT("there are %d wheels"), WheelsNum);
		}


			// Safety before all
			if (!WheelsUpdate.WheelCenterLocation.IsValidIndex(b) || 
				!SuspensionsUpdate.SpringLengthArray.IsValidIndex(b) || 
				!WheelsUpdate.CurrentAngle.IsValidIndex(b) ||
				!WheelsUpdate.bIsSliding.IsValidIndex(b)||
				!WheelsUpdate.CurrentWheelPitch.IsValidIndex(b) ||
				!WheelArray.IsValidIndex(b))
			{
				UE_LOG(LogRace, Error, TEXT("Array are not sete with enough wheels"));
				break; // we should not move further
			}
	

			WheelsUpdate.WheelCenterLocation[b] = SuspensionLocation - (BodyUpVector * (SuspensionsUpdate.SpringLengthArray[b] - WheelsSetup.Radius));
				

			// Calculate wheel forward vector
			FVector WheelForward = BodyForwardVector.RotateAngleAxis(WheelsUpdate.CurrentAngle[b], BodyUpVector);
			// Calculate wheel speed along the wheel forward direction
			float ForwardSpeed = FVector::DotProduct(WheelForward, TempVel);

			// Calculate delta rotation for this wheel
			WheelsUpdate.DeltaPitch = -ForwardSpeed * WheelsSetup.RotationAmount*DeltaTime;

			// Apply wheel delta rotation to the current rotation
			WheelsUpdate.CurrentWheelPitch[b] -= WheelsUpdate.DeltaPitch;

					//tire smoke
			if (WheelsUpdate.bIsSliding[b]) {
				//spawn smoke particle
				SpawnSmokeEffect(b);
				UpdateWheelEffects(DeltaTime, b);
				//reset sliding
				WheelsUpdate.bIsSliding[b] = false;
			}
			else {
				if (DustPSC[b]) {
					DustPSC[b]->SetActive(false);
					DustPSC[b]->bAutoDestroy = true;
				}

			}
		
			//rotate/locate wheel meshes
			WheelArray[b]->SetWorldLocation(WheelsUpdate.WheelCenterLocation[b]);
			WheelArray[b]->SetRelativeRotation(FRotator(WheelsUpdate.CurrentWheelPitch[b], WheelsUpdate.CurrentAngle[b], 0.0f));
			/**DrawDebugLine(
			GetWorld(),
			SuspensionLocation,
			WheelCenterLocation[b],
			FColor(255, 0, 0),
			false, -1, 0,
			12.333
			);*/
	}


	// Add custom physics on MainBodyMesh
	if (MainBodyInstance != NULL) {
		MainBodyInstance->AddCustomPhysics(OnCalculateCustomPhysics);
	}

	//engine sound pitch
	if (EngineAC)
	{
		float EnginePitch = 0.0f;
		EnginePitch += ((EngineUpdate.EngineRPM / EngineSetup.EngineMaxRPM)* (CarSound.EnginePitchMax - CarSound.EnginePitchIdle)) + CarSound.EnginePitchIdle;
		EnginePitch = FMath::Clamp(EnginePitch, CarSound.EnginePitchIdle, CarSound.EnginePitchMax);
		EngineAC->SetPitchMultiplier(EnginePitch);
	}

}

void ASimpleCar::SpawnSmokeEffect(int WheelIndex)
{
	UParticleSystemComponent* NewParticle = NewObject<UParticleSystemComponent>(this);
	NewParticle->SetWorldLocation(WheelsUpdate.TireHitLocation[WheelIndex]);
	NewParticle->bAutoActivate = true;
	NewParticle->bAutoDestroy = false;
	NewParticle->RegisterComponentWithWorld(GetWorld());
	NewParticle->SetTemplate(TireSmoke);

}

//for tire marks
void ASimpleCar::UpdateWheelEffects(float DeltaTime, int32 Index)
{
	if (TireMarks) {

		UParticleSystem* WheelFX = TireMarks;

		const bool bIsActive = DustPSC[Index] != NULL && !DustPSC[Index]->bWasDeactivated && !DustPSC[Index]->bWasCompleted;
		UParticleSystem* CurrentFX = DustPSC[Index] != NULL ? DustPSC[Index]->Template : NULL;
		if (WheelFX != NULL && (CurrentFX != WheelFX || !bIsActive))
		{
			if (DustPSC[Index] == NULL || !DustPSC[Index]->bWasDeactivated)
			{
				if (DustPSC[Index] != NULL)
				{
					DustPSC[Index]->SetActive(false);
					DustPSC[Index]->bAutoDestroy = true;
				}
				SpawnNewWheelEffect(Index);

			}
			DustPSC[Index]->SetTemplate(WheelFX);
			DustPSC[Index]->ActivateSystem();

		}
		else if (WheelFX == NULL && bIsActive)
		{
			DustPSC[Index]->SetActive(false);
		}

		if (DustPSC[Index]) {
			//set position
			DustPSC[Index]->SetWorldLocation(WheelsUpdate.TireHitLocation[Index]);
		}


	}
}

void ASimpleCar::SpawnNewWheelEffect(int WheelIndex)
{
	DustPSC[WheelIndex] = NewObject<UParticleSystemComponent>(this);
	DustPSC[WheelIndex]->bAutoActivate = true;
	DustPSC[WheelIndex]->bAutoDestroy = false;
	DustPSC[WheelIndex]->RegisterComponentWithWorld(GetWorld());
	DustPSC[WheelIndex]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
}

// Called to bind functionality to input
void ASimpleCar::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ASimpleCar::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASimpleCar::MoveRight);
	InputComponent->BindAction("ChangeUp", IE_Pressed, this, &ASimpleCar::ChangeUp);
	InputComponent->BindAction("ChangeDown", IE_Pressed, this, &ASimpleCar::ChangeDown);

}

void ASimpleCar::MoveForward(float Val)
{
	EngineUpdate.Throttle = Val;
}

void ASimpleCar::MoveRight(float Val)
{
	//for substep calculations
	WheelsUpdate.CurrentAngle[0] = FMath::Lerp(WheelsUpdate.CurrentAngle[0], WheelsSetup.SteerAngle * Val, WheelsSetup.SteerSpeed * GetWorld()->DeltaTimeSeconds);
	WheelsUpdate.CurrentAngle[1] = FMath::Lerp(WheelsUpdate.CurrentAngle[1], WheelsSetup.SteerAngle * Val, WheelsSetup.SteerSpeed * GetWorld()->DeltaTimeSeconds);

}


TArray<UStaticMeshComponent*> ASimpleCar::GetWheelArray()
{
	TArray<UStaticMeshComponent*> Wheels;
	Wheels.Emplace(Wheel0);
	Wheels.Emplace(Wheel1);
	Wheels.Emplace(Wheel2);
	Wheels.Emplace(Wheel3);
	return Wheels;
}

// Called every substep for selected body instance
void ASimpleCar::CustomPhysics(float DeltaTime, FBodyInstance* BodyInstance)
{
	//get physics body location
	FTransform BodyTransform = BodyInstance->GetUnrealWorldTransform();
	FVector BodyLocation = BodyTransform.GetLocation();

	//get direction vectors
	FVector BodyForwardVector = BodyTransform.GetUnitAxis(EAxis::X);
	FVector BodyRightVector = BodyTransform.GetUnitAxis(EAxis::Y);
	FVector BodyUpVector = BodyTransform.GetUnitAxis(EAxis::Z);

	//forward speed
	FVector Vel = BodyInstance->GetUnrealWorldVelocity();

	static const float Speed = FMath::Abs(FVector::DotProduct(BodyForwardVector, Vel)) * 0.036f;
	Dashboard.Speed_KPH = Speed;

	//add air resistance force
	FVector DragForce = -Vel * Speed * EngineSetup.AirResistance;
	BodyInstance->AddImpulseAtPosition(DragForce * DeltaTime, BodyLocation);

	//if we got wheels	
	if (WheelArray.Num() > 0) {

		//trace for each wheel	
		for (int32 Index = 0; Index < WheelArray.Num(); Index++)
		{

			//wheel direction vectors
			FVector WheelForward = BodyForwardVector.RotateAngleAxis(WheelsUpdate.CurrentAngle[Index], BodyUpVector);
			FVector WheelRight = BodyRightVector.RotateAngleAxis(WheelsUpdate.CurrentAngle[Index], BodyUpVector);

			FVector SuspensionLocation = BodyLocation + BodyForwardVector * SuspensionsSetup.SpringTopLocation[Index].X + BodyRightVector * SuspensionsSetup.SpringTopLocation[Index].Y + BodyUpVector * SuspensionsSetup.SpringTopLocation[Index].Z;
			//set location for susp force
			SuspensionsUpdate.SuspForceLocation[Index] = SuspensionLocation;

			//trace
			FHitResult Hit = Trace(SuspensionLocation, -BodyUpVector);

			if (Hit.bBlockingHit) {

				SuspensionsUpdate.SpringLocation = Hit.ImpactPoint;

				float SpringPosition = (Hit.Location - SuspensionLocation).Size();
				SuspensionsUpdate.SpringLengthArray[Index] = SpringPosition;

				// If previously on air, set previous position to current position
				if (!SuspensionsUpdate.bOnGround[Index]) {
					SuspensionsUpdate.PreviousPosition[Index] = SpringPosition;
				}
				float DamperVelocity = (SpringPosition - SuspensionsUpdate.PreviousPosition[Index]) / DeltaTime;
				SuspensionsUpdate.PreviousPosition[Index] = SpringPosition;
				SuspensionsUpdate.bOnGround[Index] = true;

				// Calculate spring force
				float SpringForce = (1 - (SpringPosition / SuspensionsSetup.TraceLength)) * SuspensionsSetup.SpringValue;

				// Apply damper force
				SpringForce -= SuspensionsSetup.DamperValue * DamperVelocity;

				FVector TotalForce = Hit.ImpactNormal * FVector::DotProduct(Hit.ImpactNormal, SpringForce * BodyUpVector); //SpringForce * BodyUpVector;//// Hit.ImpactNormal;
				//set total force for this spring
				SuspensionsUpdate.SpringForceArray[Index] = TotalForce;

				//for force calcs and tire smoke
				WheelsUpdate.TireHitLocation[Index] = SuspensionsUpdate.SpringLocation;

			}
			else {
				SuspensionsUpdate.bOnGround[Index] = false;
				SuspensionsUpdate.SpringLengthArray[Index] = SuspensionsSetup.TraceLength;

			}

			//grip and drive
			if (SuspensionsUpdate.bOnGround[Index]) {
				//multiply grip by spring length, compressed = more grip ...
				float GripMultiplier = FMath::Max(SuspensionsSetup.TraceLength / SuspensionsUpdate.SpringLengthArray[Index], WheelsSetup.MaxGrip);

				//add tire forces
				//lat grip
				FVector TireForces = AddLatGrip(DeltaTime, BodyInstance, SuspensionsUpdate.SpringLocation, WheelRight, Index);
				//engine/wheel torque
				if (WheelsSetup.bIsPowered[Index]) {
					TireForces += AddDrive(DeltaTime, BodyInstance, SuspensionsUpdate.SpringLocation, WheelForward, Index);
				}


				TireForces *= GripMultiplier;
				BodyInstance->AddImpulseAtPosition(TireForces * DeltaTime, SuspensionsUpdate.SpringLocation);
			}


			//~~~~~~~~~~~~~~~~~~~~~~
		}//end for()



		//add in anti-roll, take a bit of force from the longest spring and add it to the shortest
		//we can do this with the force because RB local -z will be the same for all wheels
		//anti roll front
		FVector AntiRollForceF = FVector();
		if (SuspensionsUpdate.bOnGround[0] && SuspensionsUpdate.bOnGround[1]) {
			AntiRollForceF = (SuspensionsUpdate.SpringForceArray[0] - SuspensionsUpdate.SpringForceArray[1]) * SuspensionsSetup.AntiRollFront;
			AntiRollForceF.X = FMath::Clamp(AntiRollForceF.X, -SuspensionsSetup.MaxSpringValue, SuspensionsSetup.MaxSpringValue);
			AntiRollForceF.Y = FMath::Clamp(AntiRollForceF.Y, -SuspensionsSetup.MaxSpringValue, SuspensionsSetup.MaxSpringValue);
			AntiRollForceF.Z = FMath::Clamp(AntiRollForceF.Z, -SuspensionsSetup.MaxSpringValue, SuspensionsSetup.MaxSpringValue);
		}

		//anti roll back
		FVector AntiRollForceB = FVector();
		if (SuspensionsUpdate.bOnGround[2] && SuspensionsUpdate.bOnGround[3]) {
			AntiRollForceB = (SuspensionsUpdate.SpringForceArray[2] - SuspensionsUpdate.SpringForceArray[3]) * SuspensionsSetup.AntiRollBack;
			AntiRollForceB.X = FMath::Clamp(AntiRollForceB.X, -SuspensionsSetup.MaxSpringValue, SuspensionsSetup.MaxSpringValue);
			AntiRollForceB.Y = FMath::Clamp(AntiRollForceB.Y, -SuspensionsSetup.MaxSpringValue, SuspensionsSetup.MaxSpringValue);
			AntiRollForceB.Z = FMath::Clamp(AntiRollForceB.Z, -SuspensionsSetup.MaxSpringValue, SuspensionsSetup.MaxSpringValue);
		}

		//suspension springs
		//front
		if (SuspensionsUpdate.bOnGround[0]) {
			SuspensionsUpdate.SpringForceArray[0] += AntiRollForceF;
			BodyInstance->AddImpulseAtPosition(SuspensionsUpdate.SpringForceArray[0] * DeltaTime, SuspensionsUpdate.SuspForceLocation[0]);
		}
		if (SuspensionsUpdate.bOnGround[1]) {
			SuspensionsUpdate.SpringForceArray[1] -= AntiRollForceF;
			BodyInstance->AddImpulseAtPosition(SuspensionsUpdate.SpringForceArray[1] * DeltaTime, SuspensionsUpdate.SuspForceLocation[1]);
		}
		//back
		if (SuspensionsUpdate.bOnGround[2]) {
			SuspensionsUpdate.SpringForceArray[2] += AntiRollForceB;
			BodyInstance->AddImpulseAtPosition(SuspensionsUpdate.SpringForceArray[2] * DeltaTime, SuspensionsUpdate.SuspForceLocation[2]);
		}
		if (SuspensionsUpdate.bOnGround[3]) {
			SuspensionsUpdate.SpringForceArray[3] -= AntiRollForceB;
			BodyInstance->AddImpulseAtPosition(SuspensionsUpdate.SpringForceArray[3] * DeltaTime, SuspensionsUpdate.SuspForceLocation[3]);
		}
	}
}

FHitResult ASimpleCar::Trace(FVector TraceStart, FVector TraceDirection) {
	//should change to shape trace, look into that................ <------------------------------------
	FHitResult Hit(ForceInit);
	FCollisionQueryParams TraceParams(FName(TEXT("Dummy_Tag")), true);
	FCollisionObjectQueryParams ObjectParams = ECC_WorldStatic | ECC_WorldDynamic; // object types that trace hits
	TraceParams.bTraceAsyncScene = true;
	//ignore self
	TraceParams.AddIgnoredActor(this);
	TraceParams.bReturnPhysicalMaterial = false;
	FVector TraceEnd = TraceStart + (TraceDirection * SuspensionsSetup.TraceLength);

	GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjectParams, TraceParams);

	return Hit;
}

void ASimpleCar::ChangeUp()
{
	if (EngineUpdate.CurrentGear < (EngineSetup.Gears.Num() - 1)) {
		EngineUpdate.CurrentGear++;
	}
}

void ASimpleCar::ChangeDown()
{
	if (EngineUpdate.CurrentGear > 0) {
		EngineUpdate.CurrentGear--;
	}
}

//force to wheels from 'engine'
FVector ASimpleCar::AddDrive(float DeltaTime, FBodyInstance* BodyInstance, FVector Loc, FVector Dir, int32 Index) {

	FVector TempVel = BodyInstance->GetUnrealWorldVelocityAtPoint(Loc);
	FTransform BodyTransform = BodyInstance->GetUnrealWorldTransform();
	FVector BodyLocation = BodyTransform.GetLocation();

	FVector BodyForwardVector = BodyTransform.GetUnitAxis(EAxis::X);
	FVector BodyUpVector = BodyTransform.GetUnitAxis(EAxis::Z);

	FVector WheelForward = BodyForwardVector.RotateAngleAxis(WheelsUpdate.CurrentAngle[Index], BodyUpVector);



	//reverse
	if (FVector::DotProduct(WheelForward, TempVel) < 10.0f) {
		EngineUpdate.bInReverse = true;
	}
	else {
		EngineUpdate.bInReverse = false;
	}

	//braking
	if (EngineUpdate.Throttle > 0.0) {
		if (EngineUpdate.bInReverse) {
			EngineUpdate.bBraking = true;
		}
		else {
			EngineUpdate.bBraking = false;
		}
	}
	else if (EngineUpdate.Throttle < 0.0) {
		if (EngineUpdate.bInReverse) {
			EngineUpdate.bBraking = false;
		}
		else {
			EngineUpdate.bBraking = true;
		}
	}
	else {
		EngineUpdate.bBraking = false;
	}


	//lag or slowdown from mechanical/friction forces
	float ForwardSpeed = FVector::DotProduct(WheelForward, TempVel);

	FVector TempVec = -WheelForward * ForwardSpeed * EngineSetup.EngineBrake;

	EngineUpdate.EnginePower = GetPowerToWheels(DeltaTime, BodyInstance);

	//final power output 
	//multiply grip by spring length, compressed = more grip ...
	float GripMultiplier = FMath::Max(SuspensionsSetup.TraceLength / SuspensionsUpdate.SpringLengthArray[Index], WheelsSetup.MaxLatGrip);

	if (EngineUpdate.bBraking) {
		EngineUpdate.CurrentPower = EngineUpdate.Throttle * EngineSetup.BrakeForce*GripMultiplier;
	}
	else {
		EngineUpdate.CurrentPower = EngineUpdate.Throttle * EngineUpdate.EnginePower*GripMultiplier;
	}


	//tire smoke
	if (abs(int32(EngineUpdate.CurrentPower)) >= WheelsSetup.LongSlipThreshold * 2) {
		WheelsUpdate.bIsSliding[Index] = true;
	}

	float ActualPower = FMath::Clamp(EngineUpdate.CurrentPower, -WheelsSetup.LongSlipThreshold, WheelsSetup.LongSlipThreshold);
	//test wasted power
	EngineUpdate.WastedPower = FMath::Abs(EngineUpdate.CurrentPower) - FMath::Abs(ActualPower);

	//TempVec += WheelForward * ActualPower;
	TempVec += WheelForward * ActualPower;

	return TempVec;

}

//lateral grip from the tire
FVector ASimpleCar::AddLatGrip(float DeltaTime, FBodyInstance* BodyInstance, FVector Loc, FVector Dir, int32 Index) {

	FVector TempVel = BodyInstance->GetUnrealWorldVelocityAtPoint(Loc);
	FTransform BodyTransform = BodyInstance->GetUnrealWorldTransform();
	FVector BodyLocation = BodyTransform.GetLocation();

	FVector BodyRightVector = BodyTransform.GetUnitAxis(EAxis::Y);
	FVector BodyUpVector = BodyTransform.GetUnitAxis(EAxis::Z);

	FVector WheelRight = BodyRightVector.RotateAngleAxis(WheelsUpdate.CurrentAngle[Index], BodyUpVector);

	float SideSpeed = FVector::DotProduct(WheelRight, TempVel) * WheelsSetup.Grip;

	//tire smoke
	if (abs(int32(SideSpeed)) >= WheelsSetup.SlipThreshold * WheelsSetup.SmokeKickIn) {
		WheelsUpdate.bIsSliding[Index] = true;
	}

	//clamp it, we dont want infinite grip
	float ActualGrip = FMath::Clamp(SideSpeed, -WheelsSetup.SlipThreshold, WheelsSetup.SlipThreshold);

	FVector TempVec = -WheelRight * ActualGrip;

	return TempVec;
}

float ASimpleCar::GetPowerToWheels(float DeltaTime, FBodyInstance* BodyInstance)
{
	//get velocity
	FVector TempVel = BodyInstance->GetUnrealWorldVelocity();
	//	get forward
	FTransform BodyTransform = BodyInstance->GetUnrealWorldTransform();
	FVector BodyForwardVector = BodyTransform.GetUnitAxis(EAxis::X);
	//get forward speed
	float ForwardSpeed = FVector::DotProduct(BodyForwardVector, TempVel);

	//get wheel rpm
	WheelsUpdate.WheelRPM = ((60 * FMath::Abs(ForwardSpeed)) / (WheelsSetup.Radius * 2 * PI));
	//get engine rpm
	EngineUpdate.EngineRPM = WheelsUpdate.WheelRPM * EngineSetup.Gears[EngineUpdate.CurrentGear] * EngineSetup.FinalGearRatio;

	//limit revs, and/or change gear
	if (EngineUpdate.EngineRPM < EngineSetup.GearDownRPM) {
		if (EngineUpdate.CurrentGear > 1 && EngineSetup.bAutomaticGears) {
			EngineUpdate.CurrentGear--;
		}
		else {
			EngineUpdate.EngineRPM = EngineSetup.EngineIdleRPM;
		}

	}

	if (EngineUpdate.EngineRPM > EngineSetup.GearUpRPM) {
		if (EngineUpdate.CurrentGear < (EngineSetup.Gears.Num() - 1) && EngineSetup.bAutomaticGears && ForwardSpeed > 0.0f) {//Gears.Num()
			EngineUpdate.CurrentGear++;
		}
		else {
			EngineUpdate.EngineRPM = EngineSetup.EngineMaxRPM;
		}

	}

	//get power from torque curve
	if (EngineSetup.TorqueCurve)
		EngineUpdate.AvailablePower = EngineSetup.TorqueCurve->GetFloatValue(EngineUpdate.EngineRPM);
	//go through the gearbox
	if (WheelsSetup.Radius!=0.f)
	EngineUpdate.EnginePower = EngineUpdate.AvailablePower * EngineSetup.Gears[EngineUpdate.CurrentGear] * EngineSetup.FinalGearRatio * 0.7 / (WheelsSetup.Radius*0.1);
	//newton/unreal conversion
	EngineUpdate.EnginePower *= 100;

	if (EngineUpdate.EngineRPM > EngineSetup.RedLineRPM) {
		EngineUpdate.EnginePower = 0;
	}

	return 	EngineUpdate.EnginePower;
}

/** Returns Mesh subobject **/
USkeletalMeshComponent* ASimpleCar::GetMesh() const { return Mesh; }


