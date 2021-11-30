// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Weapon.generated.h"

UCLASS()
class AICOMPARISON_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	
	// Pure virtual function - implementation in child classes melee and ranged weapons respectively
	virtual void Attack();

	// Attach the weapon to the owning pawn/character
	virtual bool AttachWeapon(APawn* WeaponOwner, FString SkeletalCompName, FName socketName);

	// Get a pointer to the skeletal mesh of the owning pawn
	virtual USkeletalMeshComponent* GetPawnSkeletalMesh(FString ComponentName);

	// Weapon owner
	APawn* OwningPawn;

	// Root component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Root)
	USceneComponent* SceneComponent;

	// Weapon mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* weaponMesh;

	// Weapon modifiers for combat
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	float weaponDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	float weaponAttackRate;

	// WEAPONS WILL NEED A COLLIDER THAT WILL BE TURNED ON/OFF WHEN ATTACKING FOR MELEE
	// IS ATTACKING BOOL THAT IS SET BY THE PLAYER


};
