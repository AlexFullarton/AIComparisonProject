// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set size for the player capsule collider
	GetCapsuleComponent()->InitCapsuleSize(40.0f, 100.0f);

	// Set player look and turn rates
	TurnRate = 45.0f;
	LookUpRate = 45.0f;
	isJumping = false;

	// Initial Camera Offset
	FVector cameraOffset(-30.0f, 0.0f, 70.0f);

	// Create third person camera component
	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	ThirdPersonCameraComponent->SetRelativeLocation(cameraOffset);
	ThirdPersonCameraComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	ThirdPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component for 3rd person view
	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh3P"));
	Mesh3P->SetupAttachment(GetCapsuleComponent());
	//Mesh3P->SetOwnerNoSee(true);
	Mesh3P->bCastDynamicShadow = true;
	Mesh3P->CastShadow = false;
	Mesh3P->SetRelativeRotation(FRotator(0, 0, 0));
	Mesh3P->SetRelativeLocation(FVector(0, 0, -100.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh3PObject(TEXT("SkeletalMesh'/Game/Meshes/Characters/ybot.ybot'"));
	Mesh3P->SkeletalMesh = Mesh3PObject.Object;

	// Set animation blueprint for the third person mesh
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimationBP(TEXT("AnimBlueprint'/Game/Animations/Character/PlayerCharacter3P_Anim.PlayerCharacter3P_Anim'"));
	Mesh3P->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	Mesh3P->AnimClass = AnimationBP.Object->GeneratedClass;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::MoveForward(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void APlayerCharacter::MoveRight(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

void APlayerCharacter::TurnAtRate(float rate)
{
	AddControllerYawInput(rate * TurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float rate)
{
	AddControllerPitchInput(rate * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Interact()
{

}

void APlayerCharacter::Jump()
{
	Super::Jump();
}

void APlayerCharacter::StopJumping()
{
	Super::StopJumping();
}
