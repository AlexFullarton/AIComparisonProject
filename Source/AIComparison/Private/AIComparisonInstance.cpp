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

	FPSData.Reset();
	CPUPercentageData.Reset();
	MemoryData.Reset();

	shouldGatherData = false;
}

void UAIComparisonInstance::CalculateDataAverages()
{
	FPSData.CalculateAverage();
	CPUPercentageData.CalculateAverage();
	MemoryData.CalculateAverage();
}

void UAIComparisonInstance::CalculateDataStdDevs()
{
	FPSData.CalculateStandardDeviation();
	CPUPercentageData.CalculateStandardDeviation();
	MemoryData.CalculateStandardDeviation();
}

void UAIComparisonInstance::RecordFPSData(float DeltaTime)
{
	float lastFPS = 1.0f / DeltaTime;
	if (lastFPS > FPSData.highest)
		FPSData.highest = lastFPS;
	else if (lastFPS < FPSData.lowest)
		FPSData.lowest = lastFPS;
	FPSData.Values.Add(lastFPS);
}

void UAIComparisonInstance::RecordCPUPercentageData()
{
	FCPUTime lastCPUTime = FWindowsPlatformTime::GetCPUTime();
	if (lastCPUTime.CPUTimePct > CPUPercentageData.highest)
		CPUPercentageData.highest = lastCPUTime.CPUTimePct;
	else if (lastCPUTime.CPUTimePct < CPUPercentageData.lowest)
		CPUPercentageData.lowest = lastCPUTime.CPUTimePct;
	CPUPercentageData.Values.Add(lastCPUTime.CPUTimePct);
}

void UAIComparisonInstance::RecordMemoryUseageData()
{
	// Get current memory stats - values are in bytes so must convert
	FPlatformMemoryStats lastMemoryStats = FWindowsPlatformMemory::GetStats();
	float memoryUsedInGB = (lastMemoryStats.UsedPhysical * 1.0f)/ ByteToGigabyte;
	if (memoryUsedInGB > MemoryData.highest)
		MemoryData.highest = memoryUsedInGB;
	else if (memoryUsedInGB < MemoryData.lowest)
		MemoryData.lowest = memoryUsedInGB;
	MemoryData.Values.Add(memoryUsedInGB);
}

FCollectedData::FCollectedData()
{
	highest = 0.0f;
	lowest = FLT_MAX;
	average = 0.0f;
	stdDev = 0.0f;
}

void FCollectedData::Reset()
{
	highest = 0.0f;
	lowest = FLT_MAX;
	average = 0.0f;
	stdDev = 0.0f;
	Values.Empty();
}

void FCollectedData::CalculateAverage()
{
	// Sum all the recorded values, then divide by the count
	average = 0.0f;
	for (auto result : Values)
		average += result;
	average = average / Values.Num();
}

void FCollectedData::CalculateStandardDeviation()
{
	// Use sample version of standard deviation equation as the mean has been calculated from generated data
	float sum = 0.0f;
	for (auto result : Values)
		sum += (result - average) * (result - average);
	stdDev = sqrt(sum / (Values.Num() - 1));
}

