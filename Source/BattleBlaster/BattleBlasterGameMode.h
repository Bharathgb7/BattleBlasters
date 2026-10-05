// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerTank.h"
#include "ScreenMessageWidget.h"
#include "BattleBlasterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// to pass out C++ widget class as a ref in WBP
	UPROPERTY(EditAnywhere)
	TSubclassOf<UScreenMessageWidget>ScreenMessageWidgetClass;

	// C++ widget class as a ref 
	UScreenMessageWidget* ScreenMessageWidget;

	//in order to get player location to turn the head to them need a reference ptr
	APlayerTank* PlayerTank;

	// to monitor the count of enemy tower
	int32 TowerCount;

	UPROPERTY(EditAnywhere)
	float GameOverDelay = 3.0f;

	bool IsVictory = false;

	UPROPERTY(EditAnywhere)
	int32 CountdownDelay = 3;

	//variable used to keep track of countdown
	int32 CountdownSeconds;

	FTimerHandle CountdownTimerHandle;

	void OnCountDownTimerTimeOut();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActorDied(AActor* DeadActor);

	void OnGameOverTimeOut();

	

};
