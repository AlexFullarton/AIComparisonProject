// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
	OwningPawn = nullptr;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(SceneComponent);
	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	weaponMesh->SetupAttachment(RootComponent);
}

void AWeapon::Attack()
{

}

void AWeapon::stopAttacking()
{

}

bool AWeapon::AttachWeapon(APawn* WeaponOwner, FString SkeletalCompName, FName socketName)
{
	// Set owner of this weapon
	OwningPawn = WeaponOwner;
	SetOwner(OwningPawn);

	// Get the skeletal mesh of the owning pawn
	USkeletalMeshComponent* Mesh = GetPawnSkeletalMesh(SkeletalCompName);

	// If owning pawn has a valid skeletal mesh
	if (Mesh)
	{
		FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, false);
		AttachToComponent(Mesh, attachmentRules, socketName);
		return true;
	}
	else
		return false;
}

USkeletalMeshComponent* AWeapon::GetPawnSkeletalMesh(FString ComponentName)
{
	TArray<UActorComponent*> Components;
	USkeletalMeshComponent* Mesh = nullptr;

	if (OwningPawn)
		OwningPawn->GetComponents(Components);
	for (UActorComponent* Comp : Components)
	{
		Mesh = Cast<USkeletalMeshComponent>(Comp);
		if (Mesh && Mesh->GetName() == ComponentName)
			return Mesh;
	}
	return Mesh;
}

