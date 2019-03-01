// Fill out your copyright notice in the Description page of Project Settings.

#include "Checkpoint.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "RacePlayerState.h"
#include "Components/CapsuleComponent.h"
#include "Engine/StaticMesh.h"


ACheckpoint::ACheckpoint(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer), OverlapDimension(100.f)
{
	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	CheckpointMesh->SetupAttachment(GetCapsuleComponent());
	CheckpointMesh->SetRelativeLocation(FVector(0, 0, -1 * GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()),false,nullptr, ETeleportType::None);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollsionTrigger"));
	//CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetRelativeLocation(FVector(0.f), false, nullptr, ETeleportType::None);

	if(CheckpointMesh->GetStaticMesh())
		CollisionBox->InitBoxExtent(CheckpointMesh->GetStaticMesh()->ExtendedBounds.BoxExtent);


	// Connect the event to our player detection functions.
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnBeginOverlap);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnEndOverlap);
	CollisionBox->SetBoxExtent(OverlapDimension);


	// Bind default behaviour function to Multicast 
	OnCheckpointEntered.AddDynamic(this, &ACheckpoint::OnCarEntered);
	OnCheckpointLeft.AddDynamic(this, &ACheckpoint::OnCarLeft);

}

void ACheckpoint::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	//ActorsInside.Add(OtherActor);
	const auto Pawn = Cast<APawn>(OtherActor);
	if (Pawn)
		OnCheckpointEntered.Broadcast(Pawn);

}

void ACheckpoint::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//ActorsInside.Remove(OtherActor);
	const auto Pawn = Cast<APawn>(OtherActor);
	if (Pawn)
		OnCheckpointLeft.Broadcast(Pawn);
}

void ACheckpoint::OnCarEntered(APawn* Pawn)
{
	const auto PlayerState  = Cast<ARacePlayerState>(Pawn->PlayerState);
	if (!PlayerState)
		return;
	PlayerState->OnPassedCheckpoint(this);
}

void ACheckpoint::OnCarLeft(APawn* Pawn)
{

}

