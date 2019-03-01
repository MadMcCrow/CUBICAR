// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "Input/Reply.h"
#include "Styling/SlateTypes.h"
#include "Widgets/SWidget.h"
#include "Components/ContentWidget.h"
#include "CUBICARWidget.generated.h"


/**
 * @class UCUBICARWidget class
 * describes the basic elements a widget can do. widgets are mostly menus and stuff, but buttons also inherit this class
 */
UCLASS(ClassGroup = (UI))
class USERINTERFACE_API UCUBICARWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/** default Constructor */
	UCUBICARWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**	Allows for setting Applying Variables set in Blueprint	*/
	virtual void OnWidgetRebuilt() override;
	

protected:

	virtual void PostLoad() override;

	UFUNCTION()
		virtual void BindDelegates();

	
	
	
	
};
