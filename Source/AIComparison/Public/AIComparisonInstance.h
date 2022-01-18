// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <numeric>
#include <algorithm>
#include "AIComparisonInstance.generated.h"

UCLASS()
class AICOMPARISON_API UAIComparisonInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UAIComparisonInstance();
	
	UFUNCTION(BlueprintCallable)
	void ResetInstance();
	UFUNCTION(BlueprintCallable)
	void CalculateAverageFPS();
	UFUNCTION(BlueprintCallable)
	void CalculateStandardDeviationFPS();

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
	float highestFPS;
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	float lowestFPS;
	// Average fps, calculated on quit or return to menu
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	float averageFPS;
	// Standard deviation of the collected fps values
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	float stdDevFPS;
	// array of each ticks fps data
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	TArray<float> FPSValues;

	// Gate bool for gathering data
	UPROPERTY(BlueprintReadWrite, Category = Globals)
	bool shouldGatherData;
};
