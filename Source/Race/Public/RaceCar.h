// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleCar.h"
#include "Parts/CarEngine.h"
#include "Parts/CarWheel.h"
#include "RaceCar.generated.h"

class UCarEngine;

/**
 *	Base Car for CUBICAR car
 */
UCLASS()
class RACE_API ARaceCar : public ASimpleCar
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;


protected:

	/**	Team Color used for the car		*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FColor TeamColor;

	/**	Wheel Setup		*/
	UPROPERTY(Replicated, BlueprintReadOnly)
		FWheelPart WheelUpgrade;

	/**	Engine Setup	*/
	UPROPERTY(Replicated, BlueprintReadOnly)
		FEnginePart EngineUpgrade;

private:

	UCarEngine * CarEngine;

public:
	//UFUNCTION(Server, reliable, WithValidation)
		//void Server_SetWheels(URaceCarWheel * NewWheels);

	UFUNCTION(Server, reliable, WithValidation)
		void Server_SetEngine(const FEnginePart &NewEngine);


	UFUNCTION()
		void UpdateWheels();

	UFUNCTION()
		void UpdateEngine();


	
};

