// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameUI.generated.h"

/**
 * 
 */



UCLASS()
class SNAKE_API UEndGameUI : public UUserWidget
{
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString EndGameText = "Deafault";

	GENERATED_BODY()
	
};
