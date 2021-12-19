// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "EnemyHealthbar.h"
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

	UClass* healthbarClass;

private:
	class UTexture2D* CrosshairTexture;
};
