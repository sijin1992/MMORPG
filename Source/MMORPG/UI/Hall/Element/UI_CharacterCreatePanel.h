// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "MMORPGType.h"
#include "UI_CharacterCreatePanel.generated.h"

class UTextBlock;
class UScrollBox;
class UUI_CharacterButton;
class UUI_KneadFace;
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

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_KneadFace> UI_KneadFaceClass;
public:
	void NativeConstruct() override;

	void NativeDestruct() override;
	//创建捏脸界面
	void CreateKneadFace();
	//创建角色界面的按钮
	void CreateCharacterButtons();

	void InitCharacterButtons(FCharacterAppearacnce& InCAs);
};
