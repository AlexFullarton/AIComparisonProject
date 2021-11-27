// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"

// Called to bind functionality to input
void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind jump events
	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacterController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacterController::StopJumping);

	// Bind interact event
	InputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacterController::Interact);

	// Bind movement events
	InputComponent->BindAxis("MoveForward", this, &APlayerCharacterController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacterController::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APlayerCharacterController::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &APlayerCharacterController::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APlayerCharacterController::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &APlayerCharacterController::LookUpAtRate);
}

void APlayerCharacterController::Jump()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
	// If the player is currently controlling their avatar
	if (player != nullptr)
	{
		player->isJumping = true;
		player->Jump();
	}
}

void APlayerCharacterController::StopJumping()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
	// If the player is currently controlling their avatar
	if (player != nullptr)
	{
		player->isJumping = false;
		player->StopJumping();
	}
}

void APlayerCharacterController::Interact()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
	// If the player is currently controlling their avatar
	if (player != nullptr)
	{
		player->Interact();
	}
}

void APlayerCharacterController::MoveForward(float value)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
	// If the player is currently controlling their avatar
	if (player != nullptr)
	{
		player->MoveForward(value);
	}
}

void APlayerCharacterController::MoveRight(float value)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
	// If the player is currently controlling their avatar
	if (player != nullptr)
	{
		player->MoveRight(value);
	}
}

void APlayerCharacterController::AddControllerYawInput(float value)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
	// If the player is currently controlling their avatar
	if (player != nullptr)
	{
		player->AddControllerYawInput(value);
	}
}

void APlayerCharacterController::TurnAtRate(float rate)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
	// If the player is currently controlling their avatar
	if (player != nullptr)
	{
		player->TurnAtRate(rate);
	}
}

void APlayerCharacterController::AddControllerPitchInput(float value)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
	// If the player is currently controlling their avatar
	if (player != nullptr)
	{
		player->AddControllerPitchInput(value);
	}
}

void APlayerCharacterController::LookUpAtRate(float rate)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
	// If the player is currently controlling their avatar
	if (player != nullptr)
	{
		player->LookUpAtRate(rate);
	}
}