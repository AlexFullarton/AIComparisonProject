// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class AICOMPARISON_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	// Character body mesh for third person view
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh3P;

	// Third person camera - viewport
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCameraComponent;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	// jump functions
	void Jump();
	void StopJumping();

	// block function
	void Block();
	void StopBlocking();

	// Interaction function
	void Interact();

	// moving forward/backward
	void MoveForward(float Val);

	// movement left and right
	void MoveRight(float Val);

	//turn at given rate
	void TurnAtRate(float Rate);

	// look up/down at given rate
	void LookUpAtRate(float Rate);

	//turn rate, in deg/sec
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float TurnRate;

	//look up/down rate, in deg/sec
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float LookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool isJumping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool isBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float maxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	float currentHealth;
};
