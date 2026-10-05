// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "PlayerTank.generated.h"



/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API APlayerTank : public ABasePawn
{
	GENERATED_BODY()


public:
	// Sets default values for this pawn's properties
	APlayerTank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// a shared handle destruction function to be called when the player tank is destroyed
	// same spelling is must to exectue
	void HandleDestruction();

	void SetPlayerEnabled(bool Enabled);

	APlayerController* PlayerController;

	bool IsAlive = true;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	// forward declaration of variable for performance 
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "INPUT")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "INPUT")
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, Category = "INPUT")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "INPUT")
	class UInputMappingContext* InputMC;

	UPROPERTY(EditAnywhere)
	float Speed = 300.0f;

	UPROPERTY(EditAnywhere)
	float TurnRate = 50.0f;

	//getting the float1dValue(1 or -1) on input action which use to identify whether we press w(1) or s(-1) 
	// using FInputActionValue as argument to get these value to access it 
	void MoveInput(const FInputActionValue& Value);
	void TurnInput(const FInputActionValue& Value);

};
