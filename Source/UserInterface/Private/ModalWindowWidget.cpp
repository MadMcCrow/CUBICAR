// Fill out your copyright notice in the Description page of Project Settings.

#include "ModalWindowWidget.h"


UModalWindowWidget::UModalWindowWidget()
{

}

bool UModalWindowWidget::Initialize()
{
	bool Success = Super::Initialize();
	SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);
	return Success;
}

void UModalWindowWidget::SetIsEnabled(bool bInIsEnabled)
{
	Super::SetIsEnabled(bInIsEnabled);
	if (bInIsEnabled)
	{
		SetVisibility(ESlateVisibility::Visible);
		return;
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UModalWindowWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
}


