// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGGameInstance.h"
#include "Global/SimpleNetGlobalInfo.h"
#include "ThreadManage.h"

//关闭优化
#if PLATFORM_WINDOWS
#pragma optimize("", off)
#endif

void UMMORPGGameInstance::Init()
{
	Super::Init();
}

void UMMORPGGameInstance::Tick(float DeltaTime)
{
	if (Client)
	{
		Client->Tick(DeltaTime);
	}
	GThread::Get()->Tick(DeltaTime);//协程插件需要手动Tick
}

TStatId UMMORPGGameInstance::GetStatId() const
{
	return TStatId();
}

void UMMORPGGameInstance::Shutdown()
{
	Super::Shutdown();

	if (Client)
	{
		FSimpleNetManage::Destroy(Client);
		GThread::Destroy();//销毁协程
	}
}

void UMMORPGGameInstance::CreateClient()
{
	if (!Client)
	{
		FSimpleNetGlobalInfo::Get()->Init();//初始化服务器全局配置
		Client = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);
	}
}

void UMMORPGGameInstance::LinkServer()
{
	//连接服务器
	if (Client)
	{
		if (!Client->Init())
		{
			delete Client;
			Client = nullptr;
		}
	}
}

void UMMORPGGameInstance::LinkServer(const FSimpleAddr& InAddr)
{
	//连接服务器
	if (Client)
	{
		if (!Client->Init(InAddr))
		{
			delete Client;
			Client = nullptr;
		}
	}
}

void UMMORPGGameInstance::LinkServer(const TCHAR* InIP, uint32 InPort)
{
	//连接服务器
	if (Client)
	{
		if (!Client->Init(InIP, InPort))
		{
			delete Client;
			Client = nullptr;
		}
	}
}

FSimpleNetManage* UMMORPGGameInstance::GetClient()
{
	return Client;
}

FMMORPGUserData& UMMORPGGameInstance::GetUserData()
{
	return UserData;
}

FMMORPGGateStatus& UMMORPGGameInstance::GetGateStatus()
{
	return GateStatus;
}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif
