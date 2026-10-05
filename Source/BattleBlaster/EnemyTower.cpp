// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTower.h"

void AEnemyTower::BeginPlay()
{
	Super::BeginPlay();

	// creating a timer to automatically fire for enemy in every 2 seconds without player input
	FTimerHandle FireRateTimerHandle;
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, 
		this, 
		&AEnemyTower::CheckFireCondition, 
		FireInterval, 
		true);
	
}
bool AEnemyTower::IsInFiringRange()
{
	//variable to check whether the player is in range or not 
	// created outside of if statement to avoid scope issue and return the value outside of the if statement
	bool Result = false;

	if (PlayerTank)
	{
		// rotating and firing the turret based on how close the player is to enemy tower 
		// to measure the distance between player and enemy we use Dist()
		float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerTank->GetActorLocation());
		Result = DistanceToPlayer <= FiringRange;
	}
	return Result;
}

void AEnemyTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// then checking with the used defined range and rotating turret based on that
	if (IsInFiringRange())
	{
		RotateTurret(PlayerTank->GetActorLocation());
	}
}


void AEnemyTower::CheckFireCondition()
{
	if (PlayerTank && PlayerTank->IsAlive && IsInFiringRange())
	{
		Fire();
	}
}

void AEnemyTower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}
