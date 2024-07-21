// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../Core/UI_Base.h"
#include "UI_Talent.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UUI_Talent : public UUI_Base
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

};
