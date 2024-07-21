// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../Core/UI_Base.h"
#include "UI_CharacterAppearance.generated.h"

class UComboBoxString;
class UWrapBox;
/**
 * 
 */
UCLASS()
class MMORPG_API UUI_CharacterAppearance : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* AppearanceType;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* SkinWrap;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* ClothWrap;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* LegWrap;

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void SelectAppearanceType(FString SelectedItem, ESelectInfo::Type SelectionType);
};
