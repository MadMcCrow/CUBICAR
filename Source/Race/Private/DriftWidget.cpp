// Fill out your copyright notice in the Description page of Project Settings.

#include "DriftWidget.h"
#include "RacePlayerState.h"
#include "TextBlock.h"

void UDriftWidget::OnWidgetRebuilt()
{
	if(DriftTimeTextBlock)
	{
		DriftTimeTextBlock->TextDelegate.BindDynamic(this, &UDriftWidget::GetDriftLength);
	}
	VisibilityDelegate.BindDynamic(this, &UDriftWidget::GetIsDriftVisible);
}

FText UDriftWidget::GetDriftLength()
{
	const auto PS = GetOwningPlayerState<ARacePlayerState>();
	if (!PS)
		return FText();

	const auto Time = FTimespan::FromSeconds(PS->GetTempDriftTime());

	FString TimeString;
	if (Time.ExportTextItem(TimeString, FTimespan::MinValue(), nullptr, 0, nullptr))
		return  FText::FromString(TimeString);

	return FText();
}

ESlateVisibility UDriftWidget::GetIsDriftVisible()
{
	return IsDrifting() ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
}

bool UDriftWidget::IsDrifting()
{
	const auto PS = GetOwningPlayerState<ARacePlayerState>();
	if (!PS)
		return false;
	return (PS->GetTempDriftTime() > 0);
}
