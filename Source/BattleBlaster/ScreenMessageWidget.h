// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ScreenMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API UScreenMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// here meta = BindWidget and the name(Must be same name from WBP's TextBlock Name) 
	// to connect this variable to our WBP's TextBlock 
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MessageTextBlock;

	void SetTextMessage(FString Message);
};
