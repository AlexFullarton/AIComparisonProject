// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Characters/Enemy/EnemyHealthbar.h"
#include "PlayerHUD.generated.h"

UCLASS()
class AICOMPARISON_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	APlayerHUD();

protected:
	virtual void BeginPlay();

public:
	virtual void DrawHUD() override;

	UEnemyHealthbar* playerHealthbar;
	UUserWidget* playerCrosshair;
	UClass* healthbarClass;
	UClass* crosshairClass;
};
