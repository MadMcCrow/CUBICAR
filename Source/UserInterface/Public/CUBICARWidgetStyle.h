// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Styling/ButtonWidgetStyle.h"
#include "Style/CustomButtonStyle.h"
#include "CUBICARWidgetStyle.generated.h"

struct FCustomButtonStyle;
/**
 * 
 */
UCLASS()
class UCUBICARWidgetStyle : public USlateWidgetStyleContainerBase
{
public:
	GENERATED_BODY()

public:
	/** The actual data describing the button's appearance. */
	UPROPERTY(Category = Appearance, EditAnywhere, BlueprintReadWrite, meta = (ShowOnlyInnerProperties))
		FCustomButtonStyle ButtonStyle;


	
	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast<const struct FSlateWidgetStyle*>(&ButtonStyle);
	}

	/**
	 *	@fn SetTextStyle()
	 *	@brief Sets the text bloc according to the style 
	 *	@param Button : the custom button to skin.
	 *	@param WidgetStyle : the style to use
	 */
	static void SetStyle(class UCUBICARButton* Button, const FCustomButtonStyle& WidgetStyle);
	
};