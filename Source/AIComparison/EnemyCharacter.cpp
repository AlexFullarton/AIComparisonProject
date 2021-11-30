// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for the enemy capsule collider
	GetCapsuleComponent()->InitCapsuleSize(40.0f, 100.0f);

	// Set enemy's max and current health stats
	maxHealth = 100.0f;
	currentHealth = maxHealth;

	// Setup a skeletal mesh component
	USkeletalMeshComponent* SkelMesh = GetMesh();
	SkelMesh->SetupAttachment(GetCapsuleComponent());
	SkelMesh->bCastDynamicShadow = true;
	SkelMesh->CastShadow = true;
	SkelMesh->SetRelativeRotation(FRotator(0, 0, 0));
	SkelMesh->SetRelativeLocation(FVector(0, 0, -100.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh3PObject(TEXT("SkeletalMesh'/Game/Meshes/Characters/ybot_enemy.ybot_enemy'"));
	SkelMesh->SkeletalMesh = Mesh3PObject.Object;

	// Set animation blueprint for the third person mesh
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimationBP(TEXT("AnimBlueprint'/Game/Animations/Character/Sword/Player3P_AnimBP.Player3P_AnimBP'"));
	SkelMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkelMesh->AnimClass = AnimationBP.Object->GeneratedClass;
	
	// Gather weapon data for the enemy
	static ConstructorHelpers::FObjectFinder<UBlueprint> swordBP(TEXT("Blueprint'/Game/Blueprints/Weapons/Sword.Sword'"));
	rightHandSword = swordBP.Object->GeneratedClass;

	static ConstructorHelpers::FObjectFinder<UBlueprint> shieldBP(TEXT("Blueprint'/Game/Blueprints/Weapons/Shield.Shield'"));
	leftHandShield = shieldBP.Object->GeneratedClass;
}

void AEnemyCharacter::InitialiseEnemy(FString enemyType)
{
	// Depending on what is passed in from the enemy spawner, spawn different types of enemy (patrol vs. stationary)
	// different AI implementations will be added via child enemy classes with different tick functions
	// if (type1) AIControllerClass = 
	// else AIControllerClass = 
	// etc.
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Spawn starting weapons for the enemy
	AMeleeWeapon* sword = Cast<AMeleeWeapon>(GetWorld()->SpawnActor(rightHandSword));
	sword->AttachWeapon(this, GetMesh()->GetName(), "RightHandSocket");

	AMeleeWeapon* shield = Cast<AMeleeWeapon>(GetWorld()->SpawnActor(leftHandShield));
	shield->AttachWeapon(this, GetMesh()->GetName(), "LeftHandSocket");

}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharacter::MoveForward(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AEnemyCharacter::MoveRight(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

void AEnemyCharacter::Block()
{
	isBlocking = true;
}

void AEnemyCharacter::StopBlocking()
{
	isBlocking = false;
}

void AEnemyCharacter::Attack()
{
	isAttacking = true;
}