// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/BlendSpace.h"
#include "GameCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerCharacter3P_Anim.generated.h"

UCLASS()
class AICOMPARISON_API UPlayerCharacter3P_Anim : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPlayerCharacter3P_Anim();

protected:
	// Called when the game starts or when spawned
	virtual void NativeBeginPlay() override;

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Anims")
	float forwardMovement;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Anims")
	float sidewaysMovement;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Anims")
	float currentHealth;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Anims")
	bool isBlocking;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Anims")
	bool isAttacking;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Anims")
	int attackCounter;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Anims")
	bool isMelee;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Anims")
	bool isRanged;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Anims")
	bool canFire;

	AGameCharacter* Owner;

private:
	UAnimMontage* blockAnimationMontage, *attackAnimationMontage0, * attackAnimationMontage1;
};
