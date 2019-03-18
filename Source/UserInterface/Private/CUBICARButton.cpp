// Fill out your copyright notice in the Description page of Project Settings.

#include "CUBICARButton.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Style/CustomButtonStyle.h"
#include "SlateWidgetStyleAsset.h"
#include "CUBICARWidgetStyle.h"


UCUBICARButton::UCUBICARButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BindDelegates();
}

void UCUBICARButton::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	if (!ButtonWidget || !LabelWidget)
	{
		UE_LOG(LogSlate, Error, TEXT("Custom button not set."));
		return;	// do not do anything if there's no button or no Label
	}
	LabelWidget->SetText(Label);

	if (!CustomButtonStyle)
	return;

	const auto Style = CustomButtonStyle->GetStyle<FCustomButtonStyle>();
	if (Style)
		UCUBICARWidgetStyle::SetStyle(this, *Style);
	else
		UE_LOG(LogSlate, Error, TEXT("Custom style not set."));
}


void UCUBICARButton::BindDelegates()
{
	if (ButtonWidget)
	{
		ButtonWidget->OnClicked.Clear();
		ButtonWidget->OnClicked.AddDynamic(this, &UCUBICARButton::Clicked);
	}
}

void UCUBICARButton::Clicked()
{
	OnClicked.Broadcast();
}
