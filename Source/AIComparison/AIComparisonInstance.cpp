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
}

