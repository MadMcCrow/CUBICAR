// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UserInterfaceStatics.h"
#include "CUBICARSingleton.generated.h"

/**
 *	Singleton to Store all Important variables
 *	This is not meant to do any calculus
 */
UCLASS(Blueprintable, BlueprintType)
class CUBICAR_API UCUBICARSingleton : public UObject
{
	GENERATED_BODY()

public :

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CUBICAR Data Singleton")
		FCUBICARWidgetClasses WidgetClasses;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maps and Modes")
		TSoftObjectPtr<UWorld> MainMenuUMap;
	   
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maps and Modes")
		TSoftObjectPtr<UWorld> LobbyUMap;

	
	
	
};
