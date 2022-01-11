// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set size for the player capsule collider
	GetCapsuleComponent()->InitCapsuleSize(40.0f, 100.0f);
	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName(TEXT("Pawn"));

	// Set player look and turn rates
	TurnRate = 45.0f;
	LookUpRate = 45.0f;
	isBlocking = false;
	isAttacking = false;
	attackCounter = 0;

	// Set players max and current health defaults
	maxHealth = 100.0f;
	currentHealth = maxHealth;

	// Set player's damage defaults
	meleeDamage = 30.0f;
	rangedDamage = 50.0f;

	// Player will start with melee weapons equipped
	isMelee = true;
	isRanged = false;

	// Initial Camera Offset
	FVector cameraOffset(50.0f, 20.0f, 20.0f);

	// Create spring arm component to handle camera clipping
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->bEnableCameraLag = true;


	// Create third person camera component
	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCameraComponent->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Setup a skeletal mesh component for 3rd person view
	USkeletalMeshComponent* SkelMesh = GetMesh();
	SkelMesh->SetupAttachment(GetCapsuleComponent());
	SkelMesh->bCastDynamicShadow = true;
	SkelMesh->CastShadow = true;
	SkelMesh->SetRelativeRotation(FRotator(0, 0, 0));
	SkelMesh->SetRelativeLocation(FVector(0, 0, -100.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh3PObject(TEXT("SkeletalMesh'/Game/Meshes/Characters/ybot.ybot'"));
	SkelMesh->SkeletalMesh = Mesh3PObject.Object;

	// Set animation blueprint for the third person mesh
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimationBP(TEXT("AnimBlueprint'/Game/Animations/Character/Player3P_AnimBP.Player3P_AnimBP'"));
	SkelMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkelMesh->AnimClass = AnimationBP.Object->GeneratedClass;

	// Setup player movement component acceleration/deceleration coefficients
	Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxAcceleration = 800.0f;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->BrakingFrictionFactor = 0.1f;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->BrakingFriction = 0.01f;

	// Gather weapon data for the player
	static ConstructorHelpers::FObjectFinder<UBlueprint> swordBP(TEXT("Blueprint'/Game/Blueprints/Weapons/Sword.Sword'"));
	rightHandSword = swordBP.Object->GeneratedClass;

	static ConstructorHelpers::FObjectFinder<UBlueprint> shieldBP(TEXT("Blueprint'/Game/Blueprints/Weapons/Shield.Shield'"));
	leftHandShield = shieldBP.Object->GeneratedClass;

	static ConstructorHelpers::FObjectFinder<UBlueprint> bowBP(TEXT("Blueprint'/Game/Blueprints/Weapons/Bow.Bow'"));
	leftHandBow = bowBP.Object->GeneratedClass;

	// Get the death screen widget class
	static ConstructorHelpers::FClassFinder<UUserWidget> DeathScreen(TEXT("WidgetBlueprint'/Game/Blueprints/UI/DeathScreenWidget.DeathScreenWidget_C'"));
	DeathScreenWidgetClass = DeathScreen.Class;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get game instance to retrieve data from main menu
	UAIComparisonInstance* instance = Cast<UAIComparisonInstance>(GetGameInstance());

	// Get health and damage values from main menu
	maxHealth = instance->PlayerInitialHealth;
	currentHealth = maxHealth;
	HUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	// HUD isnt loaded in menu but the player character is - so this checks to avoid nullptr exception
	if (HUD)
	{
		HUD->playerHealthbar->SetMaxHealth(maxHealth);
		HUD->playerHealthbar->UpdateHealth(currentHealth);
	}

	meleeDamage = instance->PlayerMeleeDamage;
	rangedDamage = instance->PlayerRangedDamage;

	// Spawn starting weapons for the player
	swordWeapon = Cast<AMeleeWeapon>(GetWorld()->SpawnActor(rightHandSword));
	swordWeapon->AttachWeapon(this, GetMesh()->GetName(), "RightHandSocket");
	swordWeapon->weaponDamage = meleeDamage;

	shieldWeapon = Cast<AMeleeWeapon>(GetWorld()->SpawnActor(leftHandShield));
	shieldWeapon->AttachWeapon(this, GetMesh()->GetName(), "LeftHandSocket");	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::FireArrow()
{
	if (isRanged)
	{
		// Initial spawn location and rotation for arrow
		FVector spawnLocation = GetMesh()->GetSocketLocation(TEXT("LeftHandSocket"));
		spawnLocation.Y -= 20.0f;
		FRotator spawnRotation = GetActorRotation();
		spawnRotation.Pitch = ThirdPersonCameraComponent->GetRelativeRotation().Pitch;
		bowWeapon->Fire(spawnLocation, spawnRotation);
	}
}

void APlayerCharacter::ModifyHealth(float healthToSubtract)
{
	Super::ModifyHealth(healthToSubtract);
	if (HUD)
		HUD->playerHealthbar->UpdateHealth(currentHealth);
}

void APlayerCharacter::CharacterDeath()
{
	RagdollDeath();
	// End game screen here and stop input
	// Could maybe add this to ragdoll death?
}

void APlayerCharacter::RagdollDeath()
{
	Super::RagdollDeath();
	// Hide healthbar
	HUD->playerHealthbar->SetVisibility(ESlateVisibility::Hidden);
	HUD->playerCrosshair->SetVisibility(ESlateVisibility::Hidden);

	// Pause the game to stop events from ticking and show the death screen
	APlayerController* controller = Cast<APlayerController>(GetController());
	FInputModeUIOnly UIInputMode;
	UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	controller->SetInputMode(UIInputMode);
	controller->SetShowMouseCursor(true);
	UUserWidget* DeathWidget = CreateWidget(GetWorld(), DeathScreenWidgetClass, TEXT("DeathScreen"));
	DeathWidget->AddToViewport();
}