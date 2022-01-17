// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/RangedWeapon.h"

ARangedWeapon::ARangedWeapon()
{
	static ConstructorHelpers::FClassFinder<AActor> arrowClass(TEXT("Class'/Script/AIComparison.Arrow'"));
	ArrowClass = arrowClass.Class;
}

void ARangedWeapon::Fire(FVector spawnLocation, FRotator spawnRotation)
{
	if (ArrowClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the given location
			AArrow* arrow = World->SpawnActor<AArrow>(ArrowClass, spawnLocation, spawnRotation, SpawnParams);
			if (arrow)
			{
				// Set projectiles initial trajectory
				FVector LaunchDirection = spawnRotation.Vector();
				arrow->FireInDirection(LaunchDirection);
			}
		}
	}
}