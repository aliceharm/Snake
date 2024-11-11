// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "PlayerPawnBase.h"
#include "Engine/World.h"
#include "Interacteble.h"


// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 100.f;
	LastMoveDirection = EMovementDirection::DOWN;
	FoodCount = 0;
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
	
	

	PlayerPawn = Cast<APlayerPawnBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerPawn->SetTutValue(FoodCount);
	
}

int32 ASnakeBase::GetFoodCount() const
{
	return FoodCount;
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
	Move();
	
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
	//AddActorWorldOffset(MovementVector);
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
	//if (MovementSpeed < 50.f)
	//{
	//	MovementSpeed = 50.f; // Минимальная скорость
	//}
	//else if (MovementSpeed > 150.f)
	//{
	//	MovementSpeed = 150.f; // Максимальная скорость
	//}
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