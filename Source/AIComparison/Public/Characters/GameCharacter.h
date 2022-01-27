// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/Class.h"
#include "Weapons/MeleeWeapon.h"
#include "Weapons/RangedWeapon.h"
#include "AIComparisonInstance.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Characters/Enemy/EnemyHealthbar.h"
#include "GameCharacter.generated.h"

UCLASS()
class AICOMPARISON_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Functions for movement of the PlayerCharacter
	void MoveForward(float Val);
	void MoveRight(float Val);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	// Function to modify health on damage from another character/object
	UFUNCTION()
	virtual void ModifyHealth(float healthToSubtract);

	// Functions for combat scenarios
	void Block();
	void StopBlocking();
	void Attack();
	void AttackRanged();

	UFUNCTION(BlueprintCallable)
	virtual void FireArrow();

	void SwapWeapons();
	virtual void CharacterDeath();
	void RagdollDeath();

	UFUNCTION(BlueprintCallable)
	void AttackDone();

	UFUNCTION(BlueprintCallable)
	void toggleWeaponCollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Allows this character to be detected by other AI controlled characters
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

	// Equipped weapons
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	UClass* leftHandShield;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	UClass* rightHandSword;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	UClass* leftHandBow;

	//turn rate, in deg/sec
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float TurnRate;

	//look up/down rate, in deg/sec
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float LookUpRate;

	// Control bools for combat - used to enable state change in animation graph
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool isBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool isAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool turnRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool turnLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float maxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float currentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	bool isMelee;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	bool isRanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	bool canFire;

	int attackCounter;

	// Damage modifiers - set from the main menu
	float meleeDamage;
	float rangedDamage;

	// Pointers to players weapons - weapons are destroyed/recreated on weapon swap
	AMeleeWeapon* swordWeapon;
	AMeleeWeapon* shieldWeapon;
	ARangedWeapon* bowWeapon;
};
