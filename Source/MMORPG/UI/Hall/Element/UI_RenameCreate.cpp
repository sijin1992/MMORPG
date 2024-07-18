// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_RenameCreate.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "../UI_HallMain.h"
#include "../../../Core/Hall/HallPawn.h"
#include "../../../Core/Hall/Character/CharacterStage.h"

void UUI_RenameCreate::NativeConstruct()
{
	Super::NativeConstruct();
	
	FindNameButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::ClickedFindName);
	CreateButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::ClickedCreate);
	CancelButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::ClickedCancel);
}

void UUI_RenameCreate::NativeDestruct()
{
	Super::NativeDestruct();

}

void UUI_RenameCreate::ClickedCreate()
{
	if (UUI_HallMain* InHall = GetParents<UUI_HallMain>())
	{
		InHall->PlayRenameOut();
		InHall->ResetCharacterCreatePanel();
	}
}

void UUI_RenameCreate::ClickedCancel()
{
	if (UUI_HallMain* InHall = GetParents<UUI_HallMain>())
	{
		InHall->PlayRenameOut();
		InHall->ResetCharacterCreatePanel();
	}
}

void UUI_RenameCreate::ClickedFindName()
{

}
