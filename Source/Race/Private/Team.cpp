// Fill out your copyright notice in the Description page of Project Settings.

#include "Team.h"
#include "UnrealNetwork.h"


// Sets default values for this component's properties
UTeam::UTeam()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTeam::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTeam::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UTeam::BuyItem(const UObject* ItemToPurchase)
{
	return false;
}


void UTeam::GetLifetimeReplicatedProps(class TArray<class FLifetimeProperty, class FDefaultAllocator> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTeam, Info);
	DOREPLIFETIME(UTeam, Money);
	DOREPLIFETIME(UTeam, PurchasedItems);
}


