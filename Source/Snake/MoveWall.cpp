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
    PrimaryActorTick.bCanEverTick = false; // ����� �� ��������� � ������ �����
     // ����� ����� �����
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
    
    // ��������� ������ �����
    bool bIsValidLocation = false;
    int MaxAttempts = 10;
    int Attempt = 0;

    while (!bIsValidLocation && Attempt < MaxAttempts)
    {
        NewLocation = GenerateRandomLocation();
        bIsValidLocation = !IsLocationOccupied(NewLocation);
        Attempt++;
    }

    // ���� ����� �������� ��������������, ������� ����� �����
    if (bIsValidLocation)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            AMoveWall* NewWall = World->SpawnActor<AMoveWall>(GetClass(), NewLocation, FRotator::ZeroRotator);
        }
    }

    Destroy(); // ���������� ������ �����
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
        return FVector(X, Y, 0); // ��������������, ��� Z ���������� �������� 0
    }

    return FVector::ZeroVector;
}

bool AMoveWall::IsLocationOccupied(const FVector& Location)
{
    
   

    

    // ��������� �� ����������� � ����
    TArray<AActor*> OverlappingFood;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFood::StaticClass(), OverlappingFood);

    for (AActor* Food : OverlappingFood)
    {
        if (Food->GetActorLocation().Equals(Location, 100.0f)) // �������� � ��������� ��������
        {
            return true; // ����� ������ ����
        }
    }

    // ��������� �� ����������� �� ������
    TArray<AActor*> OverlappingSnakes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASnakeBase::StaticClass(), OverlappingSnakes);

    for (AActor* Snake : OverlappingSnakes)
    {
        ASnakeBase* SnakeBase = Cast<ASnakeBase>(Snake);
        if (IsValid(SnakeBase) && SnakeBase->IsInSnake(Location)) // �������� �� ���������� � ����
        {
            return true; // ����� ������ �����
        }
        float MinimumDistance = 100.0f;
        TArray<AActor*> OverlappingActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASnakeBase::StaticClass(), OverlappingActors);
// ��������� �� ����������� �� �������
        TArray<AActor*> WallActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWall::StaticClass(), WallActors);
        for (AActor* Wall : WallActors)
        {
            if (IsValid(Wall))
            {
                if (Wall->GetActorLocation().Equals(Location, MinimumDistance))
                {
                    return true; // ����� ������ ������
                }
            }
        }

        for (AActor* Actor : OverlappingActors)
        {
            ASnakeBase* OtherSnake = Cast<ASnakeBase>(Actor);
            if (IsValid(OtherSnake))
            {
                // ��������� ���������� �� ���� ��������� ����
                for (const FVector& SegmentLocation : OtherSnake->GetSnakeBodySegments())
                {
                    if (FVector::Dist(Location, SegmentLocation) < MinimumDistance)

                    {
                        return true; // ��������� ������� ������ � ���� ����
                    }
                }
            }
        }
    }

    return false; // ����� ��������
}
