// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CarStatics.generated.h"

class USoundCue;
class UCurveFloat;
class UParticleSystem;
/**
 *	@struct FCarDashBoard
 *	Car values showed during game play
 */
USTRUCT()
struct FCarDashBoard
{
	GENERATED_BODY()

protected:

	UPROPERTY()
		float Speed_KPH;

	UPROPERTY()
		float Engine_RPM;

	UPROPERTY()
		int Transmission_Gear;

	UPROPERTY()
		bool bIsInReverse;

	friend class UCarStatics;
	friend class ASimpleCar;

public:

	FCarDashBoard(	float NewSpeed_KPH = 0,	float NewEngine_RPM = 0, int NewTransmissionGear = 0, bool NewbIsInReverse = false) :
		Speed_KPH(NewSpeed_KPH), Engine_RPM(NewEngine_RPM), Transmission_Gear(NewTransmissionGear), bIsInReverse(NewbIsInReverse)
	{
	}
};


/**
 *	@struct FCarEngineUpdate
 *	Car values used during game play, will be updated often
 */
//USTRUCT(BlueprintType, Category = "Engine|Update", meta = (DisplayName = "Car Engine Update"))
USTRUCT()
struct FCarEngineUpdate
{
	GENERATED_BODY()

protected:

	//UPROPERTY(BlueprintReadOnly, Category = "Engine")
	UPROPERTY()
		float Throttle = 0.0f;
	UPROPERTY()
		bool bClutchEngaged = false;
	UPROPERTY()
		float CurrentPower = 0.0f;
	UPROPERTY()
		float AvailablePower = 0.0f;
	UPROPERTY()
		float WastedPower = 0.0f;
	UPROPERTY()
		float EnginePower = 0.0f;
	UPROPERTY()
		int32 CurrentGear = 1;
	UPROPERTY()
		float EngineRPM = 0.0f;
	UPROPERTY()
		bool bBraking = false;
	UPROPERTY()
		bool bInReverse = false;

	friend class UCarStatics;
	friend class ASimpleCar;

};

USTRUCT(BlueprintType, Category = "Engine", meta = (DisplayName = "Car Engine Setup"))
struct FCarEngineSetup
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float EnginePowerScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float EngineIdleRPM = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float EngineMaxRPM = 7000.0f;

	/** Damage zone RPM */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float RedLineRPM = 6000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Engine")
		UCurveFloat* TorqueCurve;

	/** Automatic gear up shifting variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gears")
		float GearUpRPM = 5200.0f;

	/** Automatic gear down shifting variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gears")
		float GearDownRPM = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gears")
	bool bAutomaticGears = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gears")
		TArray<float> Gears;

	/** mechanical/friction braking */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float EngineBrake = 30.0f;

	//brake force
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float BrakeForce = 50000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gears")
		float FinalGearRatio = 3.92;

	/** todo : Set appart in a separate class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aerodynamic")
		float AirResistance = 3.0f;

	friend class UCarStatics;
	friend class ASimpleCar;
};

USTRUCT(BlueprintType, Category = "Sound", meta = (DisplayName = "Car Sound Effect"))
struct FCarEngineSound
{
	GENERATED_BODY()

protected:
	/** engine sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSound")
		USoundCue* EngineSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSound")
		float EnginePitchMax = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSound")
		float EnginePitchIdle = 1.0f;

	friend class UCarStatics;
	friend class ASimpleCar;
};

//USTRUCT(BlueprintType, Category = "Wheels|Update", meta = (DisplayName = "Car Wheels Update"))
USTRUCT()
struct FCarWheelsUpdate
{
	GENERATED_BODY()
protected:

	UPROPERTY()
		TArray<FVector> WheelCenterLocation;
	UPROPERTY()
		TArray<FVector> TireHitLocation;
	UPROPERTY()
		TArray<bool> bIsSliding;
	UPROPERTY()
		TArray<float> CurrentWheelPitch;
	UPROPERTY()
		float DeltaPitch = 0.0f;
	UPROPERTY()
		TArray<float> CurrentAngle;
	UPROPERTY()
		float WheelRPM = 0.0f;

	friend class UCarStatics;
	friend class ASimpleCar;

};

USTRUCT(BlueprintType, Category = "Wheels|Setup", meta = (DisplayName = "Car Wheels Setup"))
struct FCarWheelsSetup
{
	GENERATED_BODY()

protected :
	/**	wheel radius	*/
	UPROPERTY(EditAnywhere, Category = "Wheels")
		float Radius = 32.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		TArray<bool> bIsPowered;

	/**	how fast visual wheels rotate	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		float RotationAmount = -1.0f;

	//for tire smoke ect

	/**	Mimimum Slip Value at wich we're losing grip	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		float SlipThreshold = 140000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		float LongSlipThreshold = 100000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		float SmokeKickIn = 3.0f;

	/**
	 *	BaseGrip
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grip")
		float Grip = 1.0f;

	/**
	 *	maximum weight/grip multiplier, cap for weight transfer formula. 
	 *	1 = no effect, >1 = capped to MaxGrip x default Grip
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grip")
		float MaxGrip = 2.0f;

	/**
	 *	maximum weight/grip multiplier for the lateral grip only, cap for weight transfer formula.
	 *	1 = no effect, >1 = capped to MaxGrip x default Grip
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grip")
		float MaxLatGrip = 2.0f;

	//max steer angle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
		float SteerAngle = 45.0f;
	//lerp speed of wheel steering
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
		float SteerSpeed = 3.0f;

	friend class UCarStatics;
	friend class ASimpleCar;
};



USTRUCT(BlueprintType, Category = "Suspension", meta = (DisplayName = "Springs Configurations") )
struct FSpringsLocation
{
	GENERATED_BODY()
//protected:
	UPROPERTY()
		FVector2D Location;

	UPROPERTY()
		float FrontX;

	UPROPERTY()
		float BackX;

public:
	FVector GetLocation(bool bIsFront, bool bIsLeft)
	{
		const float x = bIsFront ? FrontX : BackX;
		const float sign = bIsLeft ? -1 : 1;
		return FVector(x, Location.X * sign, Location.Y);
	}

	FSpringsLocation(FVector2D NewLocation = FVector2D(), float NewFrontX = 120, float NewBackX = -120) : Location(NewLocation), FrontX(NewFrontX), BackX(NewBackX)
	{
	}
};


//USTRUCT(BlueprintType, Category = "Suspension|Update", meta = (DisplayName = "Car Suspensions Update"))
USTRUCT()
struct FCarSuspensionsUpdate
{

	GENERATED_BODY()
protected:

	UPROPERTY()
		TArray<bool> bOnGround;
	UPROPERTY()
		TArray<float> SpringLengthArray;
	UPROPERTY()
		FVector SpringLocation;
	/**	trace start locations	*/
	UPROPERTY()
		TArray<FVector> SuspForceLocation;
	UPROPERTY()
		TArray<FVector> SpringForceArray;
	UPROPERTY()
		TArray<float> PreviousPosition;

	friend class UCarStatics;
	friend class ASimpleCar;

};

USTRUCT(BlueprintType, Category = "Suspension|Setup", meta = (DisplayName = "Car Suspensions Setup"))
struct FCarSuspensionsSetup
{
	GENERATED_BODY()
protected:
	/**		*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TraceLength = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpringValue = 800000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxSpringValue = 1200000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamperValue = 1000.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSpringsLocation SpringConfiguration;

	/**	Spring base location  determined by */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FVector> SpringTopLocation;

	/**	anti-roll */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anti-roll")
		float AntiRollFront = 0.7f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anti-roll")
		float AntiRollBack = 0.6f;

	friend class UCarStatics;
	friend class ASimpleCar;
};




/**
 * 
 */
UCLASS()
class RACE_API UCarStatics : public UObject //UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public :
	UFUNCTION()
		static float GetSpeed_KPH(const FCarDashBoard &Infos);

	UFUNCTION()
		static int GetSpeed_intKPH(const FCarDashBoard &Infos);

	UFUNCTION()
		static FText GetSpeed_textKPH(const FCarDashBoard &Infos);

	UFUNCTION()
		static float GetEngine_RPM(const FCarDashBoard  &Infos);

	UFUNCTION()
		static int GetEngine_intRPM(const FCarDashBoard  &Infos);

	UFUNCTION()
		static int GetTransmission_intGear(const FCarDashBoard  &Infos);

	UFUNCTION()
		static FText GetTransmission_textGear(const FCarDashBoard  &Infos);

	
};
