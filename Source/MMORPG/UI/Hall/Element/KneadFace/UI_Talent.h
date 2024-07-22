// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_KneadFaceBase.h"
#include "UI_Talent.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UUI_Talent : public UUI_KneadFaceBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void InitKneadFace(const FMMORPGCharacterAppearance* InCAData) override;
};
