// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	if (!Collider)
	{
		// Create collider and set its size
		Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
		Collider->InitCapsuleSize(15.0f, 40.0f);
		RootComponent = Collider;
	}
	if (!ProjectileMovementComponent)
	{
		// This component will be what controls the movement of the arrow projectile
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(Collider);
		ProjectileMovementComponent->InitialSpeed = 300.0f;
		ProjectileMovementComponent->MaxSpeed = 300.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	if (!ProjectileMesh)
	{
		ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("'/Game/Meshes/Weapons/Arrow.Arrow'"));
		if (Mesh.Succeeded())
			ProjectileMesh->SetStaticMesh(Mesh.Object);
		ProjectileMesh->SetupAttachment(RootComponent);
	}

}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Initialised projectile in a given direction
void AArrow::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

