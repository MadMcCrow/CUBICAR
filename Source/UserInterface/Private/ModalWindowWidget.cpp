// Fill out your copyright notice in the Description page of Project Settings.

#include "ModalWindowWidget.h"
#include "TextBlock.h"
#include "CUBICARButton.h"
#include "WidgetTree.h"


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
	if(WidgetTree)
	{
		TArray < UWidget * > Children;
		WidgetTree->GetAllWidgets(Children);
		for (auto it : Children)
			it->SetIsEnabled(true);
	}


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
	if(Title)
		Title->SetText(WindowTitle);

}

void UModalWindowWidget::Close() {SetIsEnabled(false);}
void UModalWindowWidget::Open() {SetIsEnabled(true);}


void UModalWindowWidget::BindDelegates()
{
	if(CloseButton)
	{
		CloseButton->OnClicked.Clear();
		CloseButton->OnClicked.AddDynamic(this, &UModalWindowWidget::Close);
	}
}
