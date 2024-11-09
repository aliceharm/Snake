// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Food.h"
#include "FoodSlow.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE_API AFoodSlow : public AFood
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* Interactor, bool bIsHead) override;

	
};
