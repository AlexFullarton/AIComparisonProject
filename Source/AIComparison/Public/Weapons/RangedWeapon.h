// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Arrow.h"
#include "UObject/ConstructorHelpers.h"
#include "RangedWeapon.generated.h"

/**
 * 
 */
UCLASS()
class AICOMPARISON_API ARangedWeapon : public AWeapon
{
	GENERATED_BODY()

public:

	ARangedWeapon();

	void Fire(FVector spawnLocation, FRotator spawnRotation);

	UPROPERTY(EditDefaultSOnly, Category = Projectile)
	UClass* ArrowClass;
};
