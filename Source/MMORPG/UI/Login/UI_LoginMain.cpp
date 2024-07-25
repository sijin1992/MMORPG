// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_LoginMain.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "UI_Login.h"
#include "Protocol/LoginProtocol.h"
#include "../../MMORPGMacroType.h"
#include "Kismet/GameplayStatics.h"

void UUI_LoginMain::NativeConstruct()
{
	Super::NativeConstruct();

	PlayWidgetAnim(TEXT("LoginIn"));
	
	UI_Login->SetParents(this);

	LinkServer();

	//读取账户并解密
	if (!UI_Login->DecryptionFromLocal(FPaths::ProjectDir() / TEXT("User")))
	{
		PrintLog(TEXT("No account detected."));
	}
}

void UUI_LoginMain::NativeDestruct()
{
	Super::NativeDestruct();

}

void UUI_LoginMain::SignIn(FString& InAccount, FString& InPassword)
{
	//通过宏发送协议
	SEND_DATA(SP_LoginRequests, InAccount, InPassword);
}

void UUI_LoginMain::Register()
{

}

void UUI_LoginMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber)
	{
	case SP_LoginResponses:
		FString String;
		FMMORPGGateStatus GateStatus;
		ELoginType Type = ELoginType::LOGIN_DB_SERVER_ERROR;
		//接收数据
		SIMPLE_PROTOCOLS_RECEIVE(SP_LoginResponses, Type, String, GateStatus);

		switch (Type)
		{
		case LOGIN_DB_SERVER_ERROR:
			PrintLog(TEXT("Server error."));
			break;
		case LOGIN_SUCCESS:
			//解析用户数据
			if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
			{
				NetDataAnalysis::StringToUserData(String, InGameInstance->GetUserData());

				if (GateStatus.GateConnetionNum == INDEX_NONE)//如果当前网关人数是-1，有可能服务器满了
				{
					//等待提示

					PrintLog(TEXT("At present, the number of servers is full, and we weill try to link again."));
				}
				else
				{
					InGameInstance->GetGateStatus() = GateStatus;

					if (!UI_Login->EncryptionToLocal(FPaths::ProjectDir() / TEXT("User")))
					{
						PrintLog(TEXT("Password storage failed."));
					}
					else
					{
						PrintLog(TEXT("Login Success."));
					}

					PlayWidgetAnim(TEXT("LoginOut"));

					///切换网关
					//先关闭登录服务器LoginServer
					if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetChannel())
					{
						InGameInstance->GetClient()->GetChannel()->DestroySelf();
					}
					//协程,2S后跳转关卡
					GThread::Get()->GetCoroutines().BindLambda(2.0f, [&]()
						{
							UGameplayStatics::OpenLevel(GetWorld(), TEXT("HallMap"));
						});
				}
			}
			break;
		case LOGIN_ACCOUNT_WRONG:
			PrintLog(TEXT("Account does not exist."));
			break;
		case LOGIN_WRONG_PASSWORD:
			PrintLog(TEXT("Password verification failed."));
			break;
		}
		
		break;
	}
}

void UUI_LoginMain::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{
	if (InType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS)
	{
		//如果握手成功,将UI塌陷
		UI_LinkWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
