// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter3P_Anim.h"

UPlayerCharacter3P_Anim::UPlayerCharacter3P_Anim()
{
	// Initialise movement variables
	forwardMovement = 0.0f;
	sidewaysMovement = 0.0f;
	isJumping = false;
	isBlocking = false;

	// Initialise player stat variables
	currentHealth = 1.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> blockAnim(TEXT("AnimMontage'/Game/Animations/Character/Sword/Combat/BlockMovement.BlockMovement'"));
	blockAnimationMontage = blockAnim.Object;
}

void UPlayerCharacter3P_Anim::NativeUpdateAnimation(float DeltaSeconds)
{
	//Super::NativeUpdateAnimation(DeltaSeconds);
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningActor());
	if (Player != nullptr)
	{
		forwardMovement = FVector::DotProduct(Player->GetVelocity(), Player->GetActorForwardVector());
		sidewaysMovement = -1 * FVector::DotProduct(Player->GetVelocity(), Player->GetActorRightVector());
		isJumping = Player->isJumping;
		isBlocking = Player->isBlocking;
		currentHealth = Player->currentHealth;
	}

	if (isBlocking && !Montage_IsPlaying(blockAnimationMontage))
	{
		Montage_Play(blockAnimationMontage);
	}
	if (!isBlocking && Montage_IsPlaying(blockAnimationMontage))
	{
		Montage_Stop(0.2f, blockAnimationMontage);
	}
}