// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Checkpoint.generated.h"


class UBoxComponent;
class ACheckpoint;
class UStaticMeshComponent;
class APawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCheckpointCarEnteredDelegate, APawn*, Pawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCheckpointCarLeftDelegate, APawn*, Pawn);


/**
 *	Base Class for the Track Checkpoint and the Start/End/Loop Point
 */
UCLASS(BlueprintType)
class RACE_API ACheckpoint : public APlayerStart
{
	GENERATED_BODY()

public :
	ACheckpoint(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Components ---------------------------------
private :
	/** mesh for the Checkpoint */
	UPROPERTY(Category = "Race|Mesh", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent * CheckpointMesh;

	/** mesh for the Checkpoint */
	UPROPERTY(Category = "Race|Collision", EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent * CollisionBox;
protected:

	UPROPERTY(Category = "Race|Overlap", EditDefaultsOnly, BlueprintReadOnly)
		FVector OverlapDimension;



	// Index ---------------------------------------
protected:
	/** List of Player that passed through the Checkpoint*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Race|Checkpoint", meta = (DisplayName = "Checkpoint Index"))
		int Index;
public:
	UFUNCTION(BlueprintPure, Category = "Checkpoint")
		inline int GetIndex() const { return Index; }

protected:
#if 0 // Check player passed ... Should be done on an actor that is always on server : ie player and game states
	/** List of Player that passed through the Checkpoint*/
	UPROPERTY(BlueprintReadOnly, Category = "Checkpoint")
		TMap<APlayerController* , FDateTime> PlayerPassed;

	/** List of Actors Currently Inside the Checkpoint*/
	UPROPERTY(BlueprintReadOnly, Category = "Checkpoint")
		TArray<AActor *> ActorsInside;


#endif //0 

	// Player Detection  ------------------------------------
	
	/**
	 *	OnBeginOverlap
	 *	@brief Called when something enters the checkpoint
	 *	@param OverlappedActor
	 *	@param OtherActor
	 */
	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	/**
	 *	OnEndOverlap
	 *	@brief Called when something exits the checkpoint
	 *	@param OverlappedActor
	 *	@param OtherActor
	 */
	UFUNCTION()
		virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	/** Event for when a player enters the checkpoint **/
	UPROPERTY(BlueprintAssignable, Category = "Race|Event|Checkpoint", meta = (DisplayName = "Player Entered Checkpoint"))
	FCheckpointCarEnteredDelegate OnCheckpointEntered;

	/** Event for when a player leaves the checkpoint **/
	UPROPERTY(BlueprintAssignable, Category = "Race|Event|Checkpoint", meta = (DisplayName = "Player Left Checkpoint"))
	FCheckpointCarLeftDelegate	OnCheckpointLeft;

	/**
	 *	OnPlayeredEntered
	 *	@brief Called when a Player enters the Checkpoint
	 */
	UFUNCTION()
	void OnCarEntered(APawn * Pawn);

	/**
	 *	OnPlayeredLeft
	 *	@brief Called when a Player Leaves the Checkpoint
	 */
	UFUNCTION()
	void OnCarLeft(APawn * Pawn);

	
	// Comparison Operator ----------------------------------------------------------------
public:

	inline bool operator==( const  ACheckpoint rhs) const { return Index == rhs.Index; }
	inline bool operator!=( const  ACheckpoint rhs) const { return Index != rhs.Index; }
	inline bool operator< ( const  ACheckpoint rhs) const { return Index <  rhs.Index; }
	inline bool operator> ( const  ACheckpoint rhs) const { return Index >  rhs.Index; }
	inline bool operator<=( const  ACheckpoint rhs) const { return Index <= rhs.Index; }
	inline bool operator>=( const  ACheckpoint rhs) const { return Index >= rhs.Index; }


	static bool SortCheckpoint(const ACheckpoint& A, const ACheckpoint& B) // Sort Checkpoints by Index
	{
		return A.GetIndex() < B.GetIndex();
	}
	
};
