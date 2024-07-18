// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "MMORPGType.h"
#include "UI_CharacterButton.generated.h"

class UButton;
class UImage;
class UVerticalBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class MMORPG_API UUI_CharacterButton : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UButton* CharacterButton;

	UPROPERTY(meta = (BindWidget))
	UImage* CreateShowIcon;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* CharacterInfo;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Date;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FLinearColor HighlightColor;
public:
	void NativeConstruct() override;

	void NativeDestruct() override;

	FORCEINLINE int32 GetSlotPosition() { return SlotPosition; }

	void SetSlotPosition(const int32 InNewPos);

	void InitCharacterButton(const FMMORPGCharacterAppearance& InCA);

	void SetHighlight(bool bHigh);
protected:
	UFUNCTION()
	void ClickedCharacter();

protected:
	int32 SlotPosition;									//角色插槽
	FLinearColor DefaultColor;							//按钮默认的Color
};
