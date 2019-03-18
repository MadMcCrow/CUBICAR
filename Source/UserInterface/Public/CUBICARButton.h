// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUBICARWidget.h"
#include "Components/Button.h"
#include "CUBICARButton.generated.h"

/**
 * @class UCUBICARButton
 * A button with an automatically added text Label and styling to go with it.
 */
UCLASS()
class USERINTERFACE_API UCUBICARButton : public UCUBICARWidget
{
	GENERATED_BODY()
	
public:
	/**	Default Constructor */
    UCUBICARButton(const FObjectInitializer& ObjectInitializer);

	/**	Allows for setting Applying Variables set in Blueprint	*/
	virtual void OnWidgetRebuilt() override;

protected:
	/** The text of the Text box child, will eventually */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
		FText Label;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Style" )
		class USlateWidgetStyleAsset* CustomButtonStyle; // DEPRECATED
	//class UCUBICARButtonWidgetStyle* CustomButtonStyle;
protected:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock * LabelWidget;

	UPROPERTY(meta = (BindWidget))
		class UButton * ButtonWidget;

public:
	FORCEINLINE class UTextBlock *	GetLabelWidget()	const	{ return LabelWidget; }
	FORCEINLINE class UButton *		GetButtonWidget()	const	{ return ButtonWidget;}
	

	UFUNCTION(BlueprintCallable, Category = "Button|Event")
		virtual void Clicked();

	virtual void BindDelegates() override;


public:
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonClickedEvent	OnClicked;
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonPressedEvent	OnPressed;
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonReleasedEvent	OnRelease;
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonHoverEvent		OnHover;
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonHoverEvent		OnUnhover;

	
};
