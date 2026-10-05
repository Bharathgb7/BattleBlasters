// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
#include "EnemyTower.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// creating an array to store all enemy tower instances found on the world while playing
	TArray<AActor*>EnemyTowers;

	// Function used to get all actors of a certain class  
	// to get all the reference of enemy class helps the player in game to find enemies
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyTower::StaticClass(), EnemyTowers);

	//monitoring those enemy through count 
	TowerCount = EnemyTowers.Num();
	UE_LOG(LogTemp, Display, TEXT("Enemy Tower Count = %d"), TowerCount);

	// getting normal player pawn that default to the engine, for getting player tank 
	APawn* NormalPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	if (NormalPawn)
	{
		// converting that normal pawn to our tank pawn 
		// inorder to access its location and give to enemy for identification
		PlayerTank = Cast<APlayerTank>(NormalPawn);
		if (!PlayerTank)
		{
			UE_LOG(LogTemp, Display, TEXT("Cast UnSucessfull"));
		}
	}

	// loop index variable that'll control the loop to avoid infinite looping
	int32 LoopIndex = 0;

	// using while loop to assign the og tank pawn variable to all of the enemy instance found on the world
	// use the .num() function to measure the count of array instead of hardcoding directly 
	while (LoopIndex<TowerCount)
	{
		// accessing every individual element of the array by indexing(Arrayname[LoopIndex])
		// inorder to use the element we store it in an actor pointer
		AActor* TowerActor = EnemyTowers[LoopIndex];
		if (TowerActor && PlayerTank)
		{

		// we cast the tower actor var to our enemy tower 
		// inorder to set the og tank var to the reference of tank var that created in a enemytower class
			AEnemyTower* EnemyTower = Cast<AEnemyTower>(TowerActor);
		
		// now accessing the enemytower's playertank var to store the player tank var reference that created 
		// in the game mode class 
			EnemyTower->PlayerTank = PlayerTank;
			UE_LOG(LogTemp, Display, TEXT("Setting the tank variable = %s !"), 
				*EnemyTower->GetActorNameOrLabel());
		}
		// increment everytime if the condition is true after executing the body
		LoopIndex++;
	}

	// why not use single var ? we will lose the orginal countdown value in countdownDelay 
	// in case we want to do something using this in future 
	CountdownSeconds = CountdownDelay;

	// display get ready message via widget blueprint and widget class
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if (PlayerController)
	{
		//to create Widget 
		ScreenMessageWidget = CreateWidget<UScreenMessageWidget>(PlayerController, ScreenMessageWidgetClass);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen();

			// get ready message passed via from screenwidgetclass function
			ScreenMessageWidget->SetTextMessage("Get Ready!");
		}
	}

	// here the countdown starts 
	GetWorldTimerManager().SetTimer(CountdownTimerHandle,
		this, &ABattleBlasterGameMode::OnCountDownTimerTimeOut, CountdownDelay, 1.0f,true);
}
void ABattleBlasterGameMode::OnCountDownTimerTimeOut()
{
	//used to reduce countdown by 1 on everysecond and passed to timer
	CountdownSeconds -= 1;

	if (CountdownSeconds > 0)
	{
		// displaying the countdown in string to convert use fstring::fromint
		ScreenMessageWidget->SetTextMessage(FString::FromInt(CountdownSeconds));
	}
	else if (CountdownSeconds == 0)
	{
		// displaying the countdown in string to convert use fstring::fromint
		ScreenMessageWidget->SetTextMessage("GO!");

		// enabling player input
		PlayerTank->SetPlayerEnabled(true);
	}
	else
	{
		// clears the timer if the timer goes beyond 0 -vely
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

		// to clear the widget after showing the timer 
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
void ABattleBlasterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	// variables used to track the game over state of the game
	bool IsGameOver = false;
	IsVictory = false;

	// checking if the dead actor is the player tank or enemy
	if (DeadActor == PlayerTank)
	{
		PlayerTank->HandleDestruction();
		UE_LOG(LogTemp, Display, TEXT("Player Tank Destroyed"));
		IsGameOver = true;
	}
	else
	{
		// performing a cast to get the reference of the dead enemy tower
		AEnemyTower* DeadTower = Cast<AEnemyTower>(DeadActor);
		if (DeadTower)
		{
			DeadTower->HandleDestruction();

			// reducing the count of enemy tower when one of them is destroyed
			TowerCount--;

			//checking if all the enemy tower is destroyed or not
			if (TowerCount == 0)
			{
				IsGameOver = true;
				IsVictory = true;
				UE_LOG(LogTemp, Display, TEXT("All Enemy Tower Destroyed"));	
			}
		}
	}

	// check whether the game is over or not based on that we restart the level
	// print win or lose message in the log
	if (IsGameOver)
	{
		// using terneary operator to check the game over state and print the result in the log
		// condtion ? true statement : false statement
		FString GameOverString = IsVictory ? "Victory!" : "Defeat!";

		// displaying message based on widget on screen
		ScreenMessageWidget->SetTextMessage(GameOverString);

		// to show the widget after completing the game
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);

		// function used to call restart game with a delay after the game over state is reached
		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle,
			this, &ABattleBlasterGameMode::OnGameOverTimeOut, GameOverDelay, false);
	}

	
}

void ABattleBlasterGameMode::OnGameOverTimeOut()
{
	UGameInstance* GameInstance = GetGameInstance();
	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);

	if (MyGameInstance)
	{
		if (IsVictory)
		{
			// if player wins it loads next level
			MyGameInstance->LoadNextLevel();
		}
		else
		{
			// contradictory to above if statement
			MyGameInstance->RestartCurrentLevel();
		}
	}
}


