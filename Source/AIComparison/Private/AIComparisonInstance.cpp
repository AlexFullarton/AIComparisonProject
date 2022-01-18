// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComparisonInstance.h"

UAIComparisonInstance::UAIComparisonInstance()
{
	// Set all values to minimum
	EnemyCount = 5;
	EnemyBehaviourType = 0;
	PlayerInitialHealth = 100.0f;
	PlayerMeleeDamage = 30.0f;
	PlayerRangedDamage = 50.0f;
	EnemyInitialHealth = 100.0f;
	EnemyMeleeDamage = 10.0f;
	EnemyRangedDamage = 20.0f;

	// Enemies killed to zero - acts as a counter for game end trigger
	EnemiesKilled = 0;

	highestFPS = 0.0f;
	lowestFPS = 999999.0f;

	shouldGatherData = false;
}

void UAIComparisonInstance::ResetInstance()
{
	// Set all values to minimum
	EnemyCount = 5;
	EnemyBehaviourType = 0;
	PlayerInitialHealth = 100.0f;
	PlayerMeleeDamage = 30.0f;
	PlayerRangedDamage = 50.0f;
	EnemyInitialHealth = 100.0f;
	EnemyMeleeDamage = 10.0f;
	EnemyRangedDamage = 20.0f;

	// Enemies killed to zero - acts as a counter for game end trigger
	EnemiesKilled = 0;

	highestFPS = 0.0f;
	lowestFPS = 999999.0f;
	averageFPS = 0.0f;
	stdDevFPS = 0.0f;
	FPSValues.Empty();

	shouldGatherData = false;
}

void UAIComparisonInstance::CalculateAverageFPS()
{
	// Sum all the recorded fps values, then divide by the amount
	averageFPS = 0.0f;
	for (auto result : FPSValues)
		averageFPS += result;
	averageFPS = averageFPS / FPSValues.Num();
}

void UAIComparisonInstance::CalculateStandardDeviationFPS()
{
	// Use sample version of standard deviation equation as the mean has been calculated from generated data
	float sum = 0.0f;
	for (auto result : FPSValues)
		sum += (result - averageFPS) * (result - averageFPS);
	stdDevFPS = sqrt(sum / (FPSValues.Num() - 1));
}

