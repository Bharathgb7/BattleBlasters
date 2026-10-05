// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::ChangeLevel(int32 Index)
{
	// function used to change the level based on the index provided
	// checks the index is above 0 and below the last level index to avoid any errors

	if (Index > 0 && Index <= LastLevelIndex)
	{
		// making the current level index equal to the index provided to change the level
		CurrentLevelIndex = Index;
		FString CurrentLevelName = FString::Printf(TEXT("Level_%d"), CurrentLevelIndex);
		
		// to restart the level after the game over state is reached
		UGameplayStatics::OpenLevel(GetWorld(), *CurrentLevelName);
	}
}
void UMyGameInstance::LoadNextLevel()
{
	// checking if the current level index is less than the last level index 
	// to avoid progress beyond the last level
	if (CurrentLevelIndex < LastLevelIndex)
	{
		ChangeLevel(CurrentLevelIndex + 1);
	}
	else
	{
		// if the current level index is equal to the last level index then we restart the game
		RestartGame();
	}
}
void UMyGameInstance::RestartCurrentLevel()
{
	ChangeLevel(CurrentLevelIndex);
}
void UMyGameInstance::RestartGame()
{
	// restart the game from the level 1 after the entire game level completion
	ChangeLevel(1);
}
