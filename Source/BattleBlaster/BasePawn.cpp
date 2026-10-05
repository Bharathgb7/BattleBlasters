// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootComponent"));
	SetRootComponent(CapsuleComponent);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(CapsuleComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	// calculating the distance vector by turret's location and mouse impact point's vector
	// for rotating the turret the mouse cursor moves
	FVector TargetVector = LookAtTarget - TurretMesh->GetComponentLocation();
	
	//Assigning a new rotation that the turret only should rotate its head from left to right
	FRotator LookAtRotation = FRotator(0.0f, TargetVector.Rotation().Yaw, 0.0f);

	// smooth rotation to avoid snapping of mesh while rotating too fast
	FRotator InterploatedRotation = FMath::RInterpTo(
		TurretMesh->GetComponentRotation(), 
		LookAtRotation, 
		GetWorld()->GetDeltaSeconds(), 
		10.0f);
	TurretMesh->SetWorldRotation(InterploatedRotation);
}

void ABasePawn::Fire()
{
	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	//DrawDebugSphere(GetWorld(), SpawnLocation, 25.0f, 12, FColor::Red, false, 3.0f);

	// spawn the projectile in the world using the class reference 
	// of the projectile class that we created in the blueprint
	AProjectile* SpawnedProjectile =
		GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	if(SpawnedProjectile)
	{
		// setting the reference of the owner of the projectile fired on the world
		// so that we can find who fired enemy or player and apply damage to them accordingly
		SpawnedProjectile->SetOwner(this);

		// setting and getting the owner of the projectile to check who fired the projectile in the world
		// the owner name is the instance name of the actor that fired the projectile in the world
		//AActor* ProjectileOwner = SpawnedProjectile->GetOwner();
		//UE_LOG(LogTemp, Display, TEXT("Projectile Owner : %s"),*ProjectileOwner->GetActorNameOrLabel());


	}
}

void ABasePawn::HandleDestruction()
{
	// A shared function to handle the destruction of the pawn and its components
	// helpful to play other effects for death of the pawn and enemy

	if (DeathParticle)
	{
		// particle effect player after the player is died
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), 
			DeathParticle, GetActorLocation(),GetActorRotation());
	}
	
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
	}

	if (DeathCameraShakeClass)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			PlayerController->ClientStartCameraShake(DeathCameraShakeClass);
		}
	}
	//UE_LOG(LogTemp, Display, TEXT("BASE PAWN HANDLE DESTRUCTION"));
}

