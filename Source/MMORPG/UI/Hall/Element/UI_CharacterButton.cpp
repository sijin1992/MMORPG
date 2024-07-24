// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CharacterButton.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "UI_CharacterCreatePanel.h"
#include "../UI_HallMain.h"
#include "../../../Core/Hall/HallPlayerState.h"
#include "../../../Core/Hall/Character/CharacterStage.h"

void UUI_CharacterButton::NativeConstruct()
{
	Super::NativeConstruct();

	SlotPosition = INDEX_NONE;
	
	CharacterButton->OnReleased.AddDynamic(this, &UUI_CharacterButton::ClickedCharacter);

	CreateShowIcon->SetVisibility(ESlateVisibility::Visible);
	CharacterInfo->SetVisibility(ESlateVisibility::Collapsed);

	DefaultColor = CharacterButton->WidgetStyle.Normal.TintColor.GetSpecifiedColor();
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

void UUI_CharacterButton::SetHighlight(bool bHigh)
{
	if (bHigh)
	{
		CharacterButton->WidgetStyle.Normal.TintColor = HighlightColor;
		CharacterButton->SetStyle(CharacterButton->WidgetStyle);
	}
	else 
	{
		CharacterButton->WidgetStyle.Normal.TintColor = DefaultColor;
		CharacterButton->SetStyle(CharacterButton->WidgetStyle);
	}
}

bool UUI_CharacterButton::IsHighlight()
{
	return CharacterButton->WidgetStyle.Normal.TintColor == HighlightColor;
}

void UUI_CharacterButton::ClickedCharacter()
{
	if (AHallPlayerState* InPlayerState = GetPlayerState<AHallPlayerState>())
	{
		if (UUI_CharacterCreatePanel* UI_CharacterCreatePanel = GetParents<UUI_CharacterCreatePanel>())
		{
			//创建角色对象
			if (!InPlayerState->IsCharacterExistInSlot(SlotPosition))//如果当前插槽没有角色,就创建新的角色
			{
				if (ACharacterStage* InCharacterStage = UI_CharacterCreatePanel->CreateCharacter())
				{
					if (InPlayerState->GetCurrentTmpCreateCharacter())
					{
						InPlayerState->GetCurrentTmpCreateCharacter()->Reset();
						InPlayerState->GetCurrentTmpCreateCharacter()->SlotPosition = SlotPosition;
					}
					InCharacterStage->SetSlotID(SlotPosition);
					InCharacterStage->UpdateKneadingBody();
				}

				UI_CharacterCreatePanel->CreateKneadFace();

				if (UUI_HallMain* UI_HallMain = UI_CharacterCreatePanel->GetParents<UUI_HallMain>())
				{
					UI_HallMain->PlayRenameIn();
					UI_HallMain->SetSlotPosition(SlotPosition);

					UI_HallMain->ResetCreateType();
					UI_HallMain->SetEditorCharacterPanelEnable(false);
				}
			}
			else if(UI_CharacterCreatePanel->GetHighlightButton() == this)
			{
				//如果当前插槽已经有角色了,且是高亮状态,就请求连接DS服务器
				JumpDSServer();
			}
			else
			{
				//如果当前插槽已经有角色了，但不是高亮按钮，就将高亮切换成自己
				UI_CharacterCreatePanel->GetHighlightButton()->SetHighlight(false);
				SetHighlight(true);
				//替换角色舞台对象
				if (FMMORPGCharacterAppearance* InCA = InPlayerState->GetCharacterCA(SlotPosition))
				{
					UI_CharacterCreatePanel->SpawnCharacter(InCA);

					if (UUI_HallMain* UI_HallMain = UI_CharacterCreatePanel->GetParents<UUI_HallMain>())
					{
						UI_HallMain->SetEditCharacter(InCA);
					}
				}
			}

			UI_CharacterCreatePanel->SetCurrentSlotPosition(SlotPosition);
		}
	}
}

void UUI_CharacterButton::JumpDSServer()
{
	if (UUI_CharacterCreatePanel* InCharacterCreatePanel = GetParents<UUI_CharacterCreatePanel>())
	{
		InCharacterCreatePanel->JumpDSServer(SlotPosition);
	}
}
