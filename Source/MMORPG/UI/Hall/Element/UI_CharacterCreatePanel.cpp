// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CharacterCreatePanel.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "UI_CharacterButton.h"
#include "Components/ScrollBoxSlot.h"
#include "UI_KneadFace.h"
#include "../../../Core/Hall/HallPlayerState.h"
#include "../../../Core/Hall/Character/CharacterStage.h"
#include "../../../Core/Hall/HallPawn.h"

void UUI_CharacterCreatePanel::NativeConstruct()
{
	Super::NativeConstruct();

	SlotPosition = 0;
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
				if (UScrollBoxSlot* InScrollBoxSlot = Cast<UScrollBoxSlot>(ScrollList->AddChild(InCharacterButton)))
				{
					InScrollBoxSlot->SetPadding(10.0f);
				}

				InCharacterButton->SetSlotPosition(i);
				InCharacterButton->SetParents(this);

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

void UUI_CharacterCreatePanel::SpawnCharacter(const FMMORPGCharacterAppearance* InCAData)
{
	if (InCAData)
	{
		if (CharacterStageClass)
		{
			if (AHallPawn* InPawn = GetPawn<AHallPawn>())
			{
				if (InPawn->CharacterStage)
				{
					InPawn->CharacterStage->Destroy();
				}

				InPawn->CharacterStage = GetWorld()->SpawnActor<ACharacterStage>(CharacterStageClass, SpawnPoint, FRotator::ZeroRotator);
				if (InPawn->CharacterStage)
				{

				}
			}
		}
	}
}

void UUI_CharacterCreatePanel::SpawnCharacter(const int32 InSlotPos)
{
	if (AHallPlayerState* InPlayerState = GetPlayerState<AHallPlayerState>())
	{
		SpawnCharacter(InPlayerState->GetCharacterAppearance().FindByPredicate([&](const FMMORPGCharacterAppearance& InCA)
			{
				return InCA.SlotPosition == InSlotPos;
			}));
	}
}

void UUI_CharacterCreatePanel::SpawnCharacter()
{
	SpawnCharacter(SlotPosition);
}

void UUI_CharacterCreatePanel::SetCurrentSlotPosition(const int32 InNewPos)
{
	SlotPosition = InNewPos;
}
