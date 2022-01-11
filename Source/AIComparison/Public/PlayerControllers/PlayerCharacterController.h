// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Characters/Player/PlayerCharacter.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacterController.generated.h"

UCLASS()
class AICOMPARISON_API APlayerCharacterController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APlayerCharacterController();

protected:

	void MoveForward(float value);

	void MoveRight(float value);

	void AddControllerYawInput(float value);

	void TurnAtRate(float rate);

	void AddControllerPitchInput(float value);

	void LookUpAtRate(float rate);

	void Block();

	void StopBlocking();

	void Attack();

	void AttackRanged();

	void SwapWeapons();

	void QuitGame();

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

private:

	FGenericTeamId TeamId;

	FGenericTeamId GetGenericTeamId() const { return TeamId; }
};
