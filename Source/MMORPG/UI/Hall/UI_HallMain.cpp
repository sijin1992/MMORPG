// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_HallMain.h"
#include "../Common/UI_Print.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "../../MMORPGMacroType.h"
#include "Kismet/GameplayStatics.h"
#include "Element/UI_CharacterCreatePanel.h"

void UUI_HallMain::NativeConstruct()
{
	Super::NativeConstruct();
	
	//创建客户端
	if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
	{
		if (InGameInstance->GetClient())
		{
			//通过网关信息初始化
			InGameInstance->GetClient()->Init(InGameInstance->GetGateStatus().GateServerAddrInfo.Addr);
			//开始循环绑定代理
			BindClientRcv();
		}
	}
}

void UUI_HallMain::NativeDestruct()
{
	Super::NativeDestruct();

	//销毁代理
	if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
	{
		if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetController())
		{
			InGameInstance->GetClient()->GetController()->RecvDelegate.Remove(RecvDelegate);
		}
	}
}

void UUI_HallMain::PrintLog(const FString& InMsg)
{
	PrintLog(FText::FromString(InMsg));
}

void UUI_HallMain::PrintLog(const FText& InMsg)
{
	//播放Log动画
	UI_Print->PlayTextAnim();

	UI_Print->SetText(InMsg);
}

void UUI_HallMain::BindClientRcv()
{
	if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
	{
		if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetController())
		{
			//绑定代理
			RecvDelegate = InGameInstance->GetClient()->GetController()->RecvDelegate.AddLambda([&](uint32 ProtocolNumber, FSimpleChannel* Channel)
				{
					this->RecvProtocol(ProtocolNumber, Channel);
				});
		}
		else
		{
			//如果没有获取到Client，就通过协程等待片刻后再次绑定
			GThread::Get()->GetCoroutines().BindLambda(0.5f, [&]()
				{
					BindClientRcv();
				});
		}
	}
	else
	{
		//如果没有获取到GameInstance，就通过协程等待片刻后再次绑定
		GThread::Get()->GetCoroutines().BindLambda(0.5f, [&]()
			{
				BindClientRcv();
			});
	}
}

void UUI_HallMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	
}