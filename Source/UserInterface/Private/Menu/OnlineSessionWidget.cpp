// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineSessionWidget.h"
#include "TextBlock.h"
#include "UserInterfaceStatics.h"
#include "CUBICARButton.h"


UOnlineSessionWidget::UOnlineSessionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UOnlineSessionWidget::BindDelegates()
{
	Super::BindDelegates();
	if(JoinSession)
	{
		JoinSession->OnClicked.Clear();
		JoinSession->OnClicked.AddDynamic(this, &UOnlineSessionWidget::RequestJoin);
	}
}

void UOnlineSessionWidget::SetSession(const FServerStatus& NewSession)
{
	if (!NewSession.IsValid())
		return;
	Session = NewSession;
	if (ServerName || Players || Spectators)
	{
		ServerName->SetText(FText::FromName(Session.GetName()));
		Players->SetText(FText::FromString(FString::FromInt(Session.CurrentPlayers)));
		Spectators->SetText(FText::FromString(FString::FromInt(Session.CurrentSpectators)));
	}
}

void UOnlineSessionWidget::RequestJoin()
{
	if (!Session.IsValid())
		return;
	UCUBICARStatics::JoinGame(GetOwningPlayer(), Session);
}

UOnlineSessionWidget::UOnlineSessionWidget(const FServerStatus &SessionSettings, const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Session = SessionSettings;
}
