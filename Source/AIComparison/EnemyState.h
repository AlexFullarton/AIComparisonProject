// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AEnemyControllerFSM;

// Pure abstract class - only instances of child classes of this will be created
class EnemyState
{
public:
	virtual void enterState(AEnemyControllerFSM* enemy) = 0;
	virtual void toggleState(AEnemyControllerFSM* enemy) = 0;
	virtual void exitState(AEnemyControllerFSM* enemy) = 0;
	virtual ~EnemyState() {}
};
