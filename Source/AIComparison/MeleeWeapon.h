// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class AICOMPARISON_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()

	AMeleeWeapon();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	UBoxComponent* meleeWeaponCollider;

	// Melee weapon attack function
	void Attack() override;

	void stopAttacking() override;
};
