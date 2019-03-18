#include "UserInterfaceStatics.h"
#include "UserInterface.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "CUBICARStatics.h"
#include "CUBICARWidget.h"
#include "Engine/Engine.h"
#include "UObjectIterator.h"
#include "Menu/MainMenuWidget.h"
#include "Menu/PleaseWaitMessageWidget.h"
#include "Menu/QuitMessageWidget.h"


void FWidgetsContainerBase::Clean()
{
	for (int T = Widgets.Num(); T > 0; --T)
		if (!Widgets[T])
		{
			Widgets.RemoveAt(T);
		}
}

void FWidgetsContainerBase::SetIsEnableAll(bool bIsEnable)
{
	for (auto it : Widgets)
	{
		if (it)
			it->SetIsEnabled(bIsEnable);
	}
}

void FWidgetsContainerBase::AddWidget(UUserWidget* Widget)
{
	if (Widget)
		Widgets.Add(Widget);
}

FWidgetsContainerBase FWidgetsContainerBase::FromCurrentWidgets(UObject* WorldContextObject)
{
	FWidgetsContainerBase Ret;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(WorldContextObject, Ret.Widgets, UUserWidget::StaticClass(), false);
	return Ret;
}

void UUserInterfaceStatics::ShowMainMenu(const UObject* WorldContextObject, const FCUBICARWidgetClasses &WidgetClasses, FWidgetsContainerCUBICAR &Widgets)
{
	UUserWidget * MainMenu = Widgets.MainMenu;
	if(!MainMenu)
	{
		MainMenu = CreateMainMenu(WorldContextObject, WidgetClasses);
		if (!MainMenu)
		{
			UE_LOG(LogUserInterface, Error, TEXT("Couldn't Create a Main Menu"));
			return;
		}
	}

	APlayerController * PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	MainMenu->AddToViewport(0);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, MainMenu, EMouseLockMode::DoNotLock);
	PC->bShowMouseCursor = true;

	UWorld* World = WorldContextObject->GetWorld();
	if (World && World->IsGameWorld())
		UGameViewportClient* ViewportClient = World->GetGameViewport();

}

UUserWidget* UUserInterfaceStatics::CreateMainMenu(const UObject* WorldContextObject,	const FCUBICARWidgetClasses& Widgets)
{
	UUserWidget * Ret = nullptr;
	APlayerController * PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PC)
	{
		//UE_LOG(LogSlate, Error, TEXT("No Valid Player Controller"));
		return Ret;
	}
	if (Widgets.MainMenuClass)
		Ret = CreateWidget<UUserWidget>(PC, Widgets.MainMenuClass);

	if (!Ret)
		UE_LOG(LogUserInterface, Error, TEXT("CreateWidget Failed"));

	return Ret;
}


UUserWidget * UUserInterfaceStatics::ShowQuitGameConfirmationMessage(const UObject* WorldContextObject, const FCUBICARWidgetClasses &WidgetClasses, FWidgetsContainerCUBICAR &Widgets)
{
		APlayerController * PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
		if (!PC)
			return nullptr;

		UUserWidget * QuitMessage = Widgets.QuitMessage;
		if (!QuitMessage)
		{
			if (WidgetClasses.QuitMessageClass)
				QuitMessage = CreateWidget<UUserWidget>(PC, WidgetClasses.QuitMessageClass);
				
			if (!QuitMessage)
			{
				UE_LOG(LogUserInterface, Error, TEXT("Couldn't Create a Main Menu"));
				return nullptr;
			}
			Widgets.QuitMessage = QuitMessage;
		}

		
		QuitMessage->AddToViewport(0);
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, QuitMessage, EMouseLockMode::DoNotLock);
		PC->bShowMouseCursor = true;

		UWorld* World = WorldContextObject->GetWorld();
		if (World && World->IsGameWorld())
			UGameViewportClient* ViewportClient = World->GetGameViewport();




	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, Widgets.QuitMessage, EMouseLockMode::DoNotLock);
	PC->bShowMouseCursor = true;

	if (Widgets.MainMenu)
		Widgets.MainMenu->SetIsEnabled(false);

	Widgets.QuitMessage->AddToViewport(10);
	Widgets.QuitMessage->SetVisibility(ESlateVisibility::Visible);
	Widgets.QuitMessage->SetIsEnabled(true);
	return Widgets.QuitMessage;
}

void UUserInterfaceStatics::CancelQuitGameConfirmationMessage(const UObject* WorldContextObject, const FCUBICARWidgetClasses &WidgetClasses, FWidgetsContainerCUBICAR &Widgets)
{

	APlayerController * PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PC)
		return;

	if (Widgets.QuitMessage)
	{
		Widgets.QuitMessage->RemoveFromParent();
		Widgets.QuitMessage->BeginDestroy();
		Widgets.EnableAll();
		if (Widgets.MainMenu)
		{
			Widgets.MainMenu->SetIsEnabled(true);
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, Widgets.MainMenu, EMouseLockMode::LockOnCapture);
			PC->bShowMouseCursor = true;
		}
		Widgets.QuitMessage = nullptr;
	}
}

UUserWidget * UUserInterfaceStatics::ShowPleaseWaitMessage(const UObject* WorldContextObject, const FCUBICARWidgetClasses &WidgetClasses, FWidgetsContainerCUBICAR &Widgets)
{
	APlayerController * PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PC)
		return nullptr;

	UUserWidget * PleaseWaitMessage = Widgets.PleaseWaitMessage;
	if (!PleaseWaitMessage)
	{
		if (WidgetClasses.PleaseWaitMessageClass)
			PleaseWaitMessage = CreateWidget<UUserWidget>(PC, WidgetClasses.PleaseWaitMessageClass);

		if (!PleaseWaitMessage)
		{
			UE_LOG(LogUserInterface, Error, TEXT("Couldn't Create a Please Wait"));
			return nullptr;
		}
		Widgets.PleaseWaitMessage = PleaseWaitMessage;
	}

	PleaseWaitMessage->AddToViewport(0);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, PleaseWaitMessage, EMouseLockMode::DoNotLock);
	PC->bShowMouseCursor = true;

	UWorld* World = WorldContextObject->GetWorld();
	if (World && World->IsGameWorld())
		UGameViewportClient* ViewportClient = World->GetGameViewport();




	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, PleaseWaitMessage, EMouseLockMode::DoNotLock);
	PC->bShowMouseCursor = true;

	Widgets.DisableAll();
	PleaseWaitMessage->SetIsEnabled(true);

	auto PWM = Cast<UPleaseWaitMessageWidget>(PleaseWaitMessage);
	if(PWM)
		return PWM;
	return nullptr;
	
}

void UUserInterfaceStatics::ShowLoadingScreen(const UObject* WorldContextObject,
	const FCUBICARWidgetClasses& WidgetClasses, FWidgetsContainerCUBICAR& Widgets)
{
	// Todo : Do something here
}

int UUserInterfaceStatics::ValidateNumber(const FText& Text)
{
	if (Text.IsEmpty())
		return 0;
	if (Text.IsNumeric())
		return FCString::Atoi(*Text.ToString());
	return 0;

}

FName UUserInterfaceStatics::ValidateName(const FText & Text)
{
	if (Text.IsEmpty())
		return FName();
	const FString InvarientName = Text.ToString().Replace(TEXT(" "), TEXT(""));\
	return FName(*InvarientName);
}



FWidgetsContainerCUBICAR UUserInterfaceStatics::FromCurrentWidgets(const UObject* WorldContextObject, bool TopLevelOnly)
{
	FWidgetsContainerCUBICAR Ret;
	//Prevent possibility of an ever-growing array if user uses this in a loop
	Ret.Widgets.Empty();
	Ret.MainMenu = nullptr;
	Ret.PleaseWaitMessage = nullptr;
	Ret.QuitMessage = nullptr;

	if (!WorldContextObject)
		return Ret;
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
		return Ret;

	for (TObjectIterator<UUserWidget> Itr; Itr; ++Itr)
	{
		UUserWidget* LiveWidget = *Itr;

		// Skip any widget that's not in the current world context.
		if (LiveWidget->GetWorld() != World)
		{
			continue;
		}

		// Skip any widget that is not a child of the class specified.
		if (!LiveWidget->GetClass()->IsChildOf(UCUBICARWidget::StaticClass()))
		{
			continue;
		}

		if (TopLevelOnly)
		{
			if (LiveWidget->IsInViewport())
			{
				Ret.Widgets.Add(LiveWidget);
			}
		}
		else
		{
			Ret.Widgets.Add(LiveWidget);
		}
		// there's no switch on class possible unfortunately
		if (LiveWidget->GetClass()->IsChildOf(UMainMenuWidget::StaticClass()))
			Ret.MainMenu = LiveWidget;
		else if (LiveWidget->GetClass()->IsChildOf(UPleaseWaitMessageWidget::StaticClass()))
			Ret.PleaseWaitMessage = LiveWidget;
		else if (LiveWidget->GetClass()->IsChildOf(UQuitMessageWidget::StaticClass()))
			Ret.QuitMessage = LiveWidget;

	}

	return Ret;
}

FCUBICARWidgetClasses UUserInterfaceStatics::GetCUBICARWidgetClasses(bool &IsValid)
{
	const auto Singleton= UCUBICARStatics::GetCUBICARData(IsValid);
	if (IsValid)
		return Singleton->WidgetClasses;
	return FCUBICARWidgetClasses();
}

UUserWidget* UUserInterfaceStatics::GetMainMenu(const FWidgetsContainerCUBICAR &Widgets)
{
	if(Widgets.MainMenu)
		return Widgets.MainMenu;
	for(auto it : Widgets.Widgets)
	{
		if (Cast<UMainMenuWidget>(it))
		{
			Widgets.MainMenu = it;
			return it;
		}
	}
	return nullptr;
}

UUserWidget* UUserInterfaceStatics::GetPleaseWait(const FWidgetsContainerCUBICAR &Widgets)
{
	if (Widgets.PleaseWaitMessage)
		return Widgets.PleaseWaitMessage;
	for (auto it : Widgets.Widgets)
	{
		if (Cast<UPleaseWaitMessageWidget>(it))
		{
			Widgets.PleaseWaitMessage = it;
			return it;
		}
	}
	return nullptr;
}

UUserWidget* UUserInterfaceStatics::GetQuitMessage(const FWidgetsContainerCUBICAR &Widgets)
{
	if (Widgets.QuitMessage)
		return Widgets.QuitMessage;
	for (auto it : Widgets.Widgets)
	{
		if (Cast<UQuitMessageWidget>(it))
		{
			Widgets.QuitMessage = it;
			return it;
		}
	}
	return nullptr;
}
