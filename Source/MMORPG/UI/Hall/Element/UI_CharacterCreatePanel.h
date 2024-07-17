// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_CharacterCreatePanel.generated.h"

class UTextBlock;
class UScrollBox;
class UUI_CharacterButton;
/**
 * 
 */
UCLASS()
class MMORPG_API UUI_CharacterCreatePanel : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PanelName;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollList;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_CharacterButton> UI_CharacterButtonClass;
public:
	void NativeConstruct() override;

	void NativeDestruct() override;

	void InitCharacterButton(const int32 InNumber);
};
