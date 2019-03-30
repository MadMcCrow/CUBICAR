#include "SimpleCar.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "UnrealNetwork.h"
#include "Race.h"
#include "Kismet/KismetMathLibrary.h"

#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif //WITH_EDITOR


FName ASimpleCar::VehicleMeshComponentName(TEXT("VehicleMesh"));

// Sets default values
ASimpleCar::ASimpleCar()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(VehicleMeshComponentName);
	RootComponent = Mesh;

	bReplicateMovement = true;
	bReplicates = true;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationCustomMode);
	GetMesh()->SetDisablePostProcessBlueprint(true);

	//GetMesh()->SetVisibility(false , false);
	GetMesh()->SetCollisionProfileName(UCollisionProfile::Vehicle_ProfileName);
	GetMesh()->BodyInstance.bSimulatePhysics = true;
	GetMesh()->BodyInstance.bNotifyRigidBodyCollision = true;
	GetMesh()->BodyInstance.bUseCCD = true;
	GetMesh()->bBlendPhysics = true;
	GetMesh()->SetGenerateOverlapEvents(true);

	GetMesh()->SetCenterOfMass(FVector(0.f, 0.f, -29.3532f));

	//GetMesh()->SetCanEverAffectNavigation(false);

	//camera stuff //
	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 50.f);
	SpringArm->SetRelativeRotation(FRotator(-12.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 750.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	// trace start locations
	//SpringTopLocation.Add(Suspensions.GetTopLocation(true, false));//(FVector(120.0f, 90.0f, 20.0f));
	//SpringTopLocation.Add(Suspensions.GetTopLocation(true, true));//(FVector(120.0f, -90.0f, 20.0f));
	//SpringTopLocation.Add(Suspensions.GetTopLocation(false, false));//(FVector(-120.0f, 90.0f, 20.0f));
	//SpringTopLocation.Add(Suspensions.GetTopLocation(false, true));//(FVector(-120.0f, -90.0f, 20.0f));

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
		it->SetupAttachment(RootComponent); //  FAttachmentTransformRules::KeepWorldTransform);
	}

	//engine sound
	EngineAC = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineAudio"));
	//EngineAC->SetSound(EngineSound);
	EngineAC->SetupAttachment(GetMesh());

	//WheelCenterLocation.Init(FVector(0.0f, 0.0f, 0.0f), 4);
	//TireHitLocation.Init(FVector(0.0f, 0.0f, 0.0f), 4);
	SpringForceArray.Init(FVector(0.0f, 0.0f, 0.0f), 4);
	SuspForceLocation.Init(FVector(0.0f, 0.0f, 0.0f), 4);

	SpringLengthArray.Init(0.0f, 4);

	PreviousPosition.Init(0.0f, 4);

	bOnGround.Init(false, 4);
	//bIsSliding.Init(false, 4);

	//CurrentAngle.Init(0.0f, 4);

	//CurrentWheelPitch.Init(0.0f, 4);

	//gear ratios
	Gears.Emplace(-2.90);//reverse
	Gears.Emplace(2.66);//1st
	Gears.Emplace(1.78);
	Gears.Emplace(1.30);
	Gears.Emplace(1.0);
	Gears.Emplace(0.74);//5th

	Suspensions.Init();
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

	UpdateWheels();
	UpdateSuspensions();
}

// Called every frame
void ASimpleCar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (Role != ROLE_SimulatedProxy)
		UpdateCar(DeltaSeconds);
	//Multicast_UpdateEffects(DeltaTime);
/*
	if (Role == ROLE_AutonomousProxy)
	{
		CarTransform = GetActorTransform();
		//UE_LOG(LogTemp, Warning, TEXT("%s  : Role AutonomousProxy"), *GetActorTransform().GetLocation().ToString());
		Server_UpdateTransform(GetActorTransform());
	}
	*/
}

void ASimpleCar::UpdateCar(float DeltaSeconds)
{
	if (WheelArray.Num() == 0)
	{
		return;
		UE_LOG(LogRace, Error, TEXT("No Wheels"));
	}

	//work out wheel center locations from visiblemesh instead of physics body to avoid wheel lag
	FTransform BodyTransform = GetMesh()->GetComponentTransform();
	FVector BodyLocation = BodyTransform.GetLocation();

	FVector BodyForwardVector = BodyTransform.GetUnitAxis(EAxis::X);
	FVector BodyRightVector = BodyTransform.GetUnitAxis(EAxis::Y);
	FVector BodyUpVector = BodyTransform.GetUnitAxis(EAxis::Z);

	for (int32 b = 0; b < WheelArray.Num(); b++)
	{

		if (/*!WheelCenterLocation.IsValidIndex(b) || */ !SpringLengthArray.IsValidIndex(b) || /*!CurrentWheelPitch.
			IsValidIndex(b) ||*/ !WheelArray.IsValidIndex(b) || /*!bIsSliding.IsValidIndex(b) ||*/ !Suspensions.SpringTopLocation.
			IsValidIndex(b))
		{
			UE_LOG(LogRace, Error, TEXT("At least one wheel array is invalid"));
			break;
		}

		const auto SpringTop = Suspensions.SpringTopLocation[b];
		// Calculate suspension location
		FVector SuspensionLocation = BodyLocation + BodyForwardVector * SpringTop.X + BodyRightVector * SpringTop.Y + BodyUpVector * SpringTop.Z;

		FVector TempVel;
		if (MainBodyInstance)
			TempVel = MainBodyInstance->GetUnrealWorldVelocityAtPoint(SuspensionLocation);

		// Set wheel location
		if (/*WheelCenterLocation.IsValidIndex(b) &&*/ SpringLengthArray.IsValidIndex(b))
			WheelsUpdate.WheelCenterLocation[b] = SuspensionLocation - (BodyUpVector * (SpringLengthArray[b] - WheelSetup.Radius));

		// Calculate wheel forward vector
		FVector WheelForward = BodyForwardVector.RotateAngleAxis(WheelsUpdate.CurrentAngle[b], BodyUpVector);

		// Calculate wheel speed along the wheel forward direction
		float ForwardSpeed = FVector::DotProduct(WheelForward, TempVel);

		// Calculate delta rotation for this wheel
		WheelsUpdate.DeltaPitch = -ForwardSpeed * WheelSetup.RotationAmount*DeltaSeconds;

		// Apply wheel delta rotation to the current rotation
		WheelsUpdate.CurrentWheelPitch[b] -= WheelsUpdate.DeltaPitch;

		if (WheelArray[b] != NULL) {
			//rotate/locate wheel meshes
			WheelArray[b]->SetWorldLocation(WheelsUpdate.WheelCenterLocation[b]);
			WheelArray[b]->SetRelativeRotation(FRotator(WheelsUpdate.CurrentWheelPitch[b], WheelsUpdate.CurrentAngle[b], 0.0f));
		}

		//tire smoke
		if (WheelsUpdate.bIsSliding[b]) {
			//spawn smoke particle
			SpawnSmokeEffect(b);
			UpdateWheelEffects(DeltaSeconds, b);
			//reset sliding
			WheelsUpdate.bIsSliding[b] = false;
		}
		else {
			if (DustPSC[b]) {
				DustPSC[b]->SetActive(false);
				DustPSC[b]->bAutoDestroy = true;
			}

		}

#if WITH_EDITOR
		if (bDrawDebug)
		{
			DrawDebugLine(
				GetWorld(),
				SuspensionLocation,
				WheelsUpdate.WheelCenterLocation[b],
				FColor(255, 0, 0),
				false, -1, 0,
				12.333
			);
		}
#endif // WITH_EDITOR

	}


	// Add custom physics on MainBodyMesh
	if (MainBodyInstance) {
		MainBodyInstance->AddCustomPhysics(OnCalculateCustomPhysics);
	}
	else
	{
		UE_LOG(LogRace, Error, TEXT("This mesh does not have any Body instance"))
	}

	//engine sound pitch
	if (EngineAC)
	{
		float EnginePitch = 0.0f;
		EnginePitch += ((EngineUpdate.EngineRPM / EngineMaxRPM)* (EnginePitchMax - EnginePitchIdle)) + EnginePitchIdle;
		EnginePitch = FMath::Clamp(EnginePitch, EnginePitchIdle, EnginePitchMax);
		EngineAC->SetPitchMultiplier(EnginePitch);
	}
}
#if 0
bool ASimpleCar::Server_UpdateTransform_Validate(FTransform NewTransform) { return true; } // Todo : ADD SOME VALIDATION CODE, Check for too big of an error 
void ASimpleCar::Server_UpdateTransform_Implementation(FTransform NewTransform)
{
	CarTransform = NewTransform;
	if(Role == ROLE_Authority)
	{
		Multicast_UpdateTransform(CarTransform);
		//SetActorTransform(CarTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

bool ASimpleCar::Multicast_UpdateTransform_Validate(FTransform NewTransform) { return true; }
void ASimpleCar::Multicast_UpdateTransform_Implementation(FTransform NewTransform)
{
	if (Role == ROLE_SimulatedProxy)
	{
		UE_LOG(LogTemp, Warning, TEXT("Multicasted"));

		const auto prev = GetActorTransform();
		const auto target = UKismetMathLibrary::TInterpTo(prev, CarTransform, GetWorld()->GetDeltaSeconds(), 10);
		SetActorTransform(target, false, nullptr, ETeleportType::TeleportPhysics);
	}
}
#endif

bool ASimpleCar::Multicast_UpdateEffects_Validate(float DeltaTime) { return true; }
void ASimpleCar::Multicast_UpdateEffects_Implementation(float DeltaTime)
{

	

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
	if (Role != ROLE_Authority)
		Server_UpdateThrottle(Val);
	else
		Server_UpdateThrottle_Implementation(Val);

}

void ASimpleCar::MoveRight(float Val)
{
	//WheelsUpdate.CurrentAngle[0] = FMath::Lerp(WheelsUpdate.CurrentAngle[0], SteerAngle * Val, SteerSpeed * GetWorld()->DeltaTimeSeconds);
	//WheelsUpdate.CurrentAngle[1] = FMath::Lerp(WheelsUpdate.CurrentAngle[1], SteerAngle * Val, SteerSpeed * GetWorld()->DeltaTimeSeconds);
	
	if (Role != ROLE_Authority)
		Server_UpdateWheelAngle(Val);
	else
		Server_UpdateWheelAngle_Implementation(Val);
	
}


UStaticMesh* ASimpleCar::GetWheelMesh()
{
	return WheelMesh;
}

void ASimpleCar::UpdateWheels()
{
	if (Role == ROLE_Authority)
		Multicast_UpdateWheelsMesh();
}

bool ASimpleCar::Multicast_UpdateWheelsMesh_Validate() { return true; }
void ASimpleCar::Multicast_UpdateWheelsMesh_Implementation()
{
	UStaticMesh * W = GetWheelMesh();
	if (W && Wheel0 && Wheel1 && Wheel2 && Wheel3)
	{
		Wheel0->SetStaticMesh(W);
		Wheel1->SetStaticMesh(W);
		Wheel2->SetStaticMesh(W);
		Wheel3->SetStaticMesh(W);
	}
}

void ASimpleCar::UpdateSuspensions()
{
	if(Role == ROLE_AutonomousProxy)
		Server_UpdateSuspensions();
	if (Role == ROLE_Authority)
		Server_UpdateSuspensions_Implementation();
}

bool ASimpleCar::Server_UpdateSuspensions_Validate() { return true; }
void ASimpleCar::Server_UpdateSuspensions_Implementation()
{
	Suspensions.Init();
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
	const auto Speed = FMath::Abs(FVector::DotProduct(BodyForwardVector, Vel)) * 0.036f;
	DashboardInfo.Speed_KPH = Speed;
	//add air resistance force
	FVector DragForce = -Vel * Speed * AirResistance;
	BodyInstance->AddImpulseAtPosition(DragForce * DeltaTime, BodyLocation);

	//if we got wheels	
	if (WheelArray.Num() > 0) {

		//trace for each wheel	
		for (int32 Index = 0; Index < WheelArray.Num(); Index++)
		{

			if (/*!WheelCenterLocation.IsValidIndex(Index) ||*/ !SpringLengthArray.IsValidIndex(Index) || /*!CurrentWheelPitch.
				IsValidIndex(Index) ||*/ !WheelArray.IsValidIndex(Index) || /*!bIsSliding.IsValidIndex(Index)||*/ !Suspensions.SpringTopLocation.
				IsValidIndex(Index))
			{
				UE_LOG(LogRace, Error, TEXT("At least one wheel array is invalid"));
				break;
			}
			//wheel direction vectors
			FVector WheelForward = BodyForwardVector.RotateAngleAxis(WheelsUpdate.CurrentAngle[Index], BodyUpVector);
			FVector WheelRight = BodyRightVector.RotateAngleAxis(WheelsUpdate.CurrentAngle[Index], BodyUpVector);
			const auto SpringTop = Suspensions.SpringTopLocation[Index];
			FVector SuspensionLocation = BodyLocation + BodyForwardVector * SpringTop.X + BodyRightVector * SpringTop.Y + BodyUpVector * (SpringTop.Z);
			//set location for susp force
			SuspForceLocation[Index] = SuspensionLocation;

			//trace
			FHitResult Hit = Trace(SuspensionLocation + FVector(0.f, 0.f, 0.f), -BodyUpVector);

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
				float SpringForce = (1 - (SpringPosition / Suspensions.TraceLength)) * Suspensions.SpringValue;

				// Apply damper force
				SpringForce -= Suspensions.DamperValue * DamperVelocity;

				FVector TotalForce = Hit.ImpactNormal * FVector::DotProduct(Hit.ImpactNormal, SpringForce * BodyUpVector); //SpringForce * BodyUpVector;//// Hit.ImpactNormal;
				//set total force for this spring
				SpringForceArray[Index] = TotalForce;

				//for force calcs and tire smoke
				WheelsUpdate.TireHitLocation[Index] = SpringLocation;

			}
			else {
				bOnGround[Index] = false;
				SpringLengthArray[Index] = Suspensions.TraceLength;

			}

			//grip and drive
			if (bOnGround[Index]) {
				//multiply grip by spring length, compressed = more grip ...
				float GripMultiplier = FMath::Max(Suspensions.TraceLength / SpringLengthArray[Index], WheelSetup.MaxGrip);

				//add tire forces
				//lat grip
				FVector TireForces = AddLatGrip(DeltaTime, BodyInstance, SpringLocation, WheelRight, Index);
				//engine/wheel torque
				if (WheelSetup.bIsPowered[Index]) {
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
			AntiRollForceF = (SpringForceArray[0] - SpringForceArray[1]) * Suspensions.AntiRollFront;
			AntiRollForceF.X = FMath::Clamp(AntiRollForceF.X, -Suspensions.MaxSpringValue, Suspensions.MaxSpringValue);
			AntiRollForceF.Y = FMath::Clamp(AntiRollForceF.Y, -Suspensions.MaxSpringValue, Suspensions.MaxSpringValue);
			AntiRollForceF.Z = FMath::Clamp(AntiRollForceF.Z, -Suspensions.MaxSpringValue, Suspensions.MaxSpringValue);
		}

		//anti roll back
		FVector AntiRollForceB = FVector();
		if (bOnGround[2] && bOnGround[3]) {
			AntiRollForceB = (SpringForceArray[2] - SpringForceArray[3]) * Suspensions.AntiRollBack;
			AntiRollForceB.X = FMath::Clamp(AntiRollForceB.X, -Suspensions.MaxSpringValue, Suspensions.MaxSpringValue);
			AntiRollForceB.Y = FMath::Clamp(AntiRollForceB.Y, -Suspensions.MaxSpringValue, Suspensions.MaxSpringValue);
			AntiRollForceB.Z = FMath::Clamp(AntiRollForceB.Z, -Suspensions.MaxSpringValue, Suspensions.MaxSpringValue);
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
	FVector TraceEnd = TraceStart + (TraceDirection * Suspensions.TraceLength);

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

	FVector TempVec = -WheelForward * ForwardSpeed * EngineBrake;

	EnginePower = GetPowerToWheels(DeltaTime, BodyInstance);

	//final power output 
	//multiply grip by spring length, compressed = more grip ...
	float GripMultiplier = FMath::Max(Suspensions.TraceLength / SpringLengthArray[Index], WheelSetup.MaxLatGrip);

	if (EngineUpdate.bBraking) {
		EngineUpdate.CurrentPower = EngineUpdate.Throttle * BrakeForce*GripMultiplier;// ;
	}
	else {
		EngineUpdate.CurrentPower = EngineUpdate.Throttle * EnginePower*GripMultiplier;// ;
	}


	//tire smoke
	if (abs(int32(EngineUpdate.CurrentPower)) >= WheelSetup.LongSlipThreshold * 2) {
		WheelsUpdate.bIsSliding[Index] = true;
	}

	float ActualPower = FMath::Clamp(EngineUpdate.CurrentPower, -WheelSetup.LongSlipThreshold, WheelSetup.LongSlipThreshold);
	//test wasted power
	WastedPower = FMath::Abs(EngineUpdate.CurrentPower) - FMath::Abs(ActualPower);

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

	float SideSpeed = FVector::DotProduct(WheelRight, TempVel) * WheelSetup.Grip;

	//tire smoke
	if (abs(int32(SideSpeed)) >= WheelSetup.SlipThreshold * WheelSetup.SmokeKickIn) {
		WheelsUpdate.bIsSliding[Index] = true;
	}

	//clamp it, we dont want infinite grip
	float ActualGrip = FMath::Clamp(SideSpeed, -WheelSetup.SlipThreshold, WheelSetup.SlipThreshold);

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
	WheelRPM = ((60 * FMath::Abs(ForwardSpeed)) / (WheelSetup.Radius * 2 * PI));
	//get engine rpm
	EngineUpdate.EngineRPM = WheelRPM * Gears[CurrentGear] * FinalGearRatio;

	//limit revs, and/or change gear
	if (EngineUpdate.EngineRPM < GearDownRPM) {
		if (CurrentGear > 1 && bAutomaticGears) {
			CurrentGear--;
		}
		else {
			EngineUpdate.EngineRPM = EngineIdleRPM;
		}

	}

	if (EngineUpdate.EngineRPM > GearUpRPM) {
		if (CurrentGear < (Gears.Num() - 1) && bAutomaticGears && ForwardSpeed > 0.0f) {//Gears.Num()
			CurrentGear++;
		}
		else {
			EngineUpdate.EngineRPM = EngineMaxRPM;
		}

	}

	//get power from torque curve
	if(TorqueCurve)
		EngineUpdate.AvailablePower = TorqueCurve->GetFloatValue(EngineUpdate.EngineRPM);
	//go through the gearbox

	if (WheelSetup.Radius == 0) // save yourself from zero division 
		WheelSetup.Radius = 32;
	if (Gears.IsValidIndex(CurrentGear))
		EnginePower = EngineUpdate.AvailablePower * Gears[CurrentGear] * FinalGearRatio * 0.7 / (WheelSetup.Radius*0.1);
	else
		EnginePower = 0;


	//newton/unreal conversion
	EnginePower *= 100;

	if (EngineUpdate.EngineRPM > RedLineRPM) {
		EnginePower = 0;
	}

	return EnginePower;
}

bool ASimpleCar::Server_UpdateThrottle_Validate(float Throttle) { return true; }
void ASimpleCar::Server_UpdateThrottle_Implementation(float Throttle)
{
	EngineUpdate.Throttle = Throttle;
}

bool ASimpleCar::Server_UpdateWheelAngle_Validate(float Val) { return true; }
void ASimpleCar::Server_UpdateWheelAngle_Implementation(float Val)
{
	//for substep calculations
	WheelsUpdate.CurrentAngle[0] = FMath::Lerp(WheelsUpdate.CurrentAngle[0], SteerAngle * Val, SteerSpeed * GetWorld()->DeltaTimeSeconds);
	WheelsUpdate.CurrentAngle[1] = FMath::Lerp(WheelsUpdate.CurrentAngle[1], SteerAngle * Val, SteerSpeed * GetWorld()->DeltaTimeSeconds);
}

void ASimpleCar::GetSliding(TArray<bool> &Out) const
{
	Out.Empty();
	for (uint8 w = 0; w<=4; w++  )
	{
		Out.Add((bool)WheelsUpdate.bIsSliding[w]);
	}
}

/** Returns Mesh subobject **/
USkeletalMeshComponent* ASimpleCar::GetMesh() const { return Mesh; }



void ASimpleCar::GetLifetimeReplicatedProps(class TArray<class FLifetimeProperty, class FDefaultAllocator> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASimpleCar, DashboardInfo); 
	DOREPLIFETIME(ASimpleCar, WheelMesh);
	DOREPLIFETIME(ASimpleCar, WheelsUpdate);
	DOREPLIFETIME_CONDITION(ASimpleCar, WheelSetup, COND_SkipOwner);
	//DOREPLIFETIME(ASimpleCar, TireHitLocation); 
	DOREPLIFETIME(ASimpleCar, EngineUpdate);
	DOREPLIFETIME(ASimpleCar, Suspensions);
	//DOREPLIFETIME(ASimpleCar, CarTransform);
	//DOREPLIFETIME_CONDITION(ASimpleCar, CarTransform, COND_SkipOwner);
	

	
}


