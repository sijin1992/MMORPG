// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SimpleNetChannelType.h"
#include "MMORPGType.h"
#include "MMORPGGameMode.generated.h"

class FSimpleChannel;
UCLASS(minimalapi)
class AMMORPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMMORPGGameMode();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

public:

	void LoginCharacterUpdateKneadingRequest(int32 InUserID);

protected:
	//玩家登录后会激活这个接口
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	//循环绑定
	void BindClientRcv();

	/// <summary>
	/// 连接服务器信息
	/// </summary>
	/// <param name="InType">连接错误状态类型</param>
	/// <param name="InMsg"></param>
	UFUNCTION()
	void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg);
	void LinkServer();

	//通信协议的代理函数
	void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);

private:
	//接收代理
	FDelegateHandle RecvDelegate;
};



