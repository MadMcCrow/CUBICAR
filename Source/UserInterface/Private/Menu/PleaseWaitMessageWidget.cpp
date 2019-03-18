// Fill out your copyright notice in the Description page of Project Settings.

#include "PleaseWaitMessageWidget.h"


void UPleaseWaitMessageWidget::UnlockUI(FWidgetsContainerCUBICAR& Widgets)
{

}

void UPleaseWaitMessageWidget::LockUI(FWidgetsContainerCUBICAR& Widgets)
{

}

void UPleaseWaitMessageWidget::EndPleaseWait()
{
	DestroyFromParent();
}

void UPleaseWaitMessageWidget::DestroyFromParent_Implementation()
{
	auto Widgets = UUserInterfaceStatics::FromCurrentWidgets(this, false);
	
	auto MainMenu = UUserInterfaceStatics::GetMainMenu(Widgets);
	if (MainMenu)
		MainMenu->SetIsEnabled(true);
	else
		Widgets.EnableAll();
	RemoveFromParent();
	//BeginDestroy();
	
}

void UPleaseWaitMessageWidget::BindDelegates()
{

}

