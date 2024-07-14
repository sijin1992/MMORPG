// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoginHUD.generated.h"

class UUI_LoginMain;
/**
 * 
 */
UCLASS()
class MMORPG_API ALoginHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<UUI_LoginMain> LoginMainClass;//用于创建蓝图实例的类信息
public:
	ALoginHUD();

	virtual void BeginPlay() override;
private:
	UUI_LoginMain* LoginMain;//UI蓝图实例
};
