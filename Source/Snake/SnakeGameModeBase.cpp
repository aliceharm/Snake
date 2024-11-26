// Copyright Epic Games, Inc. All Rights Reserved.


#include "SnakeGameModeBase.h"
#include "EndGameUI.h"
#include "Kismet/GameplayStatics.h"


void ASnakeGameModeBase::EndGame(EGameEndCause Cause)
{
	UpdateEndGameUI(Cause);
	
}

void ASnakeGameModeBase::UpdateEndGameUI(EGameEndCause Cause)
{
	if (EndGameUIClass == nullptr) return;

	const UWorld* GameWorld = GetWorld();
	if (GameWorld == nullptr) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GameWorld, 0);
	EndGameUI = Cast<UEndGameUI>(CreateWidget(PC, EndGameUIClass));

	if (EndGameUI == nullptr) return;
	FString* Message = EndGameStrings.Find(Cause);
	if (Message == nullptr) return;

	EndGameUI->EndGameText = *Message;

	EndGameUI->AddToViewport();
}
