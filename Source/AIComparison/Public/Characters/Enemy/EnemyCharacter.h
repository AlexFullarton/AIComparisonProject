// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GameCharacter.h"
#include "EnemyControllers/StateMachine/EnemyControllerFSM.h"
#include "EnemyControllers/BehaviourTree/EnemyControllerBT.h"
#include "Components/WidgetComponent.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class AICOMPARISON_API AEnemyCharacter : public AGameCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	virtual void CharacterDeath() override;
	virtual void ModifyHealth(float healthToSubtract) override;

	// Widget component for healthbar
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UWidgetComponent* healthbarWidget;

	// healthbar widget
	UEnemyHealthbar* healthbarObject;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called just before begin play - allows early initialisation of enemies
	void InitialiseEnemy();

	void RagdollDeath();
};
