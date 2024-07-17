// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "SimpleNetChannelType.h"
#include "UI_HallMain.generated.h"

class FSimpleChannel;
class UUI_Print;
class UUI_CharacterCreatePanel;
class UUI_RenameCreate;
/**
 * 
 */
UCLASS()
class MMORPG_API UUI_HallMain : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UUI_Print* UI_Print;									//Log界面

	UPROPERTY(meta = (BindWidget))
	UUI_CharacterCreatePanel* UI_CharacterCreatePanel;		//创建角色界面

	UPROPERTY(meta = (BindWidget))
	UUI_RenameCreate* UI_RenameCreate;						//命名界面

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

public:
	void PrintLog(const FString& InMsg);
	void PrintLog(const FText& InMsg);

	void PlayRenameIn();								//播放角色命名界面渐入动画
	void PlayRenameOut();								//播放角色命名界面渐出动画

	void ResetCharacterCreatePanel();					//还原创角界面

protected:
	//循环绑定
	void BindClientRcv();

	//通信协议的代理函数
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel) override;

private:
	//接收代理
	FDelegateHandle RecvDelegate;
};
