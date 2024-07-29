// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MainBase.h"
#include "../Common/UI_Print.h"
#include "../../MMORPGMacroType.h"
#include "ThreadManage.h"

void UUI_MainBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_MainBase::NativeDestruct()
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

void UUI_MainBase::LinkServer()
{
	//创建客户端
	if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
	{
		InGameInstance->CreateClient();
		if (InGameInstance->GetClient())
		{
			//握手时绑定消息代理
			InGameInstance->GetClient()->NetManageMsgDelegate.BindUObject(this, &UUI_MainBase::LinkServerInfo);

			InGameInstance->LinkServer();
			//开始循环绑定代理
			BindClientRcv();
		}
	}
}

void UUI_MainBase::LinkServer(const FSimpleAddr& InAddr)
{
	//创建客户端
	if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
	{
		if (InGameInstance->GetClient())
		{
			//握手时绑定消息代理
			InGameInstance->GetClient()->NetManageMsgDelegate.BindUObject(this, &UUI_MainBase::LinkServerInfo);

			InGameInstance->LinkServer(InAddr);
			//开始循环绑定代理
			BindClientRcv();
		}
	}
}

void UUI_MainBase::PrintLog(const FString& InMsg)
{
	PrintLog(FText::FromString(InMsg));
}

void UUI_MainBase::PrintLog(const FText& InMsg)
{
	//播放Log动画
	UI_Print->PlayTextAnim();

	UI_Print->SetText(InMsg);
}

void UUI_MainBase::BindClientRcv()
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

void UUI_MainBase::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{

}