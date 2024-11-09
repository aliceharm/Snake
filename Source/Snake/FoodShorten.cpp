// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodShorten.h"
#include "SnakeBase.h"

void AFoodShorten::Interact(AActor* Interactor, bool bIsHead)
{
    Super::Interact(Interactor, bIsHead);

    if (bIsHead)
    {
        auto Snake = Cast<ASnakeBase>(Interactor);
        if (IsValid(Snake))
        {
            Snake->Shorten(); // Вызываем метод укорачивания
           
        }
    }
}


