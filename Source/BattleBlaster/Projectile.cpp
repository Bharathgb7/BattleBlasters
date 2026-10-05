// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating the projectile mesh and setting it as the root component
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);

	// Creating the projectile movement component on projectile blueprint
	ProjectileMovementComponent = 
		CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));



	// creating and attaching trail particle effect in blueprint
	TrailParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail Particle"));
	TrailParticles->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// bind that delegate function that gives the properties of the object 
	// that the projectile hits to the OnComponentHit event of the projectile mesh 
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	// playing the laucnch sound after creating the projectile mesh
	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), LaunchSound, GetActorLocation());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// function used to identify the properties of the object that the projectile hits 
	// and to apply damage to it

	// using unreal's apply damage function to apply damage to the object that the projectile hits
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		if (OtherActor && OtherActor != MyOwner && OtherActor != this)
		{
			UGameplayStatics::ApplyDamage(OtherActor,
				Damage,
				MyOwner->GetInstigatorController(), // the controller that causes the damage
				this, // the actor that causes the damage
				UDamageType::StaticClass() // generic damage type class that can be used to apply damage to the object
			);
			
			// spawning the particle after applying damage because we need to do after inflicting the damage 
			// not to interrupt the damaging of the adversary, which follow the projectile as a trail

			if (HitParticleAsset)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), 
					HitParticleAsset, GetActorLocation(), GetActorRotation());
			}

			// playing the hit sound when the projectile hits
			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
			}
			if (HitCameraShakeClass)
			{
				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (PlayerController)
				{
					PlayerController->ClientStartCameraShake(HitCameraShakeClass);
				}
			}
		}
	}
	Destroy(); // destroy the projectile after it hits an object
}



