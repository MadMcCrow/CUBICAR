// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MobileStatics.generated.h"


UENUM(BlueprintType)
enum EMobilePlateform :uint8
{
	MPE_Android 	UMETA(DisplayName = "Android"),
	MPE_IOS			UMETA(DisplayName = "IOS")
};


/**
 *	Static function for mobile plateform
 */
UCLASS()
class MOBILEAPP_API UMobileStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure)
	static EMobilePlateform GetMobilePlateform();

};
