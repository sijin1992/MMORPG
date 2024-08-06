
#pragma once

#include "CoreMinimal.h"
#include "SimpleNetManage.h"

//模拟DS服务器
class FMMORPGDSRobot
{
public:
	FMMORPGDSRobot();
	void Init(const FString& Host, const int32 Port);
	virtual void Tick(float DeltaTime);
private:
	FSimpleNetManage* DSClient;
};