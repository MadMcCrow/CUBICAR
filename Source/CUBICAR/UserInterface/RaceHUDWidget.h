// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserInterface/MainHUDWidget.h"
#include "RaceHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class CUBICAR_API URaceHUDWidget : public UMainHUDWidget
{
	GENERATED_BODY()


public :

	URaceHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	virtual bool Initialize() override;

	virtual void NativeTick(const FGeometry &MyGeometry, float DeltaSeconds);


protected :

	/**
	 *	The Current Car Speed - 0 if pawn cannot be found
	 */
	UPROPERTY(VisibleAnywhere, Category = "Dasboard", BlueprintReadOnly, meta = (DisplayName = "Car Speed"))
		float Speed;

	/**
	 *	The Current Engine RPM - 0 if pawn cannot be found
	 */
	UPROPERTY(VisibleAnywhere, Category = "Dasboard", BlueprintReadOnly, meta = (DisplayName = "Engine RPM"))
		float RPM;

	/**
	 *	The Current Gear - 0 if pawn cannot be found
	 */
	UPROPERTY(VisibleAnywhere, Category = "Dasboard", BlueprintReadOnly, meta = (DisplayName = "Transmission Gear"))
		int Gear;

	/**
	 *	Update all Dashboard values
	 */
	UFUNCTION()
		void UpdateDashBoardValues();

	void BindDelegates() override;

public :

	/**
	 *	Update Car speed values
	 *	@return Car Speed
	 */
	UFUNCTION(BlueprintCallable, Category = "Dashboard", meta = (DisplayName = "Get Car Speed"))
		float GetCarSpeed()const { return Speed; }

	/**
	 *	Update Car RPM values
	 *	@return Car RPM
	 */
	UFUNCTION(BlueprintCallable, Category = "Dashboard", meta = (DisplayName = "Get Car Engine RPM"))
		float GetCarRPM() const { return RPM; }

	/**
	 *	Update Car Gear values
	 *	@return Car Gear
	 */
	UFUNCTION(BlueprintCallable, Category = "Dashboard", meta = (DisplayName = "Get Car Transmission Gear"))
		int GetCarGear() const { return Gear; }
	

	/**
	 *	Get Car speed As text
	 *	@return Car speed as FName
	 */
	UFUNCTION(BlueprintCallable, Category = "Dashboard", meta = (DisplayName = "Get Car Speed as text"))
		FText GetCarSpeedAsText() const;

	/**
	 *	Get Car Gear As text
	 *	@return Car Gear as FName
	 */
	UFUNCTION(BlueprintCallable, Category = "Dashboard", meta = (DisplayName = "Get Car Engine RPM as text"))
		FText GetCarRPMAsText() const;

	/**
	 *	Get Car Gear As text
	 *	@return Car Gear as FText
	 */
	UFUNCTION(BlueprintCallable, Category = "Dashboard", meta = (DisplayName = "Get Car Transmission Gear as text"))
		FText GetCarGearAsText() const;

private :

	/** Stored Car pointer*/
	UPROPERTY()
		class ARacingCar * CarPawn;


protected :
	/** Get Car Pawn Pointer, may be null*/
	FORCEINLINE ARacingCar* GetPlayerCar() const { return CarPawn; }

	UFUNCTION(BlueprintPure)
		ARacingCar * GetCarPawn();
	
};
