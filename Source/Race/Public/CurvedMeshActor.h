// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CurvedMeshActor.generated.h"


class USplineComponent;
class USplineMeshComponent;
class UStaticMesh;

UCLASS()
class ACurvedMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACurvedMeshActor();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	/** Meshes Spawned to fit the curve */
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Spline", meta = (DisplayName = "Spline Meshes"))
		TArray<USplineMeshComponent *> SplineMeshes;


	/** Dimension of the unmodified mesh */
	UPROPERTY()
	FVector MeshDimension;

	UPROPERTY()
		float SectionLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
		float SectionLengthDelta = 100;

protected:

	/** Spline to drive mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline", meta = (DisplayName = "Spline"))
		USplineComponent * Spline;


	/** Mesh to use for spline */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (DisplayName = "Curved Static Mesh"))
		UStaticMesh * Mesh;

	UFUNCTION(BlueprintCallable, Category = "Spline")
		void SetupSplineMeshes();

	UFUNCTION(BlueprintCallable, Category = "Spline", meta = (DisplayName = "Update Curved Mesh"))
		void UpdateCurvedMesh(int32 SplineIndex, USplineMeshComponent * CurvedMesh, bool UpdateSectionLenght);

	void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;

#if WITH_EDITORONLY_DATA
	void OnObjectSelected(UObject* Object);
#endif

protected :

	/** AddSplineMeshComponent : Add Spline Mesh */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spline")
		USplineMeshComponent * AddSplineMeshComponent(int index);
	
};
