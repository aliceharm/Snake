// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawnBase.generated.h"

class UCameraComponent;
class ASnakeBase;

UCLASS()
class SNAKE_API APlayerPawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawnBase();

	UPROPERTY(BlueprintReadWrite)
	UCameraComponent* PawnCamera;

	UPROPERTY(BlueprintReadWrite)
	ASnakeBase* SnakeActor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeBase> SnakeActorClass;

	UPROPERTY(BlueprintReadWrite, Category = "Tut")
	int tut;

	UPROPERTY(BlueprintReadWrite, Category = "Food")
	float StarvationNow;

	UFUNCTION(BlueprintCallable, Category = "Food")
	void SetStarvationNow();

	UFUNCTION(BlueprintCallable, Category = "Tut")
	void SetTutValue(int NewTutValue);

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void CreateSnakeActor();
	void StartSnakeMovement();

	UFUNCTION()
	void HandlePlayerVerticalInput(float value);
	UFUNCTION()
	void HandlePlayerHorizontalInput(float value);

	

};
