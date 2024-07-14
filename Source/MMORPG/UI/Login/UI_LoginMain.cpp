// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_LoginMain.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "../../MMORPGGameInstance.h"
#include "UI_Login.h"
#include "Components/TextBlock.h"

void UUI_LoginMain::NativeConstruct()
{
	Super::NativeConstruct();
	
	UI_Login->SetParents(this);

	//创建客户端
	if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
	{
		InGameInstance->CreateClient();
		if (InGameInstance->GetClient())
		{
			InGameInstance->GetClient()->NetManageMsgDelegate.BindUObject(this, &UUI_LoginMain::LinkServerInfo);//握手时绑定消息代理

			//连接服务器
			InGameInstance->LinkServer();

			//开始循环绑定代理
			BindClientRcv();
		}
	}
}

void UUI_LoginMain::NativeDestruct()
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

void UUI_LoginMain::SignIn(const FString& InAccount, const FString& InPassword)
{

}

void UUI_LoginMain::Register()
{

}

void UUI_LoginMain::PrintLog(const FString& InMsg)
{
	//播放动画

	MsgLog->SetText(FText::FromString(InMsg));
}

void UUI_LoginMain::BindClientRcv()
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

void UUI_LoginMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	
}

void UUI_LoginMain::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{
	if (InType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS)
	{
		//如果握手成功,将UI塌陷
		UI_LinkWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
