// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_SkinColorButton.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UUI_SkinColorButton::NativeConstruct()
{
	Super::NativeConstruct();

	ColorButton->OnClicked.AddDynamic(this, &UUI_SkinColorButton::ColorButtonClicked);
}

void UUI_SkinColorButton::NativeDestruct()
{
	Super::NativeDestruct();

}

void UUI_SkinColorButton::ColorButtonClicked()
{

}
