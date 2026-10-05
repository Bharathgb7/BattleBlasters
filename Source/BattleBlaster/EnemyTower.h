// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "PlayerTank.h"
#include "EnemyTower.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API AEnemyTower : public ABasePawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	bool IsInFiringRange();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckFireCondition();

	void HandleDestruction();

	//in order to get player location to turn the head to them need a reference ptr
	APlayerTank* PlayerTank;

	UPROPERTY(EditAnywhere)
	float FiringRange = 300.0f;

	UPROPERTY(VisibleANywhere)
	float FireInterval = 2.0f;

	

	
};
