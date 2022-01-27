// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Characters/Player/PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "../../AIComparisonInstance.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class AICOMPARISON_API APlayerCharacter : public AGameCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	virtual void ModifyHealth(float healthToSubtract) override;
	virtual void CharacterDeath() override;

	virtual void FireArrow() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RagdollDeath();

	// Third person camera - viewport
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)

	APlayerHUD* HUD;

	UClass* DeathScreenWidgetClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
