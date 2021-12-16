// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCharacter.h"

// Sets default values
AGameCharacter::AGameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameCharacter::Block()
{
	if (isMelee)
		isBlocking = true;
}

void AGameCharacter::StopBlocking()
{
	if (isMelee)
		isBlocking = false;
}

void AGameCharacter::Attack()
{
	isAttacking = true;
}

void AGameCharacter::FireArrow()
{
	if (isRanged)
	{
		// Bool for anim transition
		canFire = true;
		// Initial spawn location and rotation for arrow
		FVector spawnLocation = GetMesh()->GetSocketLocation(TEXT("LeftHandSocket"));
		FRotator spawnRotation = GetActorRotation();
		bowWeapon->Fire(spawnLocation, spawnRotation);
	}
}

void AGameCharacter::AttackDone()
{
	canFire = false;
	isAttacking = false;
}

void AGameCharacter::toggleWeaponCollider()
{
	if (isMelee)
		swordWeapon->toggleCollider();
}

void AGameCharacter::SwapWeapons()
{
	isMelee = !isMelee;
	isRanged = !isRanged;

	if (isMelee)
	{
		// Detach bow from right hand socket
		bowWeapon->DetachWeapon();
		// Add sword and shield to right and left hand sockets
		swordWeapon = Cast<AMeleeWeapon>(GetWorld()->SpawnActor(rightHandSword));
		swordWeapon->AttachWeapon(this, GetMesh()->GetName(), "RightHandSocket");
		swordWeapon->weaponDamage = meleeDamage;;

		shieldWeapon = Cast<AMeleeWeapon>(GetWorld()->SpawnActor(leftHandShield));
		shieldWeapon->AttachWeapon(this, GetMesh()->GetName(), "LeftHandSocket");
	}
	if (isRanged)
	{
		swordWeapon->DetachWeapon();
		shieldWeapon->DetachWeapon();

		bowWeapon = Cast<ARangedWeapon>(GetWorld()->SpawnActor(leftHandBow));
		bowWeapon->AttachWeapon(this, GetMesh()->GetName(), "LeftHandSocket");
		bowWeapon->weaponDamage = rangedDamage;
	}
}

void AGameCharacter::MoveForward(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AGameCharacter::MoveRight(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

void AGameCharacter::TurnAtRate(float rate)
{

	AddControllerYawInput(rate * TurnRate * GetWorld()->GetDeltaSeconds());

	// Turn in place animation calculations
	if (GetVelocity() == FVector::ZeroVector)
	{
		if (rate > 0.3)
		{
			turnRight = true;
		}
		else if (rate < -0.3)
		{
			turnLeft = true;
		}
		else
		{
			turnRight = false;
			turnLeft = false;
		}
	}
	else
	{
		turnRight = false;
		turnLeft = false;
	}
}

void AGameCharacter::LookUpAtRate(float rate)
{
	AddControllerPitchInput(rate * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGameCharacter::ModifyHealth(float healthToSubtract)
{
	if ((currentHealth - healthToSubtract) < 0.0f)
		currentHealth = 0.0f;
	else if ((currentHealth - healthToSubtract) > maxHealth)
		currentHealth = maxHealth;
	else
		currentHealth -= healthToSubtract;
}
