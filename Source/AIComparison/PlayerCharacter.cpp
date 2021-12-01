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
	SkelMesh->bCastDynamicShadow = true;
	SkelMesh->CastShadow = true;
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

	// Gather weapon data for the player
	static ConstructorHelpers::FObjectFinder<UBlueprint> swordBP(TEXT("Blueprint'/Game/Blueprints/Weapons/Sword.Sword'"));
	rightHandSword = swordBP.Object->GeneratedClass;

	static ConstructorHelpers::FObjectFinder<UBlueprint> shieldBP(TEXT("Blueprint'/Game/Blueprints/Weapons/Shield.Shield'"));
	leftHandShield = shieldBP.Object->GeneratedClass;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Spawn starting weapons for the player
	swordWeapon = Cast<AMeleeWeapon>(GetWorld()->SpawnActor(rightHandSword));
	swordWeapon->AttachWeapon(this, GetMesh()->GetName(), "RightHandSocket");
	swordWeapon->weaponDamage = 30.0f;

	AMeleeWeapon* shield = Cast<AMeleeWeapon>(GetWorld()->SpawnActor(leftHandShield));
	shield->AttachWeapon(this, GetMesh()->GetName(), "LeftHandSocket");
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
