// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Food.h"
#include "FoodShorten.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE_API AFoodShorten : public AFood
{
	GENERATED_BODY()
public:
	virtual void Interact(AActor* Interactor, bool bIsHead) override;
};
