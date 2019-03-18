// Fill out your copyright notice in the Description page of Project Settings.

#include "QuitMessageWidget.h"
#include "TextBlock.h"

#define LOCTEXT_NAMESPACE "UIText"

FText UQuitMessageWidget::GetRandomQuitMessage()
{
	if (QuitMessages.Num() < 1)
		return FText(LOCTEXT("AreYouSureYouWantToQuit", "Are you sure you want to quit ?"));

	return QuitMessages[FMath::RandRange(0, QuitMessages.Num() - 1)];
}

void UQuitMessageWidget::OnWidgetRebuilt()
{
	QuitMessage->SetText(GetRandomQuitMessage());
}

#undef LOCTEXT_NAMESPACE
