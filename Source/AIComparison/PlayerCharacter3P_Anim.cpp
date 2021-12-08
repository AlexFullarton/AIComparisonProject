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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> blockAnim(TEXT("AnimMontage'/Game/Animations/Character/Sword/Combat/BlockMovement.BlockMovement'"));
	blockAnimationMontage = blockAnim.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> attackAnim0(TEXT("AnimMontage'/Game/Animations/Character/Sword/Combat/AttackMovement0.AttackMovement0'"));
	attackAnimationMontage0 = attackAnim0.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> attackAnim1(TEXT("AnimMontage'/Game/Animations/Character/Sword/Combat/AttackMovement1.AttackMovement1'"));
	attackAnimationMontage1 = attackAnim1.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> drawAnim(TEXT("AnimMontage'/Game/Animations/Character/Bow/Combat/DrawArrow.DrawArrow'"));
	attackAnimationMontage1 = drawAnim.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> holdAnim(TEXT("AnimMontage'/Game/Animations/Character/Bow/Combat/AimHold.AimHold'"));
	attackAnimationMontage1 = holdAnim.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> shootAnim(TEXT("AnimMontage'/Game/Animations/Character/Bow/Combat/ShootRecoil.ShootRecoil'"));
	attackAnimationMontage1 = shootAnim.Object;
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
		currentHealth = Owner->currentHealth;
		isMelee = Owner->isMelee;
		isRanged = Owner->isRanged;

		// ATTACK ANIMATIONS
		if (isAttacking && Owner->attackCounter == 0 && !Montage_IsPlaying(NULL))
		{
			Montage_Play(attackAnimationMontage0);
			Owner->attackCounter++;
		}
		else if (isAttacking && Owner->attackCounter != 0 && !Montage_IsPlaying(NULL))
		{
			Montage_Play(attackAnimationMontage1);
			Owner->attackCounter = 0;
		}

		// BLOCKING ANIMATIONS
		if (isBlocking && !Montage_IsPlaying(blockAnimationMontage))
		{
			Montage_Play(blockAnimationMontage);
		}
		if (!isBlocking && Montage_IsPlaying(blockAnimationMontage))
		{
			Montage_Stop(0.2f, blockAnimationMontage);
		}
	}
}