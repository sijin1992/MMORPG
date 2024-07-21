// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CharacterAppearance.h"
#include "Components/ComboBoxString.h"
#include "Components/WrapBox.h"

void UUI_CharacterAppearance::NativeConstruct()
{
	Super::NativeConstruct();


	AppearanceType->OnSelectionChanged.AddDynamic(this, &UUI_CharacterAppearance::SelectAppearanceType);
}

void UUI_CharacterAppearance::NativeDestruct()
{
	Super::NativeDestruct();

}

void UUI_CharacterAppearance::SelectAppearanceType(FString SelectedItem, ESelectInfo::Type SelectionType)
{

}

