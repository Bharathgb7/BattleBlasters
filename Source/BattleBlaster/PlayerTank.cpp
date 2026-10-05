// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTank.h"
// forward declaration -> remove header from .h to add in here 
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

APlayerTank::APlayerTank()
{
	// must be included to run tick function 
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	// here using capsule as root component because since this class is a inherited class of basepawn
	SpringArm->SetupAttachment(CapsuleComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm);
}

void APlayerTank::BeginPlay()
{
	Super::BeginPlay();
	// to get access player controller using a normal pawn controller , can be declared inside if condition block
	// be aware those var not be used in outside of local code block
	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		//to get access to local player from the player controller 
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			//to get subsystem(needed for implementation of enhanced I/P system) 
			// that uses enhanced input controller
			if (UEnhancedInputLocalPlayerSubsystem* SubSystem = 
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
				(LocalPlayer))
			{
				// using the susbsystem to assign the user created IMC by passing 
				// Our IMC var created in c++ that'll be assigned later on BP
				SubSystem->AddMappingContext(InputMC, 0);
			}
		}
		
	}

	// disabling player input on startup to avoid moving before countdown finishes
	SetPlayerEnabled(false);

}

void APlayerTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		// to draw a trace on our mouse cursor to turn the turret based on our mouse movement
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.0f, 12, FColor::Red);

		// rotating the turret based on our mouse location the function is from parent class 
		// because both enemy and player need to rotate their heads to shoot
		RotateTurret(HitResult.ImpactPoint);
	}
}

//called to bind functionality to input
void APlayerTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// getting enhancedinputcomponent from playerinputcomponent using Cast<>();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// binding our input action for moving front and back also for left and right
		EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&APlayerTank::MoveInput);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &APlayerTank::TurnInput);

		// here base pawn can be called because it is a parent class of player tank 
		// the function fire is in basepawn class since it is going to be inherited by both enemy and player to shoot
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ABasePawn::Fire);
	}
}

void APlayerTank::HandleDestruction()
{
	// using super keyword to call the parent class function to handle the destruction of the player tank
	Super::HandleDestruction();

	// making the player tank invisible and disabling the tick function to stop it 
	// from moving after it is destroyed
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetPlayerEnabled(false);
	IsAlive = false;
}

void APlayerTank::SetPlayerEnabled(bool Enabled)
{
	// make our own input enable and disable function to control the player tank movement 

	if (PlayerController)
	{
		// Enabled is passed via from handledestruction function to determine whether player dead or not

		if (Enabled)
		{
			EnableInput(PlayerController);
		}
		else
		{
			DisableInput(PlayerController);
		}

		// enabling and disabling the mouse cursor to be visible when the player is dead or alive
		PlayerController->bShowMouseCursor = Enabled;
	}
}

void APlayerTank::MoveInput(const FInputActionValue& Value)
{
	// getting the input value to identify the key we press w or s to identify the direction 
	// remember to set the negate in Input Mapping Context of S key to get -1 
	float InputValue = Value.Get<float>();
	
	//using an origin location to alter the value of tank's location to move
	FVector DeltaLocation = FVector(0.0f, 0.0f, 0.0f);
	
	// inorder to move forward altering its local x which faces forward
	// calculated by giving a speed value to get a distance by(Distance = speed * Time) and multiplying with input value to decide the direction
	// here distance is the delta location speed is user defined value and time is deltatime(can be used to make it as independently run
	// to make it frame rate dependable we're using deltatime to get that we use kismet's gameplat statics function
	DeltaLocation.X = Speed * InputValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	
	// function that alters the original actor location in locally to move 
	AddActorLocalOffset(DeltaLocation,true);



}

void APlayerTank::TurnInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	//same as previous we use frotator to access the pitch,yaw,roll of the object(refer flying an aircraft to know this concept)
	FRotator DeltaRotation = FRotator(0.0f, 0.0f, 0.0f);

	// we create a user define float var called turnrate to control the turning angle of the turret either left or right
	DeltaRotation.Yaw = TurnRate * InputValue * GetWorld()->GetDeltaSeconds();

	// function that alters the original actor rotation in locally to rotate left or right
	AddActorLocalRotation(DeltaRotation, true);
}
