// Fill out your copyright notice in the Description page of Project Settings.

#include "CUBICARWidgetStyle.h"
#include "TextBlock.h"
#include "Button.h"
#include "CUBICARButton.h"
#include "StyleDefaults.h"
#include "Style/CustomButtonStyle.h"


void UCUBICARWidgetStyle::SetStyle(UCUBICARButton* Button, const FCustomButtonStyle& WidgetStyle)
{
	if (!Button)
	{
		UE_LOG(LogTemp, Error, TEXT("Button couldn't be set"));
		return;
	}
	Button->GetButtonWidget()->WidgetStyle = WidgetStyle;
	Button->GetLabelWidget()->SetFont(WidgetStyle.Font);
	Button->GetLabelWidget()->SetColorAndOpacity(WidgetStyle.ColorAndOpacity);
	Button->GetLabelWidget()->SetShadowOffset(WidgetStyle.ShadowOffset);
	Button->GetLabelWidget()->SetShadowColorAndOpacity(WidgetStyle.ShadowColorAndOpacity);

}
