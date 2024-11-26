
#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "PlayerPawnBase.h"
#include "Engine/World.h"
#include "Interacteble.h"
#include "Kismet/GameplayStatics.h"
#include "SnakeGameModeBase.h"


ASnakeBase::ASnakeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 100.f;
	LastMoveDirection = EMovementDirection::DOWN;
	FoodCount = 0;
	MaxFood = 50;
	TimeSinceLastFood = 0.0f;
	MaxTime = 20.0f;
	bIsAlive = true;
	bIsMoving = false;
}

void ASnakeBase::StartMovement()
{
	bIsMoving = true; // Устанавливаем флаг, что движение начато
}

TArray<FVector> ASnakeBase::GetSnakeBodySegments() const
{
	TArray<FVector> BodyPositions;
	for (const auto& Element : SnakeElements)
		{
		BodyPositions.Add(Element->GetActorLocation());
		// Добавляем позицию элемента в массив
	}
	return BodyPositions; // Возвращаем массив позиций
}

void ASnakeBase::IncrementFoodCount()
{
	FoodCount++;
	TimeSinceLastFood = 0.0f;
	PlayerPawn = Cast<APlayerPawnBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerPawn->SetTutValue(FoodCount);

	if (FoodCount == 15)
	{
		SlowDown(0.8);
	}

	if (FoodCount == 30)
	{
		SlowDown(0.8);
	}

	if (FoodCount == MaxFood)
	{
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			ASnakeGameModeBase* GameMode = Cast<ASnakeGameModeBase>(UGameplayStatics::GetGameMode(World));
			if (GameMode != nullptr)
			{
				GameMode->EndGame(EGameEndCause::WinGame);
				Destroy();
			}
		}
	}
}

int32 ASnakeBase::GetFoodCount() const
{
	return FoodCount;
}

float ASnakeBase::GetTimeSinceLastFood() const
{
	return TimeSinceLastFood;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(5);
	
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsAlive && bIsMoving) // Добавляем проверку на bIsMoving
	{
		Move();
		if (bIsAlive)
		{
			TimeSinceLastFood += DeltaTime;
			// Проверка на смерть
			if (TimeSinceLastFood >= MaxTime)
			{
				UWorld* World = GetWorld();
				if (World != nullptr)
				{
					ASnakeGameModeBase* GameMode = Cast<ASnakeGameModeBase>(UGameplayStatics::GetGameMode(World));
					if (GameMode != nullptr)
					{
						GameMode->EndGame(EGameEndCause::TimeLost);
					}
				}
				Destroy(); // Если прошло 20 секунд, делаем змею мертвой
			}
		}
	}
}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	for (int i = 0; i < ElementsNum; ++i)
	{
		FVector NewLocation;
		if (SnakeElements.Num() > 0)
		{
			FVector LastElementLocation = SnakeElements.Last()->GetActorLocation();
			NewLocation = LastElementLocation;
		}
		else
		{
			NewLocation = FVector(0, 0, 0);
		}
		FTransform NewTransform(NewLocation);
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		NewSnakeElem->SnakeOwner = this;
		int32 ElemIndex = SnakeElements.Add(NewSnakeElem);
		if (ElemIndex == 0)
		{
			NewSnakeElem->SetFirstElementType();
		}
	}
}

void ASnakeBase::Move()
{
	FVector MovementVector(ForceInitToZero);
	
	switch (LastMoveDirection)
	{
	case EMovementDirection::UP:
		MovementVector.X += ElementSize;
		break;

	case EMovementDirection::DOWN:
		MovementVector.X -= ElementSize;
		break;

	case EMovementDirection::LEFT:
		MovementVector.Y += ElementSize;
		break;

	case EMovementDirection::RIGHT:
		MovementVector.Y -= ElementSize;
		break;
	}
	SnakeElements[0]->ToggleCollision();
	for (int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		auto CurrentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->SetActorLocation(PrevLocation);
	}
	SnakeElements[0]->AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();
}

void ASnakeBase::SnakeElementOverlap(ASnakeElementBase* OverLappedElement, AActor* Other)
{
	if (IsValid(OverLappedElement))
	{
		int32 ElemIndex;
		SnakeElements.Find(OverLappedElement, ElemIndex);
		bool bIsFirst = ElemIndex == 0;
		IInteracteble* InteractebleInterface = Cast<IInteracteble>(Other);
		if (InteractebleInterface)
		{
			InteractebleInterface->Interact(this, bIsFirst);
		}
	}
}

bool ASnakeBase::IsInSnake(const FVector& Location)
{
	for (auto SnakeElement : SnakeElements)
	{
		if (SnakeElement->GetActorLocation() == Location)
		{
			return true; 
		}
	}
	return false;
}

void ASnakeBase::SlowDown(float Factor)
{
	MovementSpeed *= Factor;
	SetMovementSpeed(MovementSpeed);
}
void ASnakeBase::SetMovementSpeed(float NewSpeed)
{
	if (NewSpeed >= 0)
	{
		MovementSpeed = NewSpeed;
		SetActorTickInterval(MovementSpeed);
	}
}

void ASnakeBase::Shorten()
{
	if (SnakeElements.Num() > 1) // Проверяем, есть ли более одного сегмента
	{
		SnakeElements.Pop(); // Удаляем последний сегмент из тела змеи
	}
}