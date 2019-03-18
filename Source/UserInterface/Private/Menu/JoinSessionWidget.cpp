// Fill out your copyright notice in the Description page of Project Settings.

#include "JoinSessionWidget.h"
#include "EditableTextBox.h"
#include "CUBICARButton.h"
#include "CUBICARStatics.h"
#include "CUBICARGameInstance.h"
#include "ComboBoxString.h"
#include "UserInterfaceStatics.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionWidget.h"
#include "ScrollBox.h"

int UJoinSessionWidget::MaxPlayers(100);
EOnlineSSModule UJoinSessionWidget::ModuleToUse(EOnlineSSModule::OSSM_Lan);

UJoinSessionWidget::UJoinSessionWidget() : Super()
{
	SearchCriterias = UCUBICARStatics::CreateSettings(MaxPlayers, ModuleToUse, TEXT("DefaultName"));
	const auto GI = UCUBICARStatics::GetCUBICARGameInstance(this);
	if (!GI)
		return;
	GI->NotifyFoundSessions.AddDynamic(this, &UJoinSessionWidget::UpdateSessions);
	bLock = false;
}

void UJoinSessionWidget::SetIsEnabled(bool bInIsEnabled)
{
	Super::SetIsEnabled(true);
	//SetVisibility(ESlateVisibility::Visible);
	if (bLock)
		return;
	if (bInIsEnabled)
	{
			SetVisibility(ESlateVisibility::Visible);
			return;
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UJoinSessionWidget::AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder)
{
	Super::AddToScreen(LocalPlayer, ZOrder);
	RefreshSessions();
}

void UJoinSessionWidget::NewPlayerName(const FText& Text, ETextCommit::Type CommitMethod)
{
	APlayerState *  PS = GetOwningPlayer()->PlayerState;
	FName PlayerName = UUserInterfaceStatics::ValidateName(Text);
	if (!PlayerName.IsEqual(TEXT("")))
		PS->SetPlayerName(PlayerName.ToString());
}



void UJoinSessionWidget::UpdateSessions(const FSessionSearchResult &SearchResults)
{
	UE_LOG(LogTemp, Warning, TEXT("Received Update Session bSuccesful : %d"), SearchResults.IsSuccessful());
	for (auto it : SessionWidgets)
	{
		it->RemoveFromViewport();
	}

	if (!SearchResults.IsSuccessful())
		return;
	const auto Array = SearchResults.GetSessions();
	for (auto it : Array)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session  : %s"), *it.GetName().ToString());
		AddSessionwidget(it);
	}
	bLock = false;

}

void UJoinSessionWidget::AddSessionwidget(const FServerStatus &Status)
{
	UOnlineSessionWidget * Widget;
	if(SessionWidgetClass)
	{
		Widget = CreateWidget<UOnlineSessionWidget>(GetOwningPlayer(), SessionWidgetClass);
	}
	else
	{
		Widget = CreateWidget<UOnlineSessionWidget>(GetOwningPlayer(), UOnlineSessionWidget::StaticClass());
	}
	Widget->SetSession(Status);
	SessionWidgets.Add(Widget);
	SessionWidgetsContainers->AddChild(Widget);

}


void UJoinSessionWidget::BindDelegates()
{
	Super::BindDelegates();
	if (PlayerName)
	{
		PlayerName->OnTextCommitted.Clear();
		PlayerName->OnTextCommitted.AddDynamic(this, &UJoinSessionWidget::NewPlayerName);
	}
	if(RefreshButton)
	{
		RefreshButton->OnClicked.Clear();
		RefreshButton->OnClicked.AddDynamic(this, &UJoinSessionWidget::RefreshSessions);
	}


}

void UJoinSessionWidget::RefreshSessions()
{
	UCUBICARStatics::FindGames(this, SearchCriterias);
	bLock = true;
}
