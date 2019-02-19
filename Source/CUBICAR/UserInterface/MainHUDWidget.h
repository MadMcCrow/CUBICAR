// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUBICARWidget.h"
#include "MainHUDWidget.generated.h"

/**
 *	@class UMainHUDWidget
 *	@brief Main class for the HUD
 */
UCLASS()
class CUBICAR_API UMainHUDWidget : public UCUBICARWidget
{
	GENERATED_BODY()

public:

	UMainHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;

	
};
