// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "MMORPGType.h"
#include "UI_KneadFace.generated.h"

class UCheckBox;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class MMORPG_API UUI_KneadFace : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UCheckBox* Body;
	
	UPROPERTY(meta = (BindWidget))
	UCheckBox* Appearance;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* Talent;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* FacePanel;

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	void InitKneadFace(const FMMORPGCharacterAppearance* InCAData);

private:
	UFUNCTION()
	void OnClickedBody(bool bIsChecked);

	UFUNCTION()
	void OnClickedAppearance(bool bIsChecked);

	UFUNCTION()
	void OnClickedTalent(bool bIsChecked);
};
