// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CharacterCreatePanel.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "UI_CharacterButton.h"
#include "Components/ScrollBoxSlot.h"

void UUI_CharacterCreatePanel::NativeConstruct()
{
	Super::NativeConstruct();

	InitCharacterButton(4);
}

void UUI_CharacterCreatePanel::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_CharacterCreatePanel::InitCharacterButton(const int32 InNumber)
{
	if (UI_CharacterButtonClass)
	{
		for (int32 i = 0; i < InNumber; i++)
		{
			if (UUI_CharacterButton* InCharacterButton = CreateWidget<UUI_CharacterButton>(GetWorld(), UI_CharacterButtonClass))
			{
				InCharacterButton->SetParents(this);
				if (UScrollBoxSlot* InScrollBoxSlot = Cast<UScrollBoxSlot>(ScrollList->AddChild(InCharacterButton)))
				{
					InScrollBoxSlot->SetPadding(10.0f);
				}
			}
		}
	}
}
