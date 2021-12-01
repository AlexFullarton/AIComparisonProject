// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "EnemyCharacter.h"
#include "EnemySpawner.generated.h"

UCLASS()
class AICOMPARISON_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	// Root component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Root)
	USceneComponent* SceneComponent;
	// Static mesh for visibility in editor
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* objectMesh;
	// Enemy type to be spawned in by this spawner
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spwan Metrics")
	UClass* enemyClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawn Metrics")
	FString enemyType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
