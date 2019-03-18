// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "CUBICARStatics.h"
#include "CUBICARButton.h"
#include "ModalWindowWidget.h"


void UMainMenuWidget::ShowOptions()
{
	if(HostContextMenu)
		HostContextMenu->Close();
	if(JoinContextMenu)
		JoinContextMenu->Close();
	if(OptionContextMenu)
		OptionContextMenu->Open();
}

void UMainMenuWidget::ShowHost()
{
	if(JoinContextMenu)
		JoinContextMenu->Close();
	if (OptionContextMenu)
		OptionContextMenu->Close();
	if(HostContextMenu)
		HostContextMenu->Open();
}

void UMainMenuWidget::ShowJoin()
{
	if(HostContextMenu)
		HostContextMenu->Close();
	if(OptionContextMenu)
		OptionContextMenu->Close();
	if(JoinContextMenu)
		JoinContextMenu->Open();
}

void UMainMenuWidget::ShowQuit()
{
	UCUBICARStatics::AskToQuit(GetOwningPlayer());
}

void UMainMenuWidget::BindDelegates()
{
	Super::BindDelegates();
	if (!HostContextButton || !JoinContextButton || !OptionContextButton || !QuitButton)
		return;

	HostContextButton->OnClicked.Clear();
	HostContextButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ShowHost);
	JoinContextButton->OnClicked.Clear();
	JoinContextButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ShowJoin);
	OptionContextButton->OnClicked.Clear();
	OptionContextButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ShowOptions);
	QuitButton->OnClicked.Clear();
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ShowQuit);
}

void UMainMenuWidget::SetIsEnabled(bool bInIsEnabled)
{
	Super::SetIsEnabled(bInIsEnabled);
	if (HostContextButton || JoinContextButton || OptionContextButton || QuitButton)
	{
		HostContextButton->SetIsEnabled(bInIsEnabled);
		JoinContextButton->SetIsEnabled(bInIsEnabled);
		OptionContextButton->SetIsEnabled(bInIsEnabled);
		QuitButton->SetIsEnabled(bInIsEnabled);
	}
	if (JoinContextMenu)
		JoinContextMenu->SetIsEnabled(false);
	if (OptionContextMenu)
		OptionContextMenu->SetIsEnabled(false);
	if (HostContextMenu)
		HostContextMenu->SetIsEnabled(false);
}
