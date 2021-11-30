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
	isBlocking = false;
	isAttacking = false;
	attackCounter = 0;

	// Set players max and current health stats
	maxHealth = 100.0f;
	currentHealth = maxHealth;

	// Initial Camera Offset
	FVector cameraOffset(-200.0f, 20.0f, 70.0f);

	// Create third person camera component
	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	ThirdPersonCameraComponent->SetRelativeLocation(cameraOffset);
	ThirdPersonCameraComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	ThirdPersonCameraComponent->bUsePawnControlRotation = true;

	// Setup a skeletal mesh component for 3rd person view
	USkeletalMeshComponent* SkelMesh = GetMesh();
	SkelMesh->SetupAttachment(GetCapsuleComponent());
	//Mesh3P->SetOwnerNoSee(true);
	SkelMesh->bCastDynamicShadow = true;
	SkelMesh->CastShadow = false;
	SkelMesh->SetRelativeRotation(FRotator(0, 0, 0));
	SkelMesh->SetRelativeLocation(FVector(0, 0, -100.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh3PObject(TEXT("SkeletalMesh'/Game/Meshes/Characters/ybot.ybot'"));
	SkelMesh->SkeletalMesh = Mesh3PObject.Object;

	// Set animation blueprint for the third person mesh
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimationBP(TEXT("AnimBlueprint'/Game/Animations/Character/Sword/Player3P_AnimBP.Player3P_AnimBP'"));
	SkelMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkelMesh->AnimClass = AnimationBP.Object->GeneratedClass;

	// Setup player movement component acceleration/deceleration coefficients
	Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxAcceleration = 800.0f;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->BrakingFrictionFactor = 0.1f;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->BrakingFriction = 0.01f;

	static ConstructorHelpers::FObjectFinder<UBlueprint> swordBP(TEXT("Blueprint'/Game/Blueprints/Weapons/Sword.Sword'"));
	rightHandSword = swordBP.Object->GeneratedClass;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Spawn starting weapons for the player
	AMeleeWeapon* sword = Cast<AMeleeWeapon>(GetWorld()->SpawnActor(rightHandSword));
	sword->AttachWeapon(this, GetMesh()->GetName(), "RightHandSocket");
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

void APlayerCharacter::Block()
{
	isBlocking = true;
}

void APlayerCharacter::StopBlocking()
{
	isBlocking = false;
}

void APlayerCharacter::Attack()
{
	isAttacking = true;
}
