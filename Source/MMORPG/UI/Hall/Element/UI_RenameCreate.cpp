// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_RenameCreate.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "../UI_HallMain.h"
#include "../../../Core/Hall/HallPawn.h"
#include "../../../Core/Hall/Character/CharacterStage.h"
#include "../../../Core/Hall/HallPlayerState.h"

void UUI_RenameCreate::NativeConstruct()
{
	Super::NativeConstruct();
	
	SlotPosition = INDEX_NONE;

	FindNameButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::ClickedFindName);
	CreateButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::ClickedCreate);
	CancelButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::ClickedCancel);
}

void UUI_RenameCreate::NativeDestruct()
{
	Super::NativeDestruct();

}

void UUI_RenameCreate::SetSlotPosition(const int32 InSlotPos)
{
	SlotPosition = InSlotPos;
}

void UUI_RenameCreate::ClickedCreate()
{
	if (UUI_HallMain* InHall = GetParents<UUI_HallMain>())
	{
		if (AHallPlayerState* InPlayerState = GetPlayerState<AHallPlayerState>())
		{
			FMMORPGCharacterAppearance InCA;
			InCA.Name = EditableName->GetText().ToString();
			InCA.Date = FDateTime::Now().ToString();
			InCA.Lv = 1;
			InCA.SlotPosition = SlotPosition;

			if (InCA.Name.IsEmpty())
			{
				InHall->PrintLog(TEXT("Name cannot be empty."));
			}
			else
			{
				InHall->CreateCharacter(InCA);
			}
		}
	}
}

void UUI_RenameCreate::ClickedCancel()
{
	if (UUI_HallMain* InHall = GetParents<UUI_HallMain>())
	{
		//删除刚刚的角色形象
		if (AHallPawn* InPawn = GetPawn<AHallPawn>())
		{
			if (InPawn->CharacterStage)
			{
				InPawn->CharacterStage->Destroy();
				InPawn->CharacterStage = nullptr;
			}
		}
		InHall->PlayRenameOut();
		InHall->ResetCharacterCreatePanel();
	}
}

void UUI_RenameCreate::ClickedFindName()
{
	if (UUI_HallMain* InHall = GetParents<UUI_HallMain>())
	{
		FString CharacterName = EditableName->GetText().ToString();
		InHall->CheckReName(CharacterName);
	}
}
