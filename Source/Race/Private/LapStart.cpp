// Fill out your copyright notice in the Description page of Project Settings.

#include "LapStart.h"
#include "Kismet/GameplayStatics.h"
#include "PitLanePosition.h"


void ALapStart::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> pits;
	UGameplayStatics::GetAllActorsOfClass(this, APitLanePosition::StaticClass(), pits);
	PitPositions = reinterpret_cast<TArray<APitLanePosition *>&>(pits) ;
}
