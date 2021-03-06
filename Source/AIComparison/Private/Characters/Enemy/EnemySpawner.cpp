// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemySpawner.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(SceneComponent);
	objectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	objectMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	objectMesh->SetStaticMesh(cylinderMesh.Object);

	static ConstructorHelpers::FObjectFinder<UClass> enemyClassBP(TEXT("/Game/Blueprints/Characters/EnemyCharacterBP.EnemyCharacterBP_C"));
	enemyClass = enemyClassBP.Object;

	SceneComponent->bHiddenInGame = true;
	objectMesh->bHiddenInGame = true;
	SetActorHiddenInGame(true);
}

void AEnemySpawner::SpawnEnemy()
{
	// Create initial spawn location for the enemy
	FTransform spawnTransform;
	spawnTransform.SetTranslation(GetActorLocation());
	spawnTransform.SetRotation(GetActorQuat());
	spawnTransform.SetScale3D(GetActorScale3D());
	AEnemyCharacter* enemy = GetWorld()->SpawnActorDeferred<AEnemyCharacter>(enemyClass, spawnTransform);
	enemy->InitialiseEnemy();
	enemy->FinishSpawning(spawnTransform);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}


	