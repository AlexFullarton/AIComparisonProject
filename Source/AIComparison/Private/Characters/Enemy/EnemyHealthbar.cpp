// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyHealthbar.h"

UEnemyHealthbar::UEnemyHealthbar(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

void UEnemyHealthbar::SetMaxHealth(float max)
{
	MaxHealth = max;
}

void UEnemyHealthbar::UpdateHealth(float currentHealth)
{
	HealthProgress = MapRangeClamped(currentHealth, 0.0f, MaxHealth, 0.0f, 1.0f);
}

float UEnemyHealthbar::MapRangeClamped(float value, float inRangeA, float inRangeB, float outRangeA, float outRangeB)
{
	if (outRangeA == outRangeB)
		return outRangeA;
	float inPercentage = (value - inRangeA) / (inRangeB - inRangeA);
	if (inPercentage < 0.0f) 
		return outRangeA;
	if (inPercentage > 1.0f)
		return outRangeB;
	return outRangeA + (inPercentage * (outRangeB - outRangeA));
}

