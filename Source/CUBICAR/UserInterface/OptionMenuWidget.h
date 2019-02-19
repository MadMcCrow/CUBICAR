// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUBICARWidget.h"
#include "OptionMenuWidget.generated.h"

/**
 *  @class UOptionMenuWidget Base Class for the Option Menu
 */
UCLASS()
class CUBICAR_API UOptionMenuWidget : public UCUBICARWidget
{
	GENERATED_BODY()

public:
	UOptionMenuWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	UPROPERTY(meta = (BindWidget))
		class UCUBICARButton * BackButton;

	
    virtual void BindDelegates() override;

	
	
};
