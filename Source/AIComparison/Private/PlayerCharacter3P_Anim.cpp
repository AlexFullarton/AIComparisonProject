// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter3P_Anim.h"

UPlayerCharacter3P_Anim::UPlayerCharacter3P_Anim()
{
	// Initialise movement variables
	forwardMovement = 0.0f;
	sidewaysMovement = 0.0f;
	isBlocking = false;
	isMelee = true;
	isRanged = false;

	// Initialise player stat variables
	currentHealth = 1.0f;
}

void UPlayerCharacter3P_Anim::NativeBeginPlay()
{
	AGameCharacter* PlayerObject = Cast<AGameCharacter>(GetOwningActor());
	if (PlayerObject)
		Owner = PlayerObject;
}

void UPlayerCharacter3P_Anim::NativeUpdateAnimation(float DeltaSeconds)
{
	//Super::NativeUpdateAnimation(DeltaSeconds);
	if (Owner != nullptr)
	{
		forwardMovement = FVector::DotProduct(Owner->GetVelocity(), Owner->GetActorForwardVector());
		sidewaysMovement = -1 * FVector::DotProduct(Owner->GetVelocity(), Owner->GetActorRightVector());
		isBlocking = Owner->isBlocking;
		isAttacking = Owner->isAttacking;
		attackCounter = Owner->attackCounter;
		currentHealth = Owner->currentHealth;
		isMelee = Owner->isMelee;
		isRanged = Owner->isRanged;
		canFire = Owner->canFire;
	}
}