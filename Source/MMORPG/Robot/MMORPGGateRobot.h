
#pragma once

#include "CoreMinimal.h"
#include "SimpleNetManage.h"

//模拟网关
class FMMORPGGateRobot
{
public:
	FMMORPGGateRobot();
	~FMMORPGGateRobot();

	void RunRobot();

	void Init(const FString& Host, const int32 Port);

	virtual void Tick(float DeltaTime);

	//通信协议的代理函数
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);

	/// <summary>
	/// 连接服务器信息
	/// </summary>
	/// <param name="InType">连接错误状态类型</param>
	/// <param name="InMsg"></param>
	virtual void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg);
private:
	FSimpleNetManage* GateClient;
};