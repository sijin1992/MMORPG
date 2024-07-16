// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "SimpleNetChannelType.h"
#include "MMORPGType.h"
#include "UI_LoginMain.generated.h"

class FSimpleChannel;
class UUI_Login;
class UUI_Print;
/**
 * 
 */
UCLASS()
class MMORPG_API UUI_LoginMain : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UUserWidget* UI_LinkWidget;			//显示连接服务器状态信息

	UPROPERTY(meta = (BindWidget))
	UUI_Login* UI_Login;				//登录界面

	UPROPERTY(meta = (BindWidget))
	UUI_Print* UI_Print;				//Log界面

public:
	//构造
	virtual void NativeConstruct();
	//析构
	virtual void NativeDestruct();

public:
	void SignIn(FString& InAccount, FString& InPassword);

	void Register();

	void PrintLog(const FString& InMsg);
	void PrintLog(const FText& InMsg);

protected:
	/// <summary>
	/// 循环绑定:做循环调用
	/// </summary>
	void BindClientRcv();

	//通信协议的代理函数
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel) override;


	/// <summary>
	/// 连接服务器信息
	/// </summary>
	/// <param name="InType">连接错误状态类型</param>
	/// <param name="InMsg"></param>
	UFUNCTION()
	void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg);



private:
	//接收代理
	FDelegateHandle RecvDelegate;
	//网关状态信息
	FMMORPGGateStatus GateStatus;
};
