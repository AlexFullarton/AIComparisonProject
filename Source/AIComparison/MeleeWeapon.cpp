// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

AMeleeWeapon::AMeleeWeapon()
{
	meleeWeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	meleeWeaponCollider->SetupAttachment(RootComponent);
}

void AMeleeWeapon::Attack()
{

}

