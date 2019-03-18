// Fill out your copyright notice in the Description page of Project Settings.

#include "CUBICARWidget.h"

UCUBICARWidget::UCUBICARWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BindDelegates();
}

void UCUBICARWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	BindDelegates(); // Somehow works when set here , at least it never fails to update :)
}

void UCUBICARWidget::PostLoad()
{
	Super::PostLoad();
	//BindDelegates();
}

void UCUBICARWidget::BindDelegates()
{

}

