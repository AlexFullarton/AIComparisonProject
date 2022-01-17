// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthbar.generated.h"

class AEnemyCharacter;

UCLASS()
class AICOMPARISON_API UEnemyHealthbar : public UUserWidget
{
	GENERATED_BODY()
public:
	UEnemyHealthbar(const FObjectInitializer& ObjectInitializer);

public:
	void SetMaxHealth(float max);

	void UpdateHealth(float currentHealth);

	float MapRangeClamped(float value, float inRangeA, float inRangeB, float outRangeA, float outRangeB);

	float MaxHealth;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float HealthProgress;
};
