// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerHUD.h"

APlayerHUD::APlayerHUD()
{
	// Setup crosshair texture
	static ConstructorHelpers::FClassFinder<UUserWidget> crosshair(TEXT("WidgetBlueprint'/Game/Blueprints/UI/CrosshairWidget.CrosshairWidget_C'"));
	crosshairClass = crosshair.Class;

	// Setup widget for healthbar
	static ConstructorHelpers::FClassFinder<UUserWidget> healthbar(TEXT("WidgetBlueprint'/Game/Blueprints/UI/PlayerHealthbarWidget.PlayerHealthbarWidget_C'"));
	healthbarClass = healthbar.Class;
}

void APlayerHUD::BeginPlay()
{
	playerHealthbar = CreateWidget<UEnemyHealthbar>(GetWorld(), healthbarClass, TEXT("HealthbarWidget"));
	playerHealthbar->AddToViewport();

	playerCrosshair = CreateWidget<UUserWidget>(GetWorld(), crosshairClass, TEXT("CrosshairWidget"));
	playerCrosshair->AddToViewport();
}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();
}