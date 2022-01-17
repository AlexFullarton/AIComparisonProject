// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/PlayerCharacterController.h"

APlayerCharacterController::APlayerCharacterController()
{
	PrimaryActorTick.bCanEverTick = true;
	TeamId = FGenericTeamId(1);
}
// Called to bind functionality to input
void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind block event
	InputComponent->BindAction("Block", IE_Pressed, this, &APlayerCharacterController::Block);
	InputComponent->BindAction("Block", IE_Released, this, &APlayerCharacterController::StopBlocking);

	// Bind Attack event
	InputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacterController::Attack);
	InputComponent->BindAction("Attack", IE_Released, this, &APlayerCharacterController::AttackRanged);

	// Bind Weapon Swap Event
	InputComponent->BindAction("WeaponSwap", IE_Pressed, this, &APlayerCharacterController::SwapWeapons);

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

	// Bind Quit event
	InputComponent->BindAction("QuitGame", IE_Released, this, &APlayerCharacterController::QuitGame);
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

void APlayerCharacterController::Block()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
	// If the player is currently controlling their avatar
	if (player != nullptr && !player->isAttacking)
	{
		player->Block();
	}
}

void APlayerCharacterController::StopBlocking()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
	// If the player is currently controlling their avatar
	if (player != nullptr)
	{
		player->StopBlocking();
	}
}

void APlayerCharacterController::Attack()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
	// If the player is currently controlling their avatar
	if (player != nullptr && !player->isBlocking && !player->isAttacking)
	{
		player->Attack();
	}
}

void APlayerCharacterController::AttackRanged()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
	// If the player is currently controlling their avatar
	if (player != nullptr)
	{
		player->AttackRanged();
	}
}

void APlayerCharacterController::SwapWeapons()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetPawn());
	// If the player is currently controlling their avatar
	if (player != nullptr && !player->isAttacking && !player->isBlocking)
	{
		player->SwapWeapons();
	}
}

void APlayerCharacterController::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);
}