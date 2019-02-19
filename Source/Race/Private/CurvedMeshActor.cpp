// Fill out your copyright notice in the Description page of Project Settings.

#include "CurvedMeshActor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/CollisionProfile.h"

#if WITH_EDITORONLY_DATA
#include "Engine/Selection.h"
#endif


// Sets default values
ACurvedMeshActor::ACurvedMeshActor()
{

#if WITH_EDITORONLY_DATA
	USelection::SelectObjectEvent.AddUObject(this, &ACurvedMeshActor::OnObjectSelected);
#endif


	PrimaryActorTick.bCanEverTick = false;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	RootComponent = Spline;
	//Spline->SetupAttachment(RootComponent);
	if (!Mesh)
		return;
	MeshDimension = Mesh->ExtendedBounds.BoxExtent;

	// Spline 
	if (!Spline)
		return;


}

void ACurvedMeshActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//SetupSplineMeshes();
}

void ACurvedMeshActor::SetupSplineMeshes()
{
	// Spline exists ?
	if (!Spline)
		return;
	if (!Mesh)
		return;


	const auto MeshLength = Mesh->ExtendedBounds.BoxExtent.X;
	SectionLength =  GetActorScale3D().X * SectionLengthDelta;
	const int32 LastIndex = UKismetMathLibrary::FTrunc(Spline->GetSplineLength() / SectionLength);

	for (int32 id = 0; id <= LastIndex; id++)
	{
		const FTransform SpawnTransform = FTransform();
		const auto NewComp = AddSplineMeshComponent(id);
		if (!NewComp)
			return;
		UE_LOG(LogTemp, Warning, TEXT("This Worked !!!!!!"));
		UpdateCurvedMesh(id, NewComp, false);
		SplineMeshes.Add(NewComp);
	}

	const int32 Points = Spline->GetNumberOfSplinePoints();


#if 0
	// use already present meshes : Only update their position, remove the unnecessary surplus, add if missing
	
	for (int32 pid = 0; pid < Points; pid++)
	{
			if(SplineMeshes.Num() <= pid)
			{
				// Spawn curved mesh
				FString NewName = TEXT("CurvedMesh");
				NewName.AppendInt(pid);
				auto NewCurvedMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass(), FName(*NewName), EObjectFlags::RF_Dynamic, nullptr, true);

				if (!NewCurvedMesh) // Safety first
					continue;

				NewCurvedMesh->RegisterComponent();
				NewCurvedMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				UpdateCurvedMesh(pid, NewCurvedMesh);

				SplineMeshes.Add(NewCurvedMesh);
				continue;
			}

			if(SplineMeshes[pid])
				UpdateCurvedMesh(pid, SplineMeshes[pid]);
	}
	const int32 ArrayLength = SplineMeshes.Num();
	if (ArrayLength >= Points)
	{
		for (int32 pid = Points-1; pid < ArrayLength; pid ++)
		{
			const auto mesh = SplineMeshes[pid];
			if (mesh)
				mesh->DestroyComponent(true);
			SplineMeshes.Remove(mesh);
		}
	}
#endif
}

void ACurvedMeshActor::UpdateCurvedMesh(int32 SplineIndex, USplineMeshComponent* CurvedMesh, bool UpdateSectionLenght)
{
	// pointer Protection 
	if (!CurvedMesh)
		return;

	// don't do anything if no further data
	if (SplineIndex >= Spline->GetNumberOfSplinePoints()+1)
		return;
	if(Mesh)
	{
		if (UpdateSectionLenght)
		{
			MeshDimension = Mesh->ExtendedBounds.BoxExtent;
			SectionLength = MeshDimension.X;
		}
		CurvedMesh->SetStaticMesh(Mesh);
	}
	CurvedMesh->SetForwardAxis(ESplineMeshAxis::X);
	CurvedMesh->Mobility = EComponentMobility::Static;

	// End and start
	const float SplineDistanceStart = SplineIndex * SectionLength;
	const float SplineDistanceEnd = (SplineIndex + 1 ) * SectionLength;

	FVector Start = Spline->GetLocationAtDistanceAlongSpline(SplineDistanceStart, ESplineCoordinateSpace::World);
	FVector End = Spline->GetLocationAtDistanceAlongSpline(SplineDistanceEnd, ESplineCoordinateSpace::World);
	FVector StartTangent =Spline->GetTangentAtSplinePoint(SplineDistanceStart, ESplineCoordinateSpace::World);
	FVector EndTangent = Spline->GetTangentAtSplinePoint(SplineDistanceEnd, ESplineCoordinateSpace::World);

	CurvedMesh->SetRelativeLocation(Start);

	// Position
	CurvedMesh->SetStartAndEnd(Start, StartTangent.GetClampedToSize(0.f, SectionLength), End, EndTangent.GetClampedToSize(0.f, SectionLength));

	CurvedMesh->UpdateMesh();
}

void ACurvedMeshActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	//SetupSplineMeshes();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#if WITH_EDITORONLY_DATA
void ACurvedMeshActor::OnObjectSelected(UObject* Object)
{
	if (Object == this)
	{
		// we just got selected
		SetupSplineMeshes();
		return;
	}
	if (!IsSelected())
	{
	  // deselection just happened
	}
}
#endif


USplineMeshComponent* ACurvedMeshActor::AddSplineMeshComponent_Implementation(int index)
{
	if (!GetWorld())
		return nullptr;
	if (GetWorld()->bIsTearingDown)
	{
		UE_LOG(LogActor, Warning, TEXT("AddComponent failed because we are in the process of tearing down the world"));
		return nullptr;
	}
	// Spawn curved mesh
	FString NewName = TEXT("CurvedMesh");
	NewName.AppendInt(index);
	auto NewCurvedMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass(), FName(*NewName));

	if (!NewCurvedMesh) // Safety first
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't Spawn New Component"))
		return nullptr;
	}
	NewCurvedMesh->SetForwardAxis(ESplineMeshAxis::Y, true);
	NewCurvedMesh->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	NewCurvedMesh->Mobility = EComponentMobility::Static;
	NewCurvedMesh->SetGenerateOverlapEvents(false);
	NewCurvedMesh->bAllowSplineEditingPerInstance = true;
	NewCurvedMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	NewCurvedMesh->RegisterComponent();

	
	return NewCurvedMesh;
}


