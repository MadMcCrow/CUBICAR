#pragma once

#include "GameFramework/Pawn.h"
#include "Components/AudioComponent.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "CarStatics.h"
#include "SimpleCar.generated.h"


class USpringArmComponent;
class UStaticMesh;
class UCameraComponent;
class UParticleSystemComponent;
class USoundCue;



UCLASS()
class RACE_API ASimpleCar : public APawn
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated, BlueprintReadOnly)
		FCarDashBoard DashboardInfo;

public:
	UFUNCTION(BlueprintPure, Category = "Info")
		FCarDashBoard GetDashboardInfo() const { return DashboardInfo; }

protected:

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Wheels")
		UStaticMesh * WheelMesh;

	UFUNCTION(BlueprintPure, Category = "Wheels")
		virtual UStaticMesh * GetWheelMesh();

	UFUNCTION()
		virtual void UpdateWheels();
	UFUNCTION()
		virtual void UpdateSuspensions();

private:
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Multicast_UpdateWheelsMesh();

	UFUNCTION(Server, Reliable, WithValidation)
		void  Server_UpdateSuspensions();


	UPROPERTY(EditAnywhere)
		bool bDrawDebug;


private:
	/**  The main skeletal mesh associated with this Vehicle */
	UPROPERTY(Category = Vehicle, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh;

	//wheel meshes
	UPROPERTY()
		class UStaticMeshComponent* Wheel0;
	UPROPERTY()
		class UStaticMeshComponent* Wheel1;
	UPROPERTY()
		class UStaticMeshComponent* Wheel2;
	UPROPERTY()
		class UStaticMeshComponent* Wheel3;

	//wheel array
	UPROPERTY(BlueprintReadOnly, Category = "Wheels", meta = (AllowPrivateAccess = "true"))
		TArray<UStaticMeshComponent*> WheelArray;

	//private:
		/** audio component for engine sounds */
	UPROPERTY(Category = Effects, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAudioComponent* EngineAC;

private:
	FCalculateCustomPhysics OnCalculateCustomPhysics;

	
	void CustomPhysics(float DeltaTime, FBodyInstance* BodyInstance);


	FHitResult Trace(FVector TraceStart, FVector TraceDirection);
	FBodyInstance *MainBodyInstance;

	UPROPERTY()
		TArray<float> PreviousPosition;

	UPROPERTY()
		FVector ArrowLocation;

public:
	// Sets default values for this pawn's properties
	ASimpleCar();

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;

	/** Name of the MeshComponent. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName VehicleMeshComponentName;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void UpdateCar(float DeltaSeconds);

#if 0
	// Accurate location replication for other clients and server. The client is authoritative. Could lead to Cheat, see validate
	UFUNCTION(Server, Reliable, WithValidation)
		virtual void Server_UpdateTransform(FTransform NewTransform);

	UPROPERTY(Replicated) //Using = OnRep_CarTransform)
		FTransform CarTransform;

	//UFUNCTION()
	//	void OnRep_CarTransform();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		virtual void Multicast_UpdateTransform(FTransform NewTransform);

	UFUNCTION(BlueprintPure, Category = Transform)
		FTransform GetCarTransform() const { return CarTransform; }

#endif //0

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	virtual void Multicast_UpdateEffects(float DeltaTime);

	void SpawnSmokeEffect(int WheelIndex);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Handle pressing forwards */
	void MoveForward(float Val);

	/** Handle pressing right */
	void MoveRight(float Val);

	/** gear up */
	void ChangeUp();

	/** gear Down */
	void ChangeDown();

	FVector AddDrive(float DeltaTime, FBodyInstance* BodyInstance, FVector Loc, FVector Dir, int32 Index);

	FVector AddLatGrip(float DeltaTime, FBodyInstance* BodyInstance, FVector Loc, FVector Dir, int32 Index);

	float GetPowerToWheels(float DeltaTime, FBodyInstance* BodyInstance);

	void SpawnNewWheelEffect(int WheelIndex);
	/** update effects under wheels */
	void UpdateWheelEffects(float DeltaTime, int32 Index);


	// Suspension --------------------------------------------------------------------------------------------------/
	UPROPERTY( Replicated, EditAnywhere, BlueprintReadWrite, Category = "Suspension")
		FCarSuspensionsSetup Suspensions;

#if 0
	//trace start locations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suspension")
		UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Suspension")
		TArray<FVector> SpringTopLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suspension")
		float TraceLength = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suspension")
		float SpringValue = 800000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suspension")
		float MaxSpringValue = 1200000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suspension")
		float DamperValue = 1000.0f;
#endif //0


	UPROPERTY(BlueprintReadOnly, Category = "Suspension")
		TArray<bool> bOnGround;

	UPROPERTY(BlueprintReadOnly, Category = "Suspension")
		TArray<float> SpringLengthArray;

	UPROPERTY(BlueprintReadOnly, Category = "Suspension")
		FVector SpringLocation;



	UPROPERTY(BlueprintReadOnly, Category = "Suspension")
		TArray<FVector> SuspForceLocation;
	UPROPERTY(BlueprintReadOnly, Category = "Suspension")
		TArray<FVector> SpringForceArray;

	// Engine --------------------------------------------------------------------------------------------------/
	/** engine sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSound")
		USoundCue* EngineSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSound")
		float EnginePitchMax = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSound")
		float EnginePitchIdle = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float EnginePower = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float EnginePowerScale = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float WastedPower = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float EngineIdleRPM = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float EngineMaxRPM = 7000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float AirResistance = 3.0f;


	UPROPERTY(EditDefaultsOnly, Category = "Engine")
		UCurveFloat* TorqueCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float RedLineRPM = 6000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float GearUpRPM = 5200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float GearDownRPM = 2000.0f;
	//mechanical/friction braking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float EngineBrake = 30.0f;
	//brake force
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine")
		float BrakeForce = 50000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gears")
		TArray<float> Gears;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gears")
		int32 CurrentGear = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gears")
		float FinalGearRatio = 3.92;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gears")
		bool bAutomaticGears = true;
	UPROPERTY(BlueprintReadOnly, Category = "Engine")
		float WheelRPM = 0.0f;

	UPROPERTY(Replicated)
		FCarEngineUpdate EngineUpdate;


	UFUNCTION(BlueprintPure)
		float GetEngineCurrentRPM() const { return EngineUpdate.EngineRPM; }
	UFUNCTION(BlueprintPure)
		float GetEngineMaxRPM() const { return EngineMaxRPM; }
	UFUNCTION(BlueprintPure)
		float GetEngineMinRPM() const { return EngineIdleRPM; }
	UFUNCTION(BlueprintPure)
		float GetRedLineRPM() const { return RedLineRPM; }


	UFUNCTION(Server, Reliable, WithValidation)
		void Server_UpdateThrottle(float Throttle);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_UpdateWheelAngle(float Val);

	// Wheels --------------------------------------------------------------------------------------------------/

	//wheel bones
	//UPROPERTY(EditAnywhere, Category = "Wheels")
	//	TArray<FName> BoneNames;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Wheels")
		FCarWheelsSetup  WheelSetup;

#if 0
	//unused
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		TArray<bool> bIsPowered;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		float RotationAmount = -1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		float SlipThreshold = 140000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		float SmokeKickIn = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		float LongSlipThreshold = 100000.0f;

	//UPROPERTY(EditAnywhere, Category = "Wheels")
		//float MaxLatGrip = 50000;
	//UPROPERTY(EditAnywhere, Category = "Wheels")
		//float MaxLongGrip = 50000;

	//grip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		float Grip = 1.0f;
	//maximum weight/grip multiplier, cap for weight transfer formula. 
	//1 = no effect, >1 = capped to MaxGrip x default Grip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		float MaxGrip = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheels")
		float MaxLatGrip = 2.0f;

	//wheel radius
	UPROPERTY(EditAnywhere, Category = "Wheels")
		float Radius = 32.0f;
#endif //0

protected:
	UPROPERTY(Replicated)
		FCarWheelsUpdate WheelsUpdate;

	UFUNCTION(BlueprintPure, Category = "Wheels")
		void GetSliding(TArray<bool> &OutArray) const;

	//UPROPERTY(BlueprintReadOnly, Category = "Wheels")
	UPROPERTY()//Replicated)
		FVector TireForceArray[4] = { FVector(0.0f) , FVector(0.0f) ,FVector(0.0f) ,FVector(0.0f) };
	UPROPERTY()//Replicated)
		FVector WheelForwardArray[4] = { FVector(0.0f) , FVector(0.0f) ,FVector(0.0f) ,FVector(0.0f) };
	UPROPERTY()//Replicated)
		FVector WheelRightArray[4] = { FVector(0.0f) , FVector(0.0f) ,FVector(0.0f) ,FVector(0.0f) };

	//tire smoke
	UPROPERTY(EditAnywhere, Category = "Wheels")
		UParticleSystem* TireSmoke;
	UPROPERTY(EditAnywhere, Category = "Wheels")
		UParticleSystem* TireMarks;
	/** dust FX components */
	UPROPERTY(Transient)
		UParticleSystemComponent* DustPSC[4];

	/** skid sound loop */
	UPROPERTY(Category = Effects, EditDefaultsOnly)
		USoundCue* SkidSound;

	//max steer angle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
		float SteerAngle = 45.0f;

	//lerp speed of wheel steering
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering")
		float SteerSpeed = 3.0f;



	/** Returns Mesh subobject **/
	class USkeletalMeshComponent* GetMesh() const;

	//FORCEINLINE UPoseableMeshComponent* GetVisibleMesh() const {return VisibleMesh;}
	/** Returns SpringArm subobject **/
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

private :

	friend UCarStatics;

};

