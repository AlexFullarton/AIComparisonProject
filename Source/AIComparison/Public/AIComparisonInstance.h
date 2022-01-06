// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AIComparisonInstance.generated.h"

/**
 * 
 */
UCLASS()
class AICOMPARISON_API UAIComparisonInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UAIComparisonInstance();

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
};
