// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "SnakeBase.h"
#include "Kismet/KismetMathLibrary.h" 
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "FoodAddLength.h"
#include "FoodFast.h"
#include "FoodShorten.h"
#include "FoodSlow.h"
#include "Wall.h"
#include "SnakeGameModeBase.h"



// Sets default values
AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    FoodClasses.Add(AFoodAddLength::StaticClass());
    FoodClasses.Add(AFoodShorten::StaticClass());
    FoodClasses.Add(AFoodFast::StaticClass());
    FoodClasses.Add(AFoodSlow::StaticClass());
           
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFood::Interact(AActor* Interactor, bool bIsHead)
{
    if (bIsHead)
    {
        auto Snake = Cast<ASnakeBase>(Interactor);
        if (IsValid(Snake))
        {
            Snake->IncrementFoodCount();
            
            Snake->AddSnakeElement();
            SpawnNewFood(Snake);

            
            DestroyFood();
        }

    }
}

void AFood::DestroyFood()
{
    Destroy();
}

void AFood::SpawnNewFood(ASnakeBase* Snake)
{
    UWorld* World = GetWorld();
    if (World)
    {
        FVector NewLocation;

        bool bIsValidLocation = false;
        int MaxAttempts = 10;
        int Attempt = 0;

        while (!bIsValidLocation && Attempt < MaxAttempts)
        {
            NewLocation = GenerateRandomLocation();
            bIsValidLocation = !IsLocationOccupied(NewLocation, Snake);
            Attempt++;
        }

        if (FoodClasses.Num() > 0)
        {
            // Случайный индекс
            int32 RandomIndex = FMath::RandRange(0, FoodClasses.Num() - 1);
            TSubclassOf<AFood> RandomFoodClass = FoodClasses[RandomIndex];

            // Логика спавна
            if (bIsValidLocation)
            {
                AFood* NewFood = World->SpawnActor<AFood>(RandomFoodClass, NewLocation, FRotator::ZeroRotator);
            }
        }
    }
}

FVector AFood::GenerateRandomLocation()
{
    UWorld* World = GetWorld();
    if (World)
    {
        
        float MinX = -500.0f; 
        float MaxX = 500.0f; 
        float MinY = -500.0f; 
        float MaxY = 500.0f; 

       
        FVector NewLocation;
        bool bIsInSnake = true;
        while (bIsInSnake)
        {
            float X = FMath::RandRange(MinX, MaxX);
            float Y = FMath::RandRange(MinY, MaxY);
            NewLocation = FVector(X, Y, 0); 

            TArray<AActor*> OverlappingActors;
            UGameplayStatics::GetAllActorsOfClass(World, ASnakeBase::StaticClass(), OverlappingActors);

            bIsInSnake = false;
            for (AActor* Actor : OverlappingActors)
            {
                ASnakeBase* Snake = Cast<ASnakeBase>(Actor);
                if (IsValid(Snake) && Snake->IsInSnake(NewLocation))
                {
                    bIsInSnake = true;
                    break;
                }
            }
        }

        
        return NewLocation;
    }

    
    return FVector::ZeroVector;
}

bool AFood::IsLocationOccupied(const FVector& Location, ASnakeBase* Snake)
{
    if (Snake->IsInSnake(Location))
    {
        return true;
    }

    // Дополнительная проверка расстояния от тела змеи
    float MinimumDistance = 100.0f; 
    TArray<AActor*> OverlappingActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASnakeBase::StaticClass(), OverlappingActors);

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
    TArray<AActor*> WallActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWall::StaticClass(), WallActors);

    for (AActor* Wall : WallActors)
    {
        if (IsValid(Wall))
        {
            // Предполагаем, что стене есть метод для получения её местоположения и размеров
            if (Wall->GetActorLocation().Equals(Location, MinimumDistance))
            {
                return true; // Точка занята стеной
            }
        }
    }

    return false;
}
