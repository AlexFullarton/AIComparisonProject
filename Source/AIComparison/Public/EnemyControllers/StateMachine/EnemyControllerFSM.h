// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyControllers/EnemyController.h"
#include "EnemyControllers/StateMachine/EnemyState.h"
#include "EnemyControllerFSM.generated.h"

class EnemyState;

UCLASS()
class AICOMPARISON_API AEnemyControllerFSM : public AEnemyController
{
	GENERATED_BODY()

public:
	AEnemyControllerFSM();

	virtual void Tick(float DeltaTime);

	inline EnemyState* getCurrentState() const { return currentState; }
	void updateState();
	void setState(EnemyState& newState);

private:
	EnemyState* currentState;
};
