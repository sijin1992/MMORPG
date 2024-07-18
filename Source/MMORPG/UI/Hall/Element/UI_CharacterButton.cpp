// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CharacterButton.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "../../../Core/Hall/Character/CharacterStage.h"
#include "../../../Core/Hall/HallPawn.h"
#include "UI_CharacterCreatePanel.h"
#include "../UI_HallMain.h"

void UUI_CharacterButton::NativeConstruct()
{
	Super::NativeConstruct();

	SlotPosition = INDEX_NONE;
	
	CharacterButton->OnReleased.AddDynamic(this, &UUI_CharacterButton::ClickedCharacter);

	CreateShowIcon->SetVisibility(ESlateVisibility::Visible);
	CharacterInfo->SetVisibility(ESlateVisibility::Collapsed);
}

void UUI_CharacterButton::NativeDestruct()
{
	Super::NativeDestruct();

}

void UUI_CharacterButton::SetSlotPosition(const int32 InNewPos)
{
	SlotPosition = InNewPos;
}

void UUI_CharacterButton::InitCharacterButton(const FMMORPGCharacterAppearance& InCA)
{
	if (InCA.SlotPosition != INDEX_NONE)
	{
		CreateShowIcon->SetVisibility(ESlateVisibility::Collapsed);
		CharacterInfo->SetVisibility(ESlateVisibility::Visible);
		CharacterName->SetText(FText::FromString(InCA.Name));
		Date->SetText(FText::FromString(InCA.Date));
	}
}

void UUI_CharacterButton::ClickedCharacter()
{
	if (1)
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

		if (UUI_CharacterCreatePanel* UI_CharacterCreatePanel = GetParents<UUI_CharacterCreatePanel>())
		{
			UI_CharacterCreatePanel->CreateKneadFace();

			if (UUI_HallMain* UI_HallMain = UI_CharacterCreatePanel->GetParents<UUI_HallMain>())
			{
				UI_HallMain->PlayRenameIn();
			}
		}
	}
}
