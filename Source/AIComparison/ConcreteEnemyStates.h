// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyState.h"
#include "EnemyControllerFSM.h"

class EnemyPatrolState : public EnemyState
{
public:
	virtual void enterState(AEnemyControllerFSM* enemy) {};
	virtual void updateState(AEnemyControllerFSM* enemy);
	virtual void exitState(AEnemyControllerFSM* enemy) {};
	static EnemyState& getInstance();
	
private:
	EnemyPatrolState() {}
	//EnemyPatrolState(const EnemyPatrolState& state);
	//EnemyPatrolState& operator=(const EnemyPatrolState& state);
};

class EnemyChaseState : public EnemyState
{
public:
	virtual void enterState(AEnemyControllerFSM* enemy) {};
	virtual void updateState(AEnemyControllerFSM* enemy);
	virtual void exitState(AEnemyControllerFSM* enemy) {};
	static EnemyState& getInstance();

private:
	EnemyChaseState() {}
	//EnemyChaseState(const EnemyChaseState& state);
	//EnemyChaseState& operator=(const EnemyChaseState& state);
};

class EnemyAttackState : public EnemyState
{
public:
	virtual void enterState(AEnemyControllerFSM* enemy) {};
	virtual void updateState(AEnemyControllerFSM* enemy);
	virtual void exitState(AEnemyControllerFSM* enemy) {};
	static EnemyState& getInstance();

private:
	EnemyAttackState() {}
	//EnemyAttackState(const EnemyAttackState& state);
	//EnemyAttackState& operator=(const EnemyAttackState& state);
};


class EnemyRetreatState : public EnemyState
{
public:
	virtual void enterState(AEnemyControllerFSM* enemy) {};
	virtual void updateState(AEnemyControllerFSM* enemy);
	virtual void exitState(AEnemyControllerFSM* enemy) {};
	static EnemyState& getInstance();

private:
	EnemyRetreatState() {}
	//EnemyRetreatState(const EnemyRetreatState& state);
	//EnemyRetreatState& operator=(const EnemyRetreatState& state);
};

class EnemyDefendState : public EnemyState
{
public:
	virtual void enterState(AEnemyControllerFSM* enemy) {};
	virtual void updateState(AEnemyControllerFSM* enemy);
	virtual void exitState(AEnemyControllerFSM* enemy) {};
	static EnemyState& getInstance();

private:
	EnemyDefendState() {}
	//EnemyDefendState(const EnemyDefendState& state);
	//EnemyDefendState& operator=(const EnemyDefendState& state);
};
