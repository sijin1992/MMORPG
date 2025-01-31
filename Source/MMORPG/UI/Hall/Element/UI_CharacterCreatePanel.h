// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "MMORPGType.h"
#include "Components/ScrollBox.h"
#include "UI_CharacterCreatePanel.generated.h"

class UTextBlock;
class UScrollBox;
class UUI_CharacterButton;
class UUI_KneadFace;
class ACharacterStage;
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
	TSubclassOf<UUI_CharacterButton> UI_CharacterButtonClass;	//创角Button类

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_KneadFace> UI_KneadFaceClass;				//捏脸UI类

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<ACharacterStage> CharacterStageClass;			//角色舞台类

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FVector SpawnPoint;											//角色生成点
public:
	void NativeConstruct() override;

	void NativeDestruct() override;
	//创建捏脸界面
	UUI_KneadFace* CreateKneadFace();
	//创建角色界面的按钮
	void CreateCharacterButtons();

	void InitCharacterButtons(FCharacterAppearacnce& InCAs);

	ACharacterStage* CreateCharacter();						//创建新的角色舞台实例
	void SpawnCharacter(const FMMORPGCharacterAppearance* InCAData);	//生成角色舞台实例
	void SpawnCharacter(const int32 InSlotPos);					
	void SpawnCharacter();

	void SetCurrentSlotPosition(const int32 InNewPos);

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="Func"></param>
	/// <param name="bool">是否继续循环</param>
	template<class T>
	T* FindByPredicateInScrollList(TFunction<bool(T*)> Func) 
	{
		for (auto& Temp : ScrollList->GetAllChildren())
		{
			if (T* InButton = Cast<T>(Temp))
			{
				if (Func(InButton))
				{
					return InButton;
				}
			}
		}
		return NULL;
	}

	void HighlightSelection(int32 InSlotPos);

	UUI_CharacterButton* GetHighlightButton();

	void JumpDSServer(int32 InSlotID);

protected:
	int32 SlotPosition;											//记录当前选择的角色插槽下标
};
