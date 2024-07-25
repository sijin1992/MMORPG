// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_MainBase.h"
#include "SimpleNetChannelType.h"
#include "MMORPGType.h"
#include "UI_LoginMain.generated.h"

class FSimpleChannel;
class UUI_Login;
/**
 * 
 */
UCLASS()
class MMORPG_API UUI_LoginMain : public UUI_MainBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UUserWidget* UI_LinkWidget;			//显示连接服务器状态信息

	UPROPERTY(meta = (BindWidget))
	UUI_Login* UI_Login;				//登录界面

public:
	//构造
	virtual void NativeConstruct();
	//析构
	virtual void NativeDestruct();

public:
	void SignIn(FString& InAccount, FString& InPassword);

	void Register();

protected:
	//通信协议的代理函数
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel) override;

	/// <summary>
	/// 连接服务器信息
	/// </summary>
	/// <param name="InType">连接错误状态类型</param>
	/// <param name="InMsg"></param>
	virtual void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg);

};
