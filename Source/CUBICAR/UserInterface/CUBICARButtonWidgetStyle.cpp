// Fill out your copyright notice in the Description page of Project Settings.

#include "CUBICARButtonWidgetStyle.h"
#include "TextBlock.h"
#include "Button.h"
#include "CUBICARButton.h"
#include "StyleDefaults.h"


FCUBICARButtonStyle::FCUBICARButtonStyle()
	: Font()
	, ColorAndOpacity()
	, ShadowOffset(FVector2D::ZeroVector)
	, ShadowColorAndOpacity(FLinearColor::Black)
{
}

void FCUBICARButtonStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
		OutBrushes.Add(&HighlightShape);
		OutBrushes.Add(&UnderlineBrush);
}

const FCUBICARButtonStyle& FCUBICARButtonStyle::GetDefault()
{
	static TSharedPtr< FCUBICARButtonStyle > Default;
	if (!Default.IsValid())
	{
		Default = MakeShareable(new FCUBICARButtonStyle());
		Default->Font = FStyleDefaults::GetFontInfo();
	}
	return *Default.Get();
}

const FName FCUBICARButtonStyle::TypeName(TEXT("FCUBICARButtonStyle"));



void UCUBICARButtonWidgetStyle::SetStyle(UCUBICARButton* Button, const FCUBICARButtonStyle& WidgetStyle)
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
