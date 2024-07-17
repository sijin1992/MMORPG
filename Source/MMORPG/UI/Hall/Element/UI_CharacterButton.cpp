// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CharacterButton.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "../../../Core/Hall/Character/CharacterStage.h"

void UUI_CharacterButton::NativeConstruct()
{
	Super::NativeConstruct();
	
	CharacterButton->OnReleased.AddDynamic(this, &UUI_CharacterButton::ClickedCharacter);

	CreateShowIcon->SetVisibility(ESlateVisibility::Visible);
	CharacterInfo->SetVisibility(ESlateVisibility::Collapsed);
}

void UUI_CharacterButton::NativeDestruct()
{
	Super::NativeDestruct();

}

void UUI_CharacterButton::ClickedCharacter()
{
	if (CharacterStageClass)
	{
		if (ACharacterStage* InCharacterStage = GetWorld()->SpawnActor<ACharacterStage>(CharacterStageClass, SpawnPoint, FRotator::ZeroRotator))
		{

		}
	}
}
