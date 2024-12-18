// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeElementBase.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SnakeGameModeBase.h"
#include "SnakeBase.h"
#include "Wall.h"


// Sets default values
ASnakeElementBase::ASnakeElementBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

}

// Called when the game starts or when spawned
void ASnakeElementBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASnakeElementBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASnakeElementBase::SetFirstElementType_Implementation()
{
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASnakeElementBase::HandleBeginOverLap);
}

void ASnakeElementBase::Interact(AActor* Interactor, bool bIsHead)
{
	auto Snake = Cast<ASnakeBase>(Interactor);
	
	if (IsValid(Snake))
	{
		Snake->Destroy();

		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			ASnakeGameModeBase* GameMode = Cast<ASnakeGameModeBase>(UGameplayStatics::GetGameMode(World));
			if (GameMode != nullptr)
			{
				GameMode->EndGame(EGameEndCause::LoseGame);
			}
		}
	}
	
}

void ASnakeElementBase::HandleBeginOverLap(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex,
		bool bFromSweep, 
		const FHitResult& SweepResult)
{
	if (IsValid(SnakeOwner))
	{
		SnakeOwner->SnakeElementOverlap(this, OtherActor);
	}
	if (OtherActor && OtherActor->IsA(AWall::StaticClass()))
		{
		// ���������� ���� ��� ������������ �� ������
		if (IsValid(SnakeOwner))
			{
			UWorld* World = GetWorld();
			if (World != nullptr)
			{
				ASnakeGameModeBase* GameMode = Cast<ASnakeGameModeBase>(UGameplayStatics::GetGameMode(World));
				if (GameMode != nullptr)
				{
					GameMode->EndGame(EGameEndCause::LoseGame);
					Destroy();
				}
			}
			SnakeOwner->Destroy();
					}
	}

}

void ASnakeElementBase::ToggleCollision()
{
	if (MeshComponent->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
	{
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

