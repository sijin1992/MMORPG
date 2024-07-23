// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CharacterModeling.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "../../../../Core/Hall/HallPlayerState.h"
#include "../../../../Core/Hall/HallPawn.h"
#include "../../../../Core/Hall/Character/CharacterStage.h"

//关闭优化
#if PLATFORM_WINDOWS
#pragma optimize("", off)
#endif

void UUI_CharacterModeling::NativeConstruct()
{
	Super::NativeConstruct();

	LegSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::LegValueChanged);
	WaistSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::WaistValueChanged);
	ArmSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::ArmValueChanged);
	HeadSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::HeadValueChanged);
	ChestSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::ChestValueChanged);

	ModelingType->OnSelectionChanged.AddDynamic(this, &UUI_CharacterModeling::SelectModelingType);
}

void UUI_CharacterModeling::NativeDestruct()
{
	Super::NativeDestruct();

}

void UUI_CharacterModeling::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UUI_CharacterModeling::InitKneadFace(const FMMORPGCharacterAppearance* InCAData)
{
	auto GetKFValue = [](float InValue)->float
		{
			return InValue / 10.f;
		};
	float LegValue = GetKFValue(InCAData->LegSize);
	float WaistValue = GetKFValue(InCAData->WaistSize);
	float ArmValue = GetKFValue(InCAData->ArmSize);
	float HeadValue = GetKFValue(InCAData->HeadSize);
	float ChestValue = GetKFValue(InCAData->ChestSize);
	LegSlider->SetValue(LegValue);
	WaistSlider->SetValue(WaistValue);
	ArmSlider->SetValue(ArmValue);
	HeadSlider->SetValue(HeadValue);
	ChestSlider->SetValue(ChestValue);
	UpdateText(LegValueText, LegValue);
	UpdateText(WaistValueText, WaistValue);
	UpdateText(ArmValueText, ArmValue);
	UpdateText(HeadValueText, HeadValue);
	UpdateText(ArmValueText, ChestValue);
}

void UUI_CharacterModeling::UpdatePawn()
{
	if (AHallPawn* InPawn = GetPawn<AHallPawn>())
	{
		if (AHallPlayerState* InPlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AHallPlayerState>())
		{
			if (InPawn->CharacterStage)
			{
				int32 SlotID = InPawn->CharacterStage->GetSlotID();
				FMMORPGCharacterAppearance* InCA = InPlayerState->GetCharacterCA(SlotID);
				if (!InCA)
				{
					if (auto* CurrentTmpCreateCharacter = InPlayerState->GetCurrentTmpCreateCharacter())
					{
						if (CurrentTmpCreateCharacter->SlotPosition == SlotID)
						{
							InCA = CurrentTmpCreateCharacter;
						}
					}
				}

				if (InCA)
				{
					InCA->LegSize = LegSlider->GetValue() * 10.f;
					InCA->WaistSize = WaistSlider->GetValue() * 10.f;
					InCA->ArmSize = ArmSlider->GetValue() * 10.f;
					InCA->HeadSize = HeadSlider->GetValue() * 10.f;
					InCA->ChestSize = ChestSlider->GetValue() * 10.f;
				}

				InPawn->CharacterStage->UpdateKneadingBody();
			}
		}
	}
}

void UUI_CharacterModeling::UpdateText(UTextBlock* InValueText, float IntValue)
{
	if (InValueText)
	{
		int32 newValue = IntValue * 10;
		InValueText->SetText(FText::FromString(FString::Printf(TEXT("%02d"), newValue)));
	}
}

void UUI_CharacterModeling::LegValueChanged(float InDeltaTime)
{
	//00 01 02 03 04 10
	UpdateText(LegValueText, InDeltaTime);
	UpdatePawn();
}

void UUI_CharacterModeling::WaistValueChanged(float InDeltaTime)
{
	UpdateText(WaistValueText, InDeltaTime);
	UpdatePawn();
}

void UUI_CharacterModeling::ArmValueChanged(float InDeltaTime)
{
	UpdateText(ArmValueText, InDeltaTime);
	UpdatePawn();
}

void UUI_CharacterModeling::HeadValueChanged(float InDeltaTime)
{
	UpdateText(HeadValueText, InDeltaTime);
	UpdatePawn();
}

void UUI_CharacterModeling::ChestValueChanged(float InDeltaTime)
{
	UpdateText(ChestValueText, InDeltaTime);
	UpdatePawn();
}

void UUI_CharacterModeling::SelectModelingType(FString SelectedItem, ESelectInfo::Type SelectionType)
{

}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif