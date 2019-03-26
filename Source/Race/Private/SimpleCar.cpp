#include "SimpleCar.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

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
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	SpringArm->TargetArmLength = 750.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepWorldTransform, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	//arrows used as trace start locations
	SpringTopLocation.Add(FVector(120.0f, 90.0f, 20.0f));
	SpringTopLocation.Add(FVector(120.0f, -90.0f, 20.0f));
	SpringTopLocation.Add(FVector(-120.0f, 90.0f, 20.0f));
	SpringTopLocation.Add(FVector(-120.0f, -90.0f, 20.0f));

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
		if (WheelMesh)
			it->SetStaticMesh(WheelMesh);
		it->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		it->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	}

	//engine sound
	EngineAC = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineAudio"));
	//EngineAC->SetSound(EngineSound);
	EngineAC->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);

	WheelCenterLocation.Init(FVector(0.0f, 0.0f, 0.0f), 4);
	TireHitLocation.Init(FVector(0.0f, 0.0f, 0.0f), 4);
	SpringForceArray.Init(FVector(0.0f, 0.0f, 0.0f), 4);
	TireForceArray.Init(FVector(0.0f, 0.0f, 0.0f), 4);
	WheelForwardArray.Init(FVector(0.0f, 0.0f, 0.0f), 4);
	WheelRightArray.Init(FVector(0.0f, 0.0f, 0.0f), 4);
	SuspForceLocation.Init(FVector(0.0f, 0.0f, 0.0f), 4);

	SpringLengthArray.Init(0.0f, 4);

	PreviousPosition.Init(0.0f, 4);

	bOnGround.Init(false, 4);
	bIsPowered.Init(true, 4);
	bIsSliding.Init(false, 4);

	CurrentAngle.Init(0.0f, 4);

	CurrentWheelPitch.Init(0.0f, 4);

	//gear ratios
	Gears.Emplace(-2.90);//reverse
	Gears.Emplace(2.66);//1st
	Gears.Emplace(1.78);
	Gears.Emplace(1.30);
	Gears.Emplace(1.0);
	Gears.Emplace(0.74);//5th


	// Bind function delegate
	OnCalculateCustomPhysics.BindUObject(this, &ASimpleCar::CustomPhysics);

}

// Called when the game starts or when spawned
void ASimpleCar::BeginPlay()
{
	Super::BeginPlay();

	if (EngineAC)
	{
		EngineAC->SetSound(EngineSound);
		EngineAC->Play();
	}

	if (GetMesh() != NULL) {
		MainBodyInstance = GetMesh()->GetBodyInstance();
	}

	if (WheelMesh)
	{
		Wheel0->SetStaticMesh(WheelMesh);
		Wheel1->SetStaticMesh(WheelMesh);
		Wheel2->SetStaticMesh(WheelMesh);
		Wheel3->SetStaticMesh(WheelMesh);
	}
}

// Called every frame
void ASimpleCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WheelArray.Num() == 0) { return; }

	//work out wheel center locations from visiblemesh instead of physics body to avoid wheel lag
	FTransform BodyTransform = GetMesh()->GetComponentTransform();
	FVector BodyLocation = BodyTransform.GetLocation();

	FVector BodyForwardVector = BodyTransform.GetUnitAxis(EAxis::X);
	FVector BodyRightVector = BodyTransform.GetUnitAxis(EAxis::Y);
	FVector BodyUpVector = BodyTransform.GetUnitAxis(EAxis::Z);

	for (int32 b = 0; b < WheelArray.Num(); b++)
	{
		if (!WheelCenterLocation.IsValidIndex(b) || !SpringLengthArray.IsValidIndex(b) || !CurrentWheelPitch.IsValidIndex(b) || !WheelArray.IsValidIndex(b) || !bIsSliding.IsValidIndex(b))
			break;
		// Calculate suspension location
		FVector SuspensionLocation = BodyLocation + BodyForwardVector * SpringTopLocation[b].X + BodyRightVector * SpringTopLocation[b].Y + BodyUpVector * SpringTopLocation[b].Z;
		FVector TempVel = MainBodyInstance->GetUnrealWorldVelocityAtPoint(SuspensionLocation);

		// Set wheel location
		if(WheelCenterLocation.IsValidIndex(b) && SpringLengthArray.IsValidIndex(b))
			WheelCenterLocation[b] = SuspensionLocation - (BodyUpVector * (SpringLengthArray[b] - Radius));

		// Calculate wheel forward vector
		FVector WheelForward = BodyForwardVector.RotateAngleAxis(CurrentAngle[b], BodyUpVector);

		// Calculate wheel speed along the wheel forward direction
		float ForwardSpeed = FVector::DotProduct(WheelForward, TempVel);

		// Calculate delta rotation for this wheel
		DeltaPitch = -ForwardSpeed * RotationAmount*DeltaTime;

		// Apply wheel delta rotation to the current rotation
		CurrentWheelPitch[b] -= DeltaPitch;

		if (WheelArray[b] != NULL) {
			//rotate/locate wheel meshes
			WheelArray[b]->SetWorldLocation(WheelCenterLocation[b]);
			WheelArray[b]->SetRelativeRotation(FRotator(CurrentWheelPitch[b], CurrentAngle[b], 0.0f));
		}

		//tire smoke
		if (bIsSliding[b]) {
			//spawn smoke particle
			SpawnSmokeEffect(b);
			UpdateWheelEffects(DeltaTime, b);
			//reset sliding
			bIsSliding[b] = false;
		}
		else {
			if (DustPSC[b]) {
				DustPSC[b]->SetActive(false);
				DustPSC[b]->bAutoDestroy = true;
			}

		}


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
		EnginePitch += ((EngineRPM / EngineMaxRPM)* (EnginePitchMax - EnginePitchIdle)) + EnginePitchIdle;
		EnginePitch = FMath::Clamp(EnginePitch, EnginePitchIdle, EnginePitchMax);
		EngineAC->SetPitchMultiplier(EnginePitch);
	}

}

void ASimpleCar::SpawnSmokeEffect(int WheelIndex)
{
	UParticleSystemComponent* NewParticle = NewObject<UParticleSystemComponent>(this);
	NewParticle->SetWorldLocation(TireHitLocation[WheelIndex]);
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
			DustPSC[Index]->SetWorldLocation(TireHitLocation[Index]);
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
	Throttle = Val;
}

void ASimpleCar::MoveRight(float Val)
{
	//for substep calculations
	CurrentAngle[0] = FMath::Lerp(CurrentAngle[0], SteerAngle * Val, SteerSpeed * GetWorld()->DeltaTimeSeconds);
	CurrentAngle[1] = FMath::Lerp(CurrentAngle[1], SteerAngle * Val, SteerSpeed * GetWorld()->DeltaTimeSeconds);

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
	SpeedKPH = FMath::Abs(FVector::DotProduct(BodyForwardVector, Vel)) * 0.036f;
	SpeedKPH_int = FMath::FloorToInt(SpeedKPH);

	//add air resistance force
	FVector DragForce = -Vel * SpeedKPH * AirResistance;
	BodyInstance->AddImpulseAtPosition(DragForce * DeltaTime, BodyLocation);

	//if we got wheels	
	if (WheelArray.Num() > 0) {

		//trace for each wheel	
		for (int32 Index = 0; Index < WheelArray.Num(); Index++)
		{

			//wheel direction vectors
			FVector WheelForward = BodyForwardVector.RotateAngleAxis(CurrentAngle[Index], BodyUpVector);
			FVector WheelRight = BodyRightVector.RotateAngleAxis(CurrentAngle[Index], BodyUpVector);

			FVector SuspensionLocation = BodyLocation + BodyForwardVector * SpringTopLocation[Index].X + BodyRightVector * SpringTopLocation[Index].Y + BodyUpVector * SpringTopLocation[Index].Z;
			//set location for susp force
			SuspForceLocation[Index] = SuspensionLocation;

			//trace
			FHitResult Hit = Trace(SuspensionLocation, -BodyUpVector);

			if (Hit.bBlockingHit) {

				SpringLocation = Hit.ImpactPoint;

				float SpringPosition = (Hit.Location - SuspensionLocation).Size();
				SpringLengthArray[Index] = SpringPosition;

				// If previously on air, set previous position to current position
				if (!bOnGround[Index]) {
					PreviousPosition[Index] = SpringPosition;
				}
				float DamperVelocity = (SpringPosition - PreviousPosition[Index]) / DeltaTime;
				PreviousPosition[Index] = SpringPosition;
				bOnGround[Index] = true;

				// Calculate spring force
				float SpringForce = (1 - (SpringPosition / TraceLength)) * SpringValue;

				// Apply damper force
				SpringForce -= DamperValue * DamperVelocity;

				FVector TotalForce = Hit.ImpactNormal * FVector::DotProduct(Hit.ImpactNormal, SpringForce * BodyUpVector); //SpringForce * BodyUpVector;//// Hit.ImpactNormal;
				//set total force for this spring
				SpringForceArray[Index] = TotalForce;

				//for force calcs and tire smoke
				TireHitLocation[Index] = SpringLocation;

			}
			else {
				bOnGround[Index] = false;
				SpringLengthArray[Index] = TraceLength;

			}

			//grip and drive
			if (bOnGround[Index]) {
				//multiply grip by spring length, compressed = more grip ...
				float GripMultiplier = FMath::Max(TraceLength / SpringLengthArray[Index], MaxGrip);

				//add tire forces
				//lat grip
				FVector TireForces = AddLatGrip(DeltaTime, BodyInstance, SpringLocation, WheelRight, Index);
				//engine/wheel torque
				if (bIsPowered[Index]) {
					TireForces += AddDrive(DeltaTime, BodyInstance, SpringLocation, WheelForward, Index);
				}


				TireForces *= GripMultiplier;
				BodyInstance->AddImpulseAtPosition(TireForces * DeltaTime, SpringLocation);
			}


			//~~~~~~~~~~~~~~~~~~~~~~
		}//end for()



		//add in anti-roll, take a bit of force from the longest spring and add it to the shotrest
		//we can do this with the force because RB local -z will be the same for all wheels
		//anti roll front
		FVector AntiRollForceF = FVector();
		if (bOnGround[0] && bOnGround[1]) {
			AntiRollForceF = (SpringForceArray[0] - SpringForceArray[1]) * AntiRollFront;
			AntiRollForceF.X = FMath::Clamp(AntiRollForceF.X, -MaxSpringValue, MaxSpringValue);
			AntiRollForceF.Y = FMath::Clamp(AntiRollForceF.Y, -MaxSpringValue, MaxSpringValue);
			AntiRollForceF.Z = FMath::Clamp(AntiRollForceF.Z, -MaxSpringValue, MaxSpringValue);
		}

		//anti roll back
		FVector AntiRollForceB = FVector();
		if (bOnGround[2] && bOnGround[3]) {
			AntiRollForceB = (SpringForceArray[2] - SpringForceArray[3]) * AntiRollBack;
			AntiRollForceB.X = FMath::Clamp(AntiRollForceB.X, -MaxSpringValue, MaxSpringValue);
			AntiRollForceB.Y = FMath::Clamp(AntiRollForceB.Y, -MaxSpringValue, MaxSpringValue);
			AntiRollForceB.Z = FMath::Clamp(AntiRollForceB.Z, -MaxSpringValue, MaxSpringValue);
		}

		//suspension springs
		//front
		if (bOnGround[0]) {
			SpringForceArray[0] += AntiRollForceF;
			BodyInstance->AddImpulseAtPosition(SpringForceArray[0] * DeltaTime, SuspForceLocation[0]);
		}
		if (bOnGround[1]) {
			SpringForceArray[1] -= AntiRollForceF;
			BodyInstance->AddImpulseAtPosition(SpringForceArray[1] * DeltaTime, SuspForceLocation[1]);
		}
		//back
		if (bOnGround[2]) {
			SpringForceArray[2] += AntiRollForceB;
			BodyInstance->AddImpulseAtPosition(SpringForceArray[2] * DeltaTime, SuspForceLocation[2]);
		}
		if (bOnGround[3]) {
			SpringForceArray[3] -= AntiRollForceB;
			BodyInstance->AddImpulseAtPosition(SpringForceArray[3] * DeltaTime, SuspForceLocation[3]);
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
	FVector TraceEnd = TraceStart + (TraceDirection * TraceLength);

	GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjectParams, TraceParams);

	return Hit;
}

void ASimpleCar::ChangeUp()
{
	if (CurrentGear < (Gears.Num() - 1)) {
		CurrentGear++;
	}
}

void ASimpleCar::ChangeDown()
{
	if (CurrentGear > 0) {
		CurrentGear--;
	}
}

//force to wheels from 'engine'
FVector ASimpleCar::AddDrive(float DeltaTime, FBodyInstance* BodyInstance, FVector Loc, FVector Dir, int32 Index) {

	FVector TempVel = BodyInstance->GetUnrealWorldVelocityAtPoint(Loc);
	FTransform BodyTransform = BodyInstance->GetUnrealWorldTransform();
	FVector BodyLocation = BodyTransform.GetLocation();

	FVector BodyForwardVector = BodyTransform.GetUnitAxis(EAxis::X);
	FVector BodyUpVector = BodyTransform.GetUnitAxis(EAxis::Z);

	FVector WheelForward = BodyForwardVector.RotateAngleAxis(CurrentAngle[Index], BodyUpVector);



	//reverse
	if (FVector::DotProduct(WheelForward, TempVel) < 10.0f) {
		bInReverse = true;
	}
	else {
		bInReverse = false;
	}

	//braking
	if (Throttle > 0.0) {
		if (bInReverse) {
			bBraking = true;
		}
		else {
			bBraking = false;
		}
	}
	else if (Throttle < 0.0) {
		if (bInReverse) {
			bBraking = false;
		}
		else {
			bBraking = true;
		}
	}
	else {
		bBraking = false;
	}


	//lag or slowdown from mechanical/friction forces
	float ForwardSpeed = FVector::DotProduct(WheelForward, TempVel);

	FVector TempVec = -WheelForward * ForwardSpeed * EngineBrake;

	EnginePower = GetPowerToWheels(DeltaTime, BodyInstance);

	//final power output 
	//multiply grip by spring length, compressed = more grip ...
	float GripMultiplier = FMath::Max(TraceLength / SpringLengthArray[Index], MaxLatGrip);

	if (bBraking) {
		CurrentPower = Throttle * BrakeForce*GripMultiplier;// ;
	}
	else {
		CurrentPower = Throttle * EnginePower*GripMultiplier;// ;
	}


	//tire smoke
	if (abs(int32(CurrentPower)) >= LongSlipThreshold * 2) {
		bIsSliding[Index] = true;
	}

	float ActualPower = FMath::Clamp(CurrentPower, -LongSlipThreshold, LongSlipThreshold);
	//test wasted power
	WastedPower = FMath::Abs(CurrentPower) - FMath::Abs(ActualPower);

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

	FVector WheelRight = BodyRightVector.RotateAngleAxis(CurrentAngle[Index], BodyUpVector);

	float SideSpeed = FVector::DotProduct(WheelRight, TempVel) * Grip;

	//tire smoke
	if (abs(int32(SideSpeed)) >= SlipThreshold * SmokeKickIn) {
		bIsSliding[Index] = true;
	}

	//clamp it, we dont want infinite grip
	float ActualGrip = FMath::Clamp(SideSpeed, -SlipThreshold, SlipThreshold);

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
	WheelRPM = ((60 * FMath::Abs(ForwardSpeed)) / (Radius * 2 * PI));
	//get engine rpm
	EngineRPM = WheelRPM * Gears[CurrentGear] * FinalGearRatio;

	//limit revs, and/or change gear
	if (EngineRPM < GearDownRPM) {
		if (CurrentGear > 1 && bAutomaticGears) {
			CurrentGear--;
		}
		else {
			EngineRPM = EngineIdleRPM;
		}

	}

	if (EngineRPM > GearUpRPM) {
		if (CurrentGear < (Gears.Num() - 1) && bAutomaticGears && ForwardSpeed > 0.0f) {//Gears.Num()
			CurrentGear++;
		}
		else {
			EngineRPM = EngineMaxRPM;
		}

	}

	//get power from torque curve
	AvailablePower = TorqueCurve->GetFloatValue(EngineRPM);
	//go through the gearbox
	EnginePower = AvailablePower * Gears[CurrentGear] * FinalGearRatio * 0.7 / (Radius*0.1);
	//newton/unreal conversion
	EnginePower *= 100;

	if (EngineRPM > RedLineRPM) {
		EnginePower = 0;
	}

	return EnginePower;
}

/** Returns Mesh subobject **/
USkeletalMeshComponent* ASimpleCar::GetMesh() const { return Mesh; }


