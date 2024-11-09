// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodSlow.h"
#include "SnakeBase.h"


void AFoodSlow::Interact(AActor* Interactor, bool bIsHead)
{
    Super::Interact(Interactor, bIsHead);

    if (bIsHead)
    {
        auto Snake = Cast<ASnakeBase>(Interactor);
        if (IsValid(Snake))
        {
            
            Snake->SlowDown(2); // Вызываем метод замедления или убыстрения с рандомным значением
            Snake->AddSnakeElement();
            //DestroyFood();
        }
    }

}
