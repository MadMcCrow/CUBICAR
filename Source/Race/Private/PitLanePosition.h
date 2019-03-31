// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "PitLanePosition.generated.h"

class ALapStart;

/**
 *	PitLane Start position for cars
 */
UCLASS()
class RACE_API APitLanePosition : public APlayerStart
{
	GENERATED_BODY()

public :

	APitLanePosition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure)
		UCapsuleComponent * GetCapsule() const { return GetCapsuleComponent(); }

private :

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Decal", meta = (AllowPrivateAccess = "true"))
		class UDecalComponent * StartingDecal;



protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Decal")
		class UMaterial * DecalMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Index;

public:

	FORCEINLINE int GetIndex() const { return Index; }

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
		bool IsAvailable();

private :

	friend ALapStart;

	// Comparison Operator ----------------------------------------------------------------
public:

	inline bool operator==(const  APitLanePosition rhs) const { return Index == rhs.Index; }
	inline bool operator!=(const  APitLanePosition rhs) const { return Index != rhs.Index; }
	inline bool operator< (const  APitLanePosition rhs) const { return Index < rhs.Index; }
	inline bool operator> (const  APitLanePosition rhs) const { return Index > rhs.Index; }
	inline bool operator<=(const  APitLanePosition rhs) const { return Index <= rhs.Index; }
	inline bool operator>=(const  APitLanePosition rhs) const { return Index >= rhs.Index; }


	static bool Sort(const APitLanePosition& A, const APitLanePosition& B) // Sort Checkpoints by Index
	{
		return A.GetIndex() < B.GetIndex();
	}


};
