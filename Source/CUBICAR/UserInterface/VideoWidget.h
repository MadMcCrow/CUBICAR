// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUBICARWidget.h"
#include "VideoWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVideoWidgetDelegates);

/**
 * 
 */
UCLASS()
class CUBICAR_API UVideoWidget : public UCUBICARWidget
{
	GENERATED_BODY()

public :

	UVideoWidget();

	UPROPERTY(meta = (BindWidget))
		class UImage * VideoScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video", Instanced)
		class UMediaPlayer * VideoPlayer;


	UPROPERTY( BlueprintReadOnly, Category = "Video", Instanced)
		class UMediaTexture * VideoTexture;

protected:

	UPROPERTY(BlueprintAssignable, Category = "Event")
		FVideoWidgetDelegates PlayDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Event")
		FVideoWidgetDelegates PauseDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Event")
		FVideoWidgetDelegates EndOfVideo;


	void BindDelegates() override;

protected:

	UFUNCTION()
		void Play();

	UFUNCTION()
		void Pause();

	UFUNCTION()
		void Skip();

	UFUNCTION()
		void ReachedEndOfVideo();

	UFUNCTION()
		FSlateBrush SetVideoBinding();

	
	
};
