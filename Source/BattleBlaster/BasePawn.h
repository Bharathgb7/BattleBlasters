// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h" // we include this header file to use the class AProjectile in this class
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "BasePawn.generated.h"

UCLASS()
class BATTLEBLASTER_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void RotateTurret(FVector LookAtTarget);

	void Fire();

	void HandleDestruction();

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleANywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleANywhere)
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* ProjectileSpawnPoint;

	// variable created to get the blueprint reference of the projectile class to spawn it in the world
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile>ProjectileClass;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DeathParticle;

	UPROPERTY(EditAnywhere)
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase>DeathCameraShakeClass;
};
