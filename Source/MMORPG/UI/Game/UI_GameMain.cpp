// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_GameMain.h"
#include "ThreadManage.h"

void UUI_GameMain::NativeConstruct()
{
	Super::NativeConstruct();

	Robot.InitGate("127.0.0.1", 11222);

	GThread::Get()->GetCoroutines().BindLambda(0.5f, [=]()
		{
			Robot.RunRobot();
		});
}

void UUI_GameMain::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_GameMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	Robot.Tick(InDeltaTime);
}

void UUI_GameMain::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{
	if (InType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS)
	{
	}
}

void UUI_GameMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber)
	{
	case 101:
		break;
	}
}
