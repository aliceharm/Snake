// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveWall.h"
#include "SnakeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Food.h"

// Sets default values
AMoveWall::AMoveWall()
{
    PrimaryActorTick.bCanEverTick = false; // Стена не нуждается в каждом кадре
     // Время жизни стены
}

// Called when the game starts or when spawned
void AMoveWall::BeginPlay()
{
    Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(LifeTimerHandle, this, &AMoveWall::DestroyAndRespawn, LifeTime, false);
}

void AMoveWall::DestroyAndRespawn()
{
    FVector NewLocation;
    
    // Генерация нового места
    bool bIsValidLocation = false;
    int MaxAttempts = 10;
    int Attempt = 0;

    while (!bIsValidLocation && Attempt < MaxAttempts)
    {
        NewLocation = GenerateRandomLocation();
        bIsValidLocation = !IsLocationOccupied(NewLocation);
        Attempt++;
    }

    // Если нашли валидное местоположение, спауним новую стену
    if (bIsValidLocation)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            AMoveWall* NewWall = World->SpawnActor<AMoveWall>(GetClass(), NewLocation, FRotator::ZeroRotator);
        }
    }

    Destroy(); // Уничтожаем старую стену
}

FVector AMoveWall::GenerateRandomLocation()
{
    UWorld* World = GetWorld();
    if (World)
    {
        float MinX = -500.0f;
        float MaxX = 500.0f;
        float MinY = -500.0f;
        float MaxY = 500.0f;

        float X = FMath::RandRange(MinX, MaxX);
        float Y = FMath::RandRange(MinY, MaxY);
        return FVector(X, Y, 0); // Предполагается, что Z координата остается 0
    }

    return FVector::ZeroVector;
}

bool AMoveWall::IsLocationOccupied(const FVector& Location)
{
    
   

    

    // Проверяем на пересечение с едой
    TArray<AActor*> OverlappingFood;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFood::StaticClass(), OverlappingFood);

    for (AActor* Food : OverlappingFood)
    {
        if (Food->GetActorLocation().Equals(Location, 100.0f)) // Проверка с некоторым допуском
        {
            return true; // Место занято едой
        }
    }

    // Проверяем на пересечение со змеями
    TArray<AActor*> OverlappingSnakes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASnakeBase::StaticClass(), OverlappingSnakes);

    for (AActor* Snake : OverlappingSnakes)
    {
        ASnakeBase* SnakeBase = Cast<ASnakeBase>(Snake);
        if (IsValid(SnakeBase) && SnakeBase->IsInSnake(Location)) // Проверка на нахождение в змее
        {
            return true; // Место занято змеей
        }
        float MinimumDistance = 100.0f;
        TArray<AActor*> OverlappingActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASnakeBase::StaticClass(), OverlappingActors);
// Проверяем на пересечение со стенами
        TArray<AActor*> WallActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWall::StaticClass(), WallActors);
        for (AActor* Wall : WallActors)
        {
            if (IsValid(Wall))
            {
                if (Wall->GetActorLocation().Equals(Location, MinimumDistance))
                {
                    return true; // Точка занята стеной
                }
            }
        }

        for (AActor* Actor : OverlappingActors)
        {
            ASnakeBase* OtherSnake = Cast<ASnakeBase>(Actor);
            if (IsValid(OtherSnake))
            {
                // Проверяем расстояние до всех сегментов тела
                for (const FVector& SegmentLocation : OtherSnake->GetSnakeBodySegments())
                {
                    if (FVector::Dist(Location, SegmentLocation) < MinimumDistance)

                    {
                        return true; // Положение слишком близко к телу змеи
                    }
                }
            }
        }
    }

    return false; // Место свободно
}
