// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "GameCharacter.h"


AMeleeWeapon::AMeleeWeapon()
{
	meleeWeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	meleeWeaponCollider->BodyInstance.SetCollisionProfileName(TEXT("Weapon"));
	meleeWeaponCollider->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnComponentBeginOverlap);
	meleeWeaponCollider->SetupAttachment(RootComponent);
}

void AMeleeWeapon::toggleCollider()
{
	meleeWeaponCollider->SetGenerateOverlapEvents(!meleeWeaponCollider->GetGenerateOverlapEvents());
}

void AMeleeWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionProfileName() == TEXT("Pawn") && OtherActor != GetOwner() && OtherActor->GetClass() != GetOwner()->GetClass())
	{
		Cast<AGameCharacter>(OtherActor)->ModifyHealth(weaponDamage);
	}
}

