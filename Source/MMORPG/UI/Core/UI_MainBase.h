// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Base.h"
#include "SimpleNetChannelType.h"
#include "UI_MainBase.generated.h"

class UUI_Print;
/**
 * 
 */
UCLASS()
class MMORPG_API UUI_MainBase : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UUI_Print* UI_Print;

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

public:
	void PrintLog(const FString& InMsg);
	void PrintLog(const FText& InMsg);

protected:
	//循环绑定
	void BindClientRcv();

	/// <summary>
	/// 连接服务器信息
	/// </summary>
	/// <param name="InType">连接错误状态类型</param>
	/// <param name="InMsg"></param>
	UFUNCTION()
	virtual void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg);

	void LinkServer();
	void LinkServer(const FSimpleAddr& InAddr);

private:
	//接收代理
	FDelegateHandle RecvDelegate;
};
