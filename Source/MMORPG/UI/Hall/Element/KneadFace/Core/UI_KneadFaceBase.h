// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../../Core/UI_Base.h"
#include "UI_KneadFaceBase.generated.h"

struct FMMORPGCharacterAppearance;
/**
 * 
 */
UCLASS()
class MMORPG_API UUI_KneadFaceBase : public UUI_Base
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

	virtual void InitKneadFace(const FMMORPGCharacterAppearance* InCAData){}
};
