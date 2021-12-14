// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "GameCharacter.h"
#include "RangedWeapon.h"

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
		Collider->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		Collider->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnComponentBeginOverlap);
		Collider->InitCapsuleSize(15.0f, 40.0f);
		RootComponent = Collider;
	}
	if (!ProjectileMovementComponent)
	{
		// This component will be what controls the movement of the arrow projectile
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(Collider);
		ProjectileMovementComponent->InitialSpeed = 1000.0f;
		ProjectileMovementComponent->MaxSpeed = 1000.0f;
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
	InitialLifeSpan = 3.0f;
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

void AArrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionProfileName() == TEXT("Pawn") && OtherActor != GetOwner()->GetOwner())
	{
		Cast<AGameCharacter>(OtherActor)->ModifyHealth(Cast<ARangedWeapon>(GetOwner())->weaponDamage);
		Destroy();
	}
}

