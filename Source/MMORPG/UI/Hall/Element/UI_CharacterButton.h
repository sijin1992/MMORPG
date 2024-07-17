// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
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
public:
	void NativeConstruct() override;

	void NativeDestruct() override;

protected:
	UFUNCTION()
	void ClickedCharacter();

};
