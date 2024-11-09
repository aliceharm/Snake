// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interacteble.h"
#include "Food.generated.h"


class ASnakeBase;


UCLASS()
class SNAKE_API AFood : public AActor, public IInteracteble
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFood();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Food")
	TArray<TSubclassOf<AFood>> FoodClasses;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Interactor, bool bIsHead) override;

	virtual void DestroyFood();

	virtual void SpawnNewFood(ASnakeBase* Snake);

	virtual FVector GenerateRandomLocation();
	virtual bool IsLocationOccupied(const FVector& Location, ASnakeBase* Snake);
};
