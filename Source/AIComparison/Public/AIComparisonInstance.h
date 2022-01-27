// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <numeric>
#include <algorithm>
#include "AIComparisonInstance.generated.h"

USTRUCT(BlueprintType)
struct FCollectedData
{
	GENERATED_BODY()
public:
	FCollectedData();

	void Reset();

	void CalculateAverage();
	void CalculateStandardDeviation();

	UPROPERTY(BlueprintReadWrite, Category = Globals)
	float highest;
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	float lowest;
	// Average fps, calculated on quit or return to menu
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	float average;
	// Standard deviation of the collected fps values
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	float stdDev;
	// array of each ticks fps data
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	TArray<float> Values;
};

UCLASS()
class AICOMPARISON_API UAIComparisonInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UAIComparisonInstance();
	
	UFUNCTION(BlueprintCallable)
	void ResetInstance();

	void CalculateDataAverages();
	void CalculateDataStdDevs();

	UFUNCTION(BlueprintCallable)
	void RecordFPSData(float deltaTime);
	UFUNCTION(BlueprintCallable)
	void RecordCPUPercentageData();
	UFUNCTION(BlueprintCallable)
	void RecordMemoryUseageData();

	UPROPERTY(BlueprintReadWrite, Category = Globals)
	int EnemyCount;
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	int EnemyBehaviourType;
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	float PlayerInitialHealth;
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	float PlayerMeleeDamage;
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	float PlayerRangedDamage;
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	float EnemyInitialHealth;
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	float EnemyMeleeDamage;
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	float EnemyRangedDamage;
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	int EnemiesKilled;

	// Profiling data
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	FCollectedData FPSData;
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	FCollectedData CPUPercentageData;
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	FCollectedData MemoryData;

	// Gate bool for gathering data
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	bool shouldGatherData;

	// conversion from bytes to gigabytes - used for manipulating RAM data
	const static int ByteToGigabyte = 1073741824;
};
