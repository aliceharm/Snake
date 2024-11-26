// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Wall.h"
#include "MoveWall.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE_API AMoveWall : public AWall
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMoveWall();

	UPROPERTY()
	FTimerHandle LifeTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifetime")
	float LifeTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	
	
	void DestroyAndRespawn();
	FVector GenerateRandomLocation();
	bool IsLocationOccupied(const FVector& Location);

	

};
