// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessageWidget.h"

void UScreenMessageWidget::SetTextMessage(FString Message)
{
	// need to convet from Fstring to FText since the settext() accepts the FText argument
	FText MessageText = FText::FromString(Message);

	// function used to set text based on game status win or lose or start game.
	MessageTextBlock->SetText(MessageText);
}
