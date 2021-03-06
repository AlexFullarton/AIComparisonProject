// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for the enemy capsule collider
	CapsuleColliderRadius = 40.0f;
	GetCapsuleComponent()->InitCapsuleSize(CapsuleColliderRadius, 100.0f);
	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName(TEXT("Pawn"));

	// Set enemy's max and current health defaults
	maxHealth = 100.0f;
	currentHealth = maxHealth;

	// Set enemy's damamage defaults
	meleeDamage = 10.0f;
	rangedDamage = 20.0f;

	// Enemy will start with melee weapons equipped
	isMelee = true;
	isRanged = false;

	// Setup a skeletal mesh component
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh3PObject(TEXT("SkeletalMesh'/Game/Meshes/Characters/ybot_enemy.ybot_enemy'"));
	USkeletalMeshComponent* SkelMesh = GetMesh();
	SkelMesh->SkeletalMesh = Mesh3PObject.Object;
	SkelMesh->SetupAttachment(GetCapsuleComponent());
	SkelMesh->bCastDynamicShadow = true;
	SkelMesh->CastShadow = true;
	SkelMesh->SetRelativeRotation(FRotator(0, 0, 0));
	SkelMesh->SetRelativeLocation(FVector(0, 0, -100.0f));
	SkelMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	// Set animation blueprint for the third person mesh
	static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> AnimationBP(TEXT("/Game/Animations/Character/Player3P_AnimBP.Player3P_AnimBP_C"));
	SkelMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkelMesh->AnimClass = AnimationBP.Object;

	// Setup widget component for healthbar
	healthbarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthbarWidget"));
	static ConstructorHelpers::FObjectFinder<UClass> healthbar(TEXT("/Game/Blueprints/UI/EnemyHealthbarWidget.EnemyHealthbarWidget_C"));
	healthbarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
	healthbarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	healthbarWidget->SetWidgetClass(healthbar.Object);
	healthbarWidget->SetDrawSize(FVector2D(50.0f, 20.0f));
	healthbarWidget->SetDrawAtDesiredSize(true);
	healthbarWidget->SetupAttachment(RootComponent);
	
	// Gather weapon data for the enemy
	static ConstructorHelpers::FObjectFinder<UClass> swordBP(TEXT("/Game/Blueprints/Weapons/Sword.Sword_C"));
	rightHandSword = swordBP.Object;

	static ConstructorHelpers::FObjectFinder<UClass> shieldBP(TEXT("/Game/Blueprints/Weapons/Shield.Shield_C"));
	leftHandShield = shieldBP.Object;

	static ConstructorHelpers::FObjectFinder<UClass> bowBP(TEXT("/Game/Blueprints/Weapons/Bow.Bow_C"));
	leftHandBow = bowBP.Object;

	// Get the death screen widget class
	static ConstructorHelpers::FObjectFinder<UClass> DeathScreen(TEXT("/Game/Blueprints/UI/DeathScreenWidget.DeathScreenWidget_C"));
	DeathScreenWidgetClass = DeathScreen.Object;
}

void AEnemyCharacter::InitialiseEnemy()
{
	// Get enemy behaviour type passed in from main menu settings
	int enemyType = Cast<UAIComparisonInstance>(GetGameInstance())->EnemyBehaviourType;

	// Spawn enemy with different AI controller based on enemyType
	switch (enemyType)
	{
	case 0:
	{
		AIControllerClass = AEnemyControllerFSM::StaticClass();
		break;
	}
	case 1:
		AIControllerClass = AEnemyControllerBT::StaticClass();
		break;
	case 2:
		break;
	default:
		break;
	}
	AutoPossessAI = EAutoPossessAI::Spawned;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get game instance to retrieve data from main menu
	UAIComparisonInstance* instance = Cast<UAIComparisonInstance>(GetGameInstance());

	// Get health and damage values from main menu
	maxHealth = instance->EnemyInitialHealth;
	currentHealth = maxHealth;
	healthbarObject = Cast<UEnemyHealthbar>(healthbarWidget->GetUserWidgetObject());
	healthbarObject->SetMaxHealth(maxHealth);
	healthbarObject->UpdateHealth(currentHealth);

	meleeDamage = instance->EnemyMeleeDamage;
	rangedDamage = instance->EnemyRangedDamage;

	// Spawn starting weapons for the enemy
	swordWeapon = Cast<AMeleeWeapon>(GetWorld()->SpawnActor(rightHandSword));
	swordWeapon->AttachWeapon(this, GetMesh()->GetName(), "RightHandSocket");
	swordWeapon->weaponDamage = meleeDamage;

	shieldWeapon = Cast<AMeleeWeapon>(GetWorld()->SpawnActor(leftHandShield));
	shieldWeapon->AttachWeapon(this, GetMesh()->GetName(), "LeftHandSocket");
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::CharacterDeath()
{
	Cast<AEnemyController>(GetController())->isDead = true;
}

void AEnemyCharacter::ModifyHealth(float healthToSubtract)
{
	Super::ModifyHealth(healthToSubtract);
	healthbarObject->UpdateHealth(currentHealth);
}

void AEnemyCharacter::RagdollDeath()
{
	Super::RagdollDeath();
	// Hide healthbar
	healthbarWidget->SetVisibility(false);
	AEnemyController* controller = Cast<AEnemyController>(GetController());
	controller->PerceptionComponent->Deactivate();

	// Increment enemy kill counter
	UAIComparisonInstance* GameInstance = Cast<UAIComparisonInstance>(GetWorld()->GetGameInstance());
	GameInstance->EnemiesKilled++;
	// If all enemies are dead
	if (GameInstance->EnemiesKilled == GameInstance->EnemyCount)
	{
		Cast<UAIComparisonInstance>(GetWorld()->GetGameInstance())->shouldGatherData = false;
		Cast<UAIComparisonInstance>(GetWorld()->GetGameInstance())->CalculateDataAverages();
		Cast<UAIComparisonInstance>(GetWorld()->GetGameInstance())->CalculateDataStdDevs();


		APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		FInputModeUIOnly UIInputMode;
		UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(UIInputMode);
		PlayerController->SetShowMouseCursor(true);
		UUserWidget* DeathWidget = CreateWidget(GetWorld(), DeathScreenWidgetClass, TEXT("DeathScreen"));
		DeathWidget->AddToViewport();
	}
}

