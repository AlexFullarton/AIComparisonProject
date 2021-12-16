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

	canSeePlayer = false;
	patrolSpeed = 100.0f;
	chaseSpeed = 500.0f;

	// How long to wait between each attack (minimum)
	attackRate = 2.0f;
	// Percentage block chance (minimum)
	blockChance = 33.3f;

	// Enemies initially are melee
	isMelee = true;
	isRanged = false;
}

void AEnemyController::MoveToRandomLocationInDistance(FVector pawnLocation)
{
	// When patrolling to a random location, set character speed to be low
	Cast<UCharacterMovementComponent>(controlledEnemy->GetMovementComponent())->MaxWalkSpeed = patrolSpeed;
	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (navSystem)
	{
		// Move the enemy to the given destination if a new location is found
		if (navSystem->GetRandomReachablePointInRadius(pawnLocation, searchRadius, destination))
		{
			MoveToLocation(destination.Location, tolerance);
		}
	}
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

	//UAIPerceptionsystem::RegisterPerceptionStimuliSource();
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector pawnLocation = controlledEnemy->GetActorLocation();
	// If the pawn has reached the given destination
	if (GetMoveStatus() != EPathFollowingStatus::Moving)
	{
		// Give the pawn a new destination
		MoveToRandomLocationInDistance(pawnLocation);
	}
}

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
				{
					sensedFriendlies.Remove(actor);
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "Goodbye friend!");
				}
				else
				{
					sensedFriendlies.Add(actor);
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "Hello friend!");

				}
				break;
			}
			// When an enemy detects the player
			case ETeamAttitude::Hostile:
			{
				if (sensedEnemies.Contains(actor))
				{
					sensedEnemies.Remove(actor);
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "You've gone!");
				}
				else
				{
					sensedEnemies.Add(actor);
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "I see you!");
				}
				break;
			}
			// When an enemy detects any other valid actor
			case ETeamAttitude::Neutral:
			default:
				break;
		}
	}
}

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