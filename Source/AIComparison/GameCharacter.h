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
#include "MeleeWeapon.h"
#include "RangedWeapon.h"
#include "GameCharacter.generated.h"

UCLASS()
class AICOMPARISON_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameCharacter();

	// Equipped weapons
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	UClass* leftHandShield;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	UClass* rightHandSword;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	UClass* leftHandBow;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// block functions
	void Block();
	void StopBlocking();

	// attack functions
	void Attack();

	void SwapWeapons();

	UFUNCTION(BlueprintCallable)
	void stopAttacking();

	// moving forward/backward
	void MoveForward(float Val);

	// movement left and right
	void MoveRight(float Val);

	//turn at given rate
	void TurnAtRate(float Rate);

	// look up/down at given rate
	void LookUpAtRate(float Rate);

	//turn rate, in deg/sec
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float TurnRate;

	//look up/down rate, in deg/sec
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float LookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool isBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool isAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float maxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float currentHealth;

	int attackCounter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool turnRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool turnLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	bool isMelee;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	bool isRanged;

	// Function to modify health on damage
	UFUNCTION()
	void ModifyHealth(float healthToSubtract);

	// For collision detection with weapons when being attacked
	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	AMeleeWeapon* swordWeapon;
	AMeleeWeapon* shieldWeapon;
	ARangedWeapon* bowWeapon;
};
