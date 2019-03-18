// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SlateEnums.h"
#include "Engine/DataTable.h"
#include "UserInterfaceStatics.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndPleaseWaitEvent);

class UUserInterfaceStatics;
class UUserWidget;
//class UMainMenuwidget;
//class UQuitMessageWidget;

USTRUCT(Blueprintable)
struct FWidgetsContainerBase
{
	GENERATED_BODY()


private :
	UPROPERTY(Transient)
		TArray<UUserWidget *> Widgets;
public:
	void Clean();

	void SetIsEnableAll(bool bIsEnable);

	void DisableAll()	{ SetIsEnableAll(false);}
	void EnableAll()	{ SetIsEnableAll(true); }

	void AddWidget(UUserWidget * Widget);

	static FWidgetsContainerBase FromCurrentWidgets(UObject* WorldContextObject);


	friend UUserInterfaceStatics;

};


USTRUCT(Blueprintable)
struct FWidgetsContainerCUBICAR : public FWidgetsContainerBase
{
	GENERATED_BODY()


private:
		UPROPERTY()
		mutable UUserWidget*	MainMenu;

		UPROPERTY()
		mutable UUserWidget*	PleaseWaitMessage;

		UPROPERTY()
		mutable UUserWidget*	QuitMessage;


public :

	

	friend UUserInterfaceStatics;

};

USTRUCT(Blueprintable)
struct FCUBICARWidgetClasses : public FTableRowBase
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> PleaseWaitMessageClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> QuitMessageClass;


	friend UUserInterfaceStatics;
	
};



/**
 *	Static Object to interact with UserInterface Game Objects
 */
UCLASS()
class USERINTERFACE_API UUserInterfaceStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	 *	@brief ShowMainMenu Function
	 *	Shows the main menu, for local Player controller
	 */
	UFUNCTION(BlueprintCallable, Category = "CUBICAR", meta = (WorldContext = "WorldContextObject"))
		static void ShowMainMenu(const UObject* WorldContextObject, const FCUBICARWidgetClasses &WidgetClasses, FWidgetsContainerCUBICAR &Widgets);

	/**
	 * @brief CreateMainMenu Function
	 * Spawns a main menu if necessary (none exists)
	 * @return true if it created a menu or one ealready existed.
	 */
	//UFUNCTION(BlueprintCallable, Category = "CUBICAR", meta = (WorldContext = "WorldContextObject"))
	UFUNCTION(Category = "CUBICAR", meta = (WorldContext = "WorldContextObject"))
		static UUserWidget * CreateMainMenu(const UObject* WorldContextObject, const FCUBICARWidgetClasses &Widgets);

	/**
	 * @brief ShowQuitGameConfirmationMessage Function
	 * Shows A message asking for confirmaktion of Quit command, for local Player controller
	 */
	//UFUNCTION(BlueprintCallable, Category = "CUBICAR", meta = (WorldContext = "WorldContextObject"))
		static UUserWidget * ShowQuitGameConfirmationMessage(const UObject* WorldContextObject, const FCUBICARWidgetClasses &WidgetClasses, FWidgetsContainerCUBICAR &Widgets);

	/**
	 *	@brief ShowQuitGameConfirmationMessage Function
	 *	Shows A message asking for confirmation of Quit command, for local Player controller
	 */
	UFUNCTION(BlueprintCallable, Category = "CUBICAR", meta = (WorldContext = "WorldContextObject"))
		static void CancelQuitGameConfirmationMessage(const UObject* WorldContextObject, const FCUBICARWidgetClasses &WidgetClasses, FWidgetsContainerCUBICAR &Widgets);

	/**
	* @brief ShowQuitGameConfirmationMessage Function
	* Shows A message that locks the menu, for local Player controller
	*/
	UFUNCTION(BlueprintCallable, Category = "CUBICAR", meta = (WorldContext = "WorldContextObject"))
		static UUserWidget * ShowPleaseWaitMessage(const UObject* WorldContextObject, const FCUBICARWidgetClasses &WidgetClasses, FWidgetsContainerCUBICAR &Widgets);

	UFUNCTION(BlueprintCallable, Category = "CUBICAR", meta = (WorldContext = "WorldContextObject"))
		static void ShowLoadingScreen(const UObject* WorldContextObject, const FCUBICARWidgetClasses &WidgetClasses, FWidgetsContainerCUBICAR &Widgets);


	UFUNCTION(BlueprintCallable, Category = "UserInterfacke") //  meta = (WorldContext = "WorldContextObject"))
		static int ValidateNumber(const FText& Text);

	UFUNCTION(BlueprintCallable, Category = "UserInterface")
		static FName ValidateName(const FText& Text);

	UFUNCTION(BlueprintCallable, Category = "CUBICAR", meta = (WorldContext = "WorldContextObject"))
		static FWidgetsContainerCUBICAR FromCurrentWidgets(const UObject* WorldContextObject, bool TopLevelOnly);

	UFUNCTION(BlueprintCallable, Category = "CUBICAR")
		static FCUBICARWidgetClasses GetCUBICARWidgetClasses(bool &IsValid);

	UFUNCTION(BlueprintPure, Category = "CUBICAR")
		static UUserWidget *  GetMainMenu(const FWidgetsContainerCUBICAR &Widgets);

	UFUNCTION(BlueprintPure, Category = "CUBICAR")
		static UUserWidget *  GetPleaseWait(const FWidgetsContainerCUBICAR &Widgets);

	UFUNCTION(BlueprintPure, Category = "CUBICAR")
		static UUserWidget *  GetQuitMessage( const FWidgetsContainerCUBICAR &Widgets);

};
