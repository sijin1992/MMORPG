// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SimpleNetManage.h"
#include "MMORPGType.h"
#include "MMORPGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UMMORPGGameInstance : public UGameInstance, public FTickableGameObject
{
	GENERATED_BODY()

public:

	virtual void Init();

	virtual void Tick(float DeltaTime);//FTickableGameObject

	virtual TStatId GetStatId() const;

	virtual void Shutdown();//关闭客户端调用

public:

	void CreateClient();//创建客户端/Socket
	void LinkServer();//连接服务器
	void LinkServer(const FSimpleAddr& InAddr);//连接服务器
	void LinkServer(const TCHAR* InIP, uint32 InPort);

	FSimpleNetManage* GetClient();
	FMMORPGUserData& GetUserData();
	FMMORPGGateStatus& GetGateStatus();
private:
	FSimpleNetManage* Client;		//当前客户端实例
	FMMORPGUserData UserData;		//用户信息
	FMMORPGGateStatus GateStatus;	//网关状态信息
};
