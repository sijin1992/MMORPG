// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_RenameCreate.generated.h"

class UButton;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class MMORPG_API UUI_RenameCreate : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UButton* FindNameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CreateButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* EditableName;
public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void ClickedCreate();

	UFUNCTION()
	void ClickedCancel();

	UFUNCTION()
	void ClickedFindName();
};
