// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter3P_Anim.h"

UPlayerCharacter3P_Anim::UPlayerCharacter3P_Anim()
{
	// Initialise movement variables
	forwardMovement = 0.0f;
	sidewaysMovement = 0.0f;
	isJumping = false;
}

void UPlayerCharacter3P_Anim::NativeUpdateAnimation(float DeltaSeconds)
{
	//Super::NativeUpdateAnimation(DeltaSeconds);
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningActor());
	if (Player != nullptr)
	{
		forwardMovement = FVector::DotProduct(Player->GetVelocity(), Player->GetActorForwardVector());
		sidewaysMovement = -1 * FVector::DotProduct(Player->GetVelocity(), Player->GetActorRightVector());
		//isJumping = Player->GetMovementComponent()->isFalling();
	}
}