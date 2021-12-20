// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "EnemyCharacter.h"

AEnemyController::AEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;

	SetGenericTeamId(FGenericTeamId(2));

	// Setup perception component
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	PerceptionComponent->ConfigureSense(*sightConfig);
	PerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyController::PerceptionUpdated);

	// Set default destination to be the origin of the world
	destination.Location = FVector(0.0f, 0.0f, 0.0f);
	searchRadius = 1000.0f;
	
	// Enemy can be 25 units away before searching for a new destination
	tolerance = 25.0f;

	patrolSpeed = 100.0f;
	chaseSpeed = 400.0f;

	// Max distance from player at which the enemy can melee attack
	meleeAttackRange = 100.0f;
	// Max distance from player at which the enemy can ranged attack
	rangedAttackRange = 450.0f;

	// How close the enemies will move to the player in melee form before trying to attack
	meleeTolerance = 10.0f;
	// How close the enemies will move to the player in ranged form before trying to attack
	rangedTolerance = 300.0f;

	// How long to wait between each attack (minimum)
	attackRate = 3.0f;

	// Enemies initially are melee
	isMelee = true;
	isRanged = false;

	isDead = false;
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// Get a reference to the enemy being controled by this
	// instance of the enemy controller
	controlledEnemy = Cast<AEnemyCharacter>(InPawn);

	// Only setup sight config when the controller is possessing an enemy
	sightConfig->SightRadius = 500.0f;
	sightConfig->LoseSightRadius = sightConfig->SightRadius + 250.0f;
	sightConfig->PeripheralVisionAngleDegrees = 70.0f;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionComponent->ConfigureSense(*sightConfig);
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	pawnLocation = controlledEnemy->GetActorLocation();
}

void AEnemyController::MoveToRandomLocationInDistance(FVector Location)
{
	// When patrolling to a random location, set character speed to be low
	Cast<UCharacterMovementComponent>(controlledEnemy->GetMovementComponent())->MaxWalkSpeed = patrolSpeed;
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (navSystem)
	{
		// Move the enemy to the given destination if a new location is found
		if (navSystem->GetRandomReachablePointInRadius(Location, searchRadius, destination))
		{
			MoveToLocation(destination.Location, tolerance);
		}
	}
}

void AEnemyController::MoveToPlayer(float acceptanceRadius)
{
	// When chasing the player, set the movement speed to be high
	Cast<UCharacterMovementComponent>(controlledEnemy->GetMovementComponent())->MaxWalkSpeed = chaseSpeed;
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (navSystem)
	{
		// Move the enemy to the player character actor, using the reference
		// to the detected player actor and acceptance radius
		MoveToActor(sensedPlayer, acceptanceRadius);
	}
}

float AEnemyController::getDistanceToPlayer()
{
	return controlledEnemy->GetDistanceTo(sensedPlayer);
}

void AEnemyController::RotateToFacePlayer()
{
	if (sensedPlayer)
	{
		controlledEnemy->SetActorRotation(FRotator(controlledEnemy->GetActorRotation().Pitch, FMath::RInterpTo(controlledEnemy->GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(controlledEnemy->GetActorLocation(), sensedPlayer->GetActorLocation()), GetWorld()->DeltaTimeSeconds, 20.0f).Yaw, controlledEnemy->GetActorRotation().Roll));
	}
}

void AEnemyController::Attack()
{
	RotateToFacePlayer();
	controlledEnemy->Attack();
}

void AEnemyController::AttackRanged()
{
	controlledEnemy->Attack();
	controlledEnemy->AttackRanged();
	SetRangedAttackTimer();
	disallowRanged();
}

bool AEnemyController::IsEnemyAttacking()
{
	return controlledEnemy->isAttacking;
}

void AEnemyController::SetAttackTimer()
{
	float rate = FMath::RandRange(attackRate, attackRate + 1.5f);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyController::allowAttack, rate, false);
}

void AEnemyController::SetRangedAttackTimer()
{
	float rate = FMath::RandRange(attackRate, attackRate + 1.5f);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyController::allowRanged, rate, false);
}

void AEnemyController::allowAttack()
{
	attackAllowed = true;
}

void AEnemyController::disallowAttack()
{
	attackAllowed = false;
}

void AEnemyController::allowRanged()
{
	rangedAllowed = true;
}

void AEnemyController::disallowRanged()
{
	rangedAllowed = false;
}

void AEnemyController::Block()
{
	controlledEnemy->Block();
}

void AEnemyController::StopBlocking()
{
	controlledEnemy->StopBlocking();
}

void AEnemyController::SetBlockTimer()
{
	float rate = FMath::RandRange(1.5, 3.0f);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyController::allowBlock, rate, false);
}

void AEnemyController::allowBlock()
{ 
	blockAllowed = true; 
}

void AEnemyController::disallowBlock()
{ 
	blockAllowed = false; 
}

void AEnemyController::SwapWeapons()
{
	controlledEnemy->SwapWeapons();
	isMelee = controlledEnemy->isMelee;
	isRanged = controlledEnemy->isRanged;
}

// Functions used for handling detection of other actors using the AI Perception Module

// Fires when perception stimuli are either added or removed from the sight sense of the enemy
void AEnemyController::PerceptionUpdated(const TArray<AActor*>& testActors)
{
	for(auto actor : testActors)
	{
		switch (GetTeamAttitudeTowards(*actor))
		{
			// When an enemy detects another enemy
			case ETeamAttitude::Friendly:
			{
				if (sensedFriendlies.Contains(actor))
					sensedFriendlies.Remove(actor);
				else
					sensedFriendlies.Add(actor);
				break;
			}
			// When an enemy detects the player
			case ETeamAttitude::Hostile:
			{
				if (sensedPlayer == actor)
					sensedPlayer = nullptr;
				else
					sensedPlayer = actor;
				break;
			}
			// When an enemy detects any other valid actor
			case ETeamAttitude::Neutral:
			default:
				break;
		}
	}
}

// Computes relationship between this actor and other actors it has percieved
ETeamAttitude::Type AEnemyController::GetTeamAttitudeTowards(const AActor& Other) const
{
	// If the other actor is a pawn
	const APawn* OtherPawn = Cast<APawn>(&Other);
	if (OtherPawn)
	{
		// If the other actor is being controlled by a controller
		const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
		if (TeamAgent)
		{ 
			// Do the same again using the controller of the other actor
			return IGenericTeamAgentInterface::GetTeamAttitudeTowards(*OtherPawn->GetController());
		}
	}
	// Neutral behaviour for non-pawns
	return ETeamAttitude::Neutral;
}
