// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SnakeGameModeBase.generated.h"

class UEndGameUI;
/**
 * 
 */
UENUM(BlueprintType)
enum class EGameEndCause : uint8
{
	LoseGame = 0 UMETA(DisplayName = "Lose Game"),
	WinGame = 1 UMETA(DisplayName = "Win Game"),
	TimeLost = 2 UMETA(DisplayName = "Time Lost")
};

UCLASS()
class SNAKE_API ASnakeGameModeBase : public AGameModeBase
{

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UEndGameUI> EndGameUIClass;

protected:
	UPROPERTY()
	UEndGameUI* EndGameUI = nullptr;



public:
	UFUNCTION()
	void EndGame(EGameEndCause Cause = EGameEndCause::LoseGame);

protected:
	UFUNCTION()
	void UpdateEndGameUI(EGameEndCause Cause = EGameEndCause::LoseGame);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<EGameEndCause, FString> EndGameStrings;


	GENERATED_BODY()
	
};
