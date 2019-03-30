// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CarStatics.generated.h"

class ARaceCar;
struct FCarSuspensionsSetup;
class USoundCue;
class UCurveFloat;
class UParticleSystem;
/**
 *	@struct FCarDashBoard
 *	Car values showed during game play
 */
USTRUCT(BlueprintType)
struct FCarDashBoard
{
	GENERATED_BODY()

private:

	UPROPERTY()
		float Speed_KPH;

	UPROPERTY()
		float Engine_RPM;

	UPROPERTY()
		int Transmission_Gear;

	UPROPERTY()
		bool bIsInReverse;
	

public:

	FCarDashBoard(	float NewSpeed_KPH = 0,	float NewEngine_RPM = 0, int NewTransmissionGear = 0, bool NewbIsInReverse = false) :
		Speed_KPH(NewSpeed_KPH), Engine_RPM(NewEngine_RPM), Transmission_Gear(NewTransmissionGear), bIsInReverse(NewbIsInReverse)
	{
	}

	float	GetSpeed() const		 { return Speed_KPH; }
	float	GetEngineRPM() const	 { return Engine_RPM; }
	int		GetCurrentGear() const	 { return Transmission_Gear; }

private :
	friend class UCarStatics;
	friend class ASimpleCar;

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
public:

	FCarEngineSetup();

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

USTRUCT(BlueprintType, Category = "Wheels", meta = (DisplayName = "Car Wheels Update"))
struct FCarWheelsUpdate
{
	GENERATED_BODY()
private:

	UPROPERTY()
		FVector WheelCenterLocation[4] = { FVector(0.0f) , FVector(0.0f) ,FVector(0.0f) ,FVector(0.0f) };
	UPROPERTY()
		FVector TireHitLocation[4] = { FVector(0.0f) , FVector(0.0f) ,FVector(0.0f) ,FVector(0.0f) };
	UPROPERTY()
		uint8 bIsSliding[4] = { 0,0,0,0 };

	UPROPERTY()
		float CurrentWheelPitch[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	UPROPERTY()
		float DeltaPitch = 0.0f;
	UPROPERTY()
		float CurrentAngle[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	UPROPERTY()
		float WheelRPM = 0.0f;

public:

	void GetIsSliding(TArray<bool> &Out) const;

	bool GetIsSlidingFront() const { return bIsSliding[0] && bIsSliding[1];}
	
	bool GetIsSlidingBack() const { return bIsSliding[2] && bIsSliding[3];}
	

private :
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
		float SlipThreshold = 220000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		float LongSlipThreshold = 150000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		float SmokeKickIn = 3.0f;

	/**
	 *	BaseGrip
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grip")
		float Grip = 1000.f;

	/**
	 *	maximum weight/grip multiplier, cap for weight transfer formula. 
	 *	1 = no effect, >1 = capped to MaxGrip x default Grip
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grip")
		float MaxGrip = 1.05f;

	/**
	 *	maximum weight/grip multiplier for the lateral grip only, cap for weight transfer formula.
	 *	1 = no effect, >1 = capped to MaxGrip x default Grip
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grip")
		float MaxLatGrip = 1.05f;

	//max steer angle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
		float SteerAngle = 45.0f;
	//lerp speed of wheel steering
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
		float SteerSpeed = 3.0f;
public :
	FCarWheelsSetup();

private:

	friend class UCarStatics;
	friend class ASimpleCar;
};



USTRUCT(BlueprintType, Category = "Suspension", meta = (DisplayName = "Springs Configurations") )
struct FSpringsLocation
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly)
		float ZFront;

	UPROPERTY(EditDefaultsOnly)
		float ZBack;

	UPROPERTY(EditDefaultsOnly)
		float YFront;

	UPROPERTY(EditDefaultsOnly)
		float YBack;

	UPROPERTY(EditDefaultsOnly)
		float XFront;

	UPROPERTY(EditDefaultsOnly)
		float XBack;

public:
	FVector GetLocation(bool bIsFront, bool bIsLeft)
	{
		const float x = bIsFront ? XFront : XBack;
		const float y = bIsFront ? YFront : YBack;
		const float z = bIsFront ? ZFront : ZBack;
		const float sign = bIsLeft ? -1 : 1;
		return {x, y * sign, z};
	}

	FSpringsLocation(float NewZFront = 40.f, float NewZBack = 40.f, float NewYFront = 90.f, float NewYBack = -90.f,
	                 float NewXFront = 145.f, float NewXBack = -90.f) : ZFront(NewZFront), ZBack(NewZBack),
	                                                                 YFront(NewYFront), YBack(NewYBack),
	                                                                 XFront(NewXFront), XBack(NewXBack)
	{
	}

	friend FCarSuspensionsSetup;
	friend class UCarStatics;
	friend class ASimpleCar;

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

USTRUCT(BlueprintType, Category = "Suspension", meta = (DisplayName = "Car Suspensions Setup"))
struct FCarSuspensionsSetup
{
	GENERATED_BODY()
protected:
	/**		*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TraceLength = 70.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpringValue = 950000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxSpringValue = 90000.f;//1200000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamperValue = 500.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSpringsLocation SpringConfiguration;

	/**	Spring base location  determined by */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FVector> SpringTopLocation;

	/**	anti-roll */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anti-roll")
		float AntiRollFront = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anti-roll")
		float AntiRollBack = 0.4f;

public :

	FVector GetTopLocation(bool bIsFront, bool bIsLeft)
	{
		return  SpringConfiguration.GetLocation(bIsFront, bIsLeft);
	}

	void Init();

	friend class UCarStatics;
	friend class ASimpleCar;
};




/**
 * 
 */
UCLASS(Category = "Car")
class RACE_API UCarStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public :
	UFUNCTION(BlueprintPure, Category ="Dashboard|Speed", meta = (DisplayName = "Speed in Kilometer per hour"))
		static float GetSpeed_KPH(const FCarDashBoard &Infos);

	UFUNCTION(BlueprintPure, Category = "Dashboard|Speed", meta = (DisplayName = "Speed in Kilometer per hour clamped"))
		static int GetSpeed_intKPH(const FCarDashBoard &Infos);

	UFUNCTION(BlueprintPure, Category = "Dashboard|Speed", meta = (DisplayName = "speed in Kilometers per hour clamped as text"))
		static FText GetSpeed_textKPH(const FCarDashBoard &Infos);

	UFUNCTION(BlueprintPure, Category = "Dashboard|Engine", meta = (DisplayName = "Engine rotation speed in Round per minutes"))
		static float GetEngine_RPM(const FCarDashBoard  &Infos);

	UFUNCTION(BlueprintPure, Category = "Dashboard|Engine", meta = (DisplayName = "Engine rotation speed in Round per minutes clamped"))
		static int GetEngine_intRPM(const FCarDashBoard  &Infos);

	UFUNCTION(BlueprintPure, Category = "Dashboard|Engine", meta = (DisplayName = "Engine rotation speed in Round per minutes clamped as text"))
		static FText GetEngine_textRPM(const FCarDashBoard  &Infos);

	UFUNCTION(BlueprintPure, Category = "Dashboard|Transmission", meta = (DisplayName = "Gear as a number"))
		static int GetTransmission_intGear(const FCarDashBoard  &Infos);

	UFUNCTION(BlueprintPure, Category = "Dashboard|Transmission", meta = (DisplayName = "Gear as text"))
		static FText GetTransmission_textGear(const FCarDashBoard  &Infos);

	UFUNCTION(BlueprintPure, Category = "Wheels|Drift")
		static void GetSlidingWheels(const ARaceCar * Car , TArray<bool> &Out);

	UFUNCTION(BlueprintPure, Category = "Wheels|Drift")
		static bool GetSlidingFront(const ARaceCar * Car);

	UFUNCTION(BlueprintPure, Category = "Wheels|Drift")
		static bool GetSlidingBack(const ARaceCar * Car);
	
};
