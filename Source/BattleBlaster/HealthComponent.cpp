// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	// set the health to max health at the start of the game
	Health = MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnDamageTaken);

	// performing casting to get our gamemode from default gamemode class
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode)
	{
		BattleBlasterGameMode = Cast<ABattleBlasterGameMode>(GameMode);
	}
}



// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	// function used to taked care of the damage applied by apply damage function in the projectile class 
	if (Damage > 0.0f)
	{
		Health -= Damage;
		if (Health == 0)
		{
			// passing the dead actor from health component to the gamemode to handle the death of the actor
			BattleBlasterGameMode->ActorDied(DamagedActor);
		}
	}
}

