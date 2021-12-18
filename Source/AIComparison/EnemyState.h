// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AEnemyController;

// Pure abstract class - only instances of child classes of this will be created
class EnemyState
{
public:
	virtual void enterState(AEnemyController* enemy) = 0;
	virtual void toggleState(AEnemyController* enemy) = 0;
	virtual void exitState(AEnemyController* enemy) = 0;
	virtual ~EnemyState() {}
};
