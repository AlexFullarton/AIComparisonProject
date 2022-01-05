// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "BehaviourTree.h"
#include "EnemyControllerBT.generated.h"

/**
 * 
 */
UCLASS()
class AICOMPARISON_API AEnemyControllerBT : public AEnemyController
{
	GENERATED_BODY()

public:
	AEnemyControllerBT();
	
private:
	BehaviourTree behaviourTree;

	bool CalculateNewPatrolLocation();
	bool ArrivedAtPatrolLocation();
	bool MoveToPatrolLocation();
};