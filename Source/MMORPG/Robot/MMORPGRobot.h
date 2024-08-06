
#pragma once

#include "CoreMinimal.h"
#include "SimpleNetManage.h"
#include "MMORPGType.h"
#include "MMORPGGateRobot.h"
#include "MMORPGDSRobot.h"

//机器人
class FMMORPGRobot
{
public:
	FMMORPGRobot();

	void InitGate(const FString& Host, const int32 Port);
	void InitDS(const FString& Host, const int32 Port);

	void Tick(float DeltaTime);

	void RunRobot();
private:
	FMMORPGGateRobot GateRobot;
	FMMORPGDSRobot DSRobot;

	FMMORPGUserData UserData;//用户表
};