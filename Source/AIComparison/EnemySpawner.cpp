// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(SceneComponent);
	objectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	objectMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> cylinderMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	objectMesh->SetStaticMesh(cylinderMesh.Object);

	SceneComponent->bHiddenInGame = true;
	objectMesh->bHiddenInGame = true;
	SetActorHiddenInGame(true);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	FTransform spawnTransform;
	spawnTransform.SetTranslation(GetActorLocation());
	spawnTransform.SetRotation(GetActorQuat());
	spawnTransform.SetScale3D(GetActorScale3D());
	AEnemyCharacter* enemy = GetWorld()->SpawnActorDeferred<AEnemyCharacter>(AEnemyCharacter::StaticClass(), spawnTransform);

	// Set the enemy controller here

	enemy->FinishSpawning(spawnTransform);
}


