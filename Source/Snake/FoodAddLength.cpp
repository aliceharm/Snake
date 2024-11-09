// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodAddLength.h"
#include "SnakeBase.h"



void AFoodAddLength::Interact(AActor* Interactor, bool bIsHead)
{
    Super::Interact(Interactor, bIsHead);

    if (bIsHead)
    {
        auto Snake = Cast<ASnakeBase>(Interactor);
        if (IsValid(Snake))
        {
            Snake->AddSnakeElement(2); // Увеличиваем длину
            
            //DestroyFood();
        }
    }

}
