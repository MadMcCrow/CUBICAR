// Fill out your copyright notice in the Description page of Project Settings.

#include "HostSessionWidget.h"
#include "EditableTextBox.h"
#include "CUBICARButton.h"
#include "CUBICARStatics.h"
#include "ComboBoxString.h"
#include "UserInterfaceStatics.h"

UHostSessionWidget::UHostSessionWidget() : Super() , Players(10), Name(TEXT("DefaultServerName"))
{
}

void UHostSessionWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	UpdateServerName();
	UpdateMaxPlayers();
}

void UHostSessionWidget::NewMaxPlayer(const FText& Text, ETextCommit::Type CommitMethod)
{
	Players = UUserInterfaceStatics::ValidateNumber(Text);
	UpdateMaxPlayers();
}


void UHostSessionWidget::NewServerName(const FText& Text, ETextCommit::Type CommitMethod)
{
	Name = UUserInterfaceStatics::ValidateName(Text);
	UpdateServerName();
}

void UHostSessionWidget::UpdateServerName() const
{
	if (ServerName)
		ServerName->SetText(FText::FromString(Name.ToString()));
}

void UHostSessionWidget::UpdateMaxPlayers() const
{
	if(MaxPlayers)
		MaxPlayers->SetText(FText::AsNumber(Players));
}

void UHostSessionWidget::BindDelegates()
{
	Super::BindDelegates();
	if (ServerName)
	{
		ServerName->OnTextCommitted.Clear();
		ServerName->OnTextCommitted.AddDynamic(this, &UHostSessionWidget::NewServerName);
	}
	if (MaxPlayers)
	{
		MaxPlayers->OnTextCommitted.Clear();
		MaxPlayers->OnTextCommitted.AddDynamic(this, &UHostSessionWidget::NewMaxPlayer);
		UpdateMaxPlayers();
	}
	if (MorePlayers)
	{
		MorePlayers->OnClicked.Clear();
		MorePlayers->OnClicked.AddDynamic(this, &UHostSessionWidget::IncrementPlayers);
	}
	if (LessPlayers)
	{
		LessPlayers->OnClicked.Clear();
		LessPlayers->OnClicked.AddDynamic(this, &UHostSessionWidget::DecrementPlayers);
	}
	if (Connection)
	{
		Connection->ClearOptions();
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EOnlineSSModule"), true);
		
		for (uint8 ossm = 0; ossm != (uint8)EOnlineSSModule::OSSM_MAX; ++ossm)
		{
			Connection->AddOption(UCUBICARStatics::GetOnlineModuleName((EOnlineSSModule)ossm).ToString());
		}
		FString Default = EnumPtr->GetNameByValue((int64)0).ToString();
		Connection->SetSelectedOption(Default);
	}
	if(Host)
	{
		Host->OnClicked.Clear();
		Host->OnClicked.AddDynamic(this, &UHostSessionWidget::RequestHost);
	}
	
}



void UHostSessionWidget::RequestHost()
{
	if (Players <= 0 && Name.ToString().IsEmpty())
		return;
	UCUBICARStatics::HostGame((UObject*)GetOwningPlayer(), UCUBICARStatics::CreateSettings(Players, EOnlineSSModule::OSSM_Lan, Name));

	
}
