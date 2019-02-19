// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RaceTime.generated.h"

/**
 *	Simplified Time Date Structure
 */
USTRUCT(BlueprintType, meta = (DisplayName = "Time"))
struct FRaceTime
{
	GENERATED_BODY()



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Milliseconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Seconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Minutes;

	FRaceTime(int milliseconds,	int seconds, int minutes) : Milliseconds(milliseconds), Seconds(seconds), Minutes(minutes)
	{}

	FRaceTime(float RealTime = 0)
	{
		UpdateFromRealTime(RealTime);
	}

	// Update
	void UpdateFromRealTime(float RealTime)
	{
		Minutes = int(RealTime / 60);
		Seconds = int(RealTime);
		Milliseconds= int(RealTime * 1000) % 1000;

	}

	float GetRealTime() const
	{
		return Minutes * 60.f + Milliseconds / 1000.f + Seconds;
	}
	 
	static FRaceTime Now()
	{
		FRaceTime now;
		now.SetFromDateTime(FDateTime::Now());
	}

	static FRaceTime SinceRaceStart(const UObject* WorldContextObject);

	// Setters
	void SetFromRealTimeSeconds(float NewTime) { UpdateFromRealTime(NewTime);}
	void AddDeltaSeconds(float DeltaSeconds) { UpdateFromRealTime(GetRealTime() + DeltaSeconds);}
	void SetFromDateTime(const FDateTime &Date)
	{
		Milliseconds = Date.GetMillisecond();
		Minutes = Date.GetMinute();
		Seconds = Date.GetSecond();
	}

	// Getters
	int GetMillisecondsTruncated() const { return Milliseconds; }
	int GetSecondsTruncated() const { return Seconds; }
	int GetMinutesTruncated() const { return Minutes; }

	FDateTime GetAsDateTime() const	{ return FDateTime(0,0,0,0,Minutes,Seconds,Milliseconds);	}

	inline bool operator==(const  FRaceTime rhs) const { return GetRealTime() == rhs.GetRealTime(); }
	inline bool operator!=(const  FRaceTime rhs) const { return GetRealTime() != rhs.GetRealTime(); }
	inline bool operator< (const  FRaceTime rhs) const { return GetRealTime() < rhs.GetRealTime();  }
	inline bool operator> (const  FRaceTime rhs) const { return GetRealTime() > rhs.GetRealTime();  }
	inline bool operator<=(const  FRaceTime rhs) const { return GetRealTime() <= rhs.GetRealTime(); }
	inline bool operator>=(const  FRaceTime rhs) const { return GetRealTime() >= rhs.GetRealTime(); }
};




/**
 *	Library to Use RaceTime Structure in Blueprint
 */
UCLASS()
class RACE_API URaceTimeLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static void SetFromRealTimeSeconds(FRaceTime Time, float NewTime) { Time.UpdateFromRealTime(NewTime); }

	UFUNCTION(BlueprintCallable)
	static void AddDeltaSeconds(FRaceTime Time, float DeltaSeconds) { Time.UpdateFromRealTime(Time.GetRealTime() + DeltaSeconds); }

	UFUNCTION(BlueprintCallable)
	static void SetFromDateTime(FRaceTime Time, const FDateTime &Date) { Time.SetFromDateTime(Date); }
	
};