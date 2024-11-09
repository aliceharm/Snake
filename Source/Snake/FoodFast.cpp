// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodFast.h"
#include "SnakeBase.h"

void AFoodFast::Interact(AActor* Interactor, bool bIsHead)
{
    Super::Interact(Interactor, bIsHead);

    if (bIsHead)
    {
        auto Snake = Cast<ASnakeBase>(Interactor);
        if (IsValid(Snake))
        {

            Snake->SlowDown(0.5); // �������� ����� ���������� ��� ���������� � ��������� ���������
            Snake->AddSnakeElement();
            //DestroyFood();
        }
    }
}
