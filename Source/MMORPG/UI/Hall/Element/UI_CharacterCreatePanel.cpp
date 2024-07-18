// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CharacterCreatePanel.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "UI_CharacterButton.h"
#include "Components/ScrollBoxSlot.h"
#include "UI_KneadFace.h"
#include "../../../Core/Hall/HallPlayerState.h"

void UUI_CharacterCreatePanel::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUI_CharacterCreatePanel::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_CharacterCreatePanel::CreateKneadFace()
{
	ScrollList->ClearChildren();

	if (UI_KneadFaceClass)
	{
		if (UUI_KneadFace* InUI_KneadFace = CreateWidget<UUI_KneadFace>(GetWorld(), UI_KneadFaceClass))
		{
			if (UScrollBoxSlot* InScrollBoxSlot = Cast<UScrollBoxSlot>(ScrollList->AddChild(InUI_KneadFace)))
			{
				InScrollBoxSlot->SetPadding(10.0f);
			}
		}
	}
}

void UUI_CharacterCreatePanel::CreateCharacterButtons()
{
	if (AHallPlayerState* InPlayerState = GetPlayerState<AHallPlayerState>())
	{
		InitCharacterButtons(InPlayerState->GetCharacterAppearance());
	}
}

void UUI_CharacterCreatePanel::InitCharacterButtons(FCharacterAppearacnce& InCAs)
{
	ScrollList->ClearChildren();

	if (UI_CharacterButtonClass)
	{
		for (int32 i = 0; i < 4; i++)
		{
			if (UUI_CharacterButton* InCharacterButton = CreateWidget<UUI_CharacterButton>(GetWorld(), UI_CharacterButtonClass))
			{
				InCharacterButton->SetSlotPosition(i);
				InCharacterButton->SetParents(this);
				if (UScrollBoxSlot* InScrollBoxSlot = Cast<UScrollBoxSlot>(ScrollList->AddChild(InCharacterButton)))
				{
					InScrollBoxSlot->SetPadding(10.0f);
				}

				if (const FMMORPGCharacterAppearance* InCharacterAppearance = 
					InCAs.FindByPredicate([&](const FMMORPGCharacterAppearance& InCA)
						{
							return InCA.SlotPosition == i;
						}))
				{
					InCharacterButton->InitCharacterButton(*InCharacterAppearance);
				}
			}
		}
	}
}
