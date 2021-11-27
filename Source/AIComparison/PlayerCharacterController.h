// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.generated.h"

UCLASS()
class AICOMPARISON_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	void Jump();

	void StopJumping();

	void Interact();

	void MoveForward(float value);

	void MoveRight(float value);

	void AddControllerYawInput(float value);

	void TurnAtRate(float rate);

	void AddControllerPitchInput(float value);

	void LookUpAtRate(float rate);

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;
};
