// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MMORPGHUD.generated.h"

class UUI_GameMain;
/**
 * 
 */
UCLASS()
class MMORPG_API AMMORPGHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<UUI_GameMain> GameMainClass;

public:
	AMMORPGHUD();

	virtual void BeginPlay() override;

private:
	UUI_GameMain* GameMain;
	
};
