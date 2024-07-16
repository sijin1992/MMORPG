// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "UI_Print.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MMORPG_API UUI_Print : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MsgLog;					//Log

public:

	void PlayTextAnim();
	void SetText(const FText& InText);
};
