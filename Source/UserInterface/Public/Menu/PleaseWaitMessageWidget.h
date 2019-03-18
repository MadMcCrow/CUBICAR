// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModalWindowWidget.h"
#include "UserInterfaceStatics.h"
#include "PleaseWaitMessageWidget.generated.h"




/**
 * 
 */
UCLASS()
class USERINTERFACE_API UPleaseWaitMessageWidget : public UCUBICARWidget
{
	GENERATED_BODY()
private:

	virtual void UnlockUI(FWidgetsContainerCUBICAR &Widgets);
	virtual void LockUI(FWidgetsContainerCUBICAR &Widgets);
	
public :
	
	UFUNCTION(BlueprintCallable)
		virtual void EndPleaseWait();

protected:

	UFUNCTION(BlueprintNativeEvent)
		void DestroyFromParent();


	void BindDelegates() override;

	
};
