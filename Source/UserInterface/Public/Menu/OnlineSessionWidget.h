// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUBICARWidget.h"
#include "CUBICARStatics.h"
#include "OnlineSessionWidget.generated.h"


class UCUBICARButton;

/**
 * 
 */
UCLASS()
class USERINTERFACE_API UOnlineSessionWidget : public UCUBICARWidget
{
	GENERATED_BODY()
public :
	UOnlineSessionWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	UOnlineSessionWidget(const FServerStatus &SessionSettings, const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected :
	UPROPERTY()
		FServerStatus Session;

	UPROPERTY(meta = (BindWidget))
		UCUBICARButton * JoinSession;
	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock * ServerName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock * Players;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock * Spectators;

	void BindDelegates() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Session")
		void SetSession(const FServerStatus &NewSession);




private:

	UFUNCTION()
		void RequestJoin();

	
	
	
};
