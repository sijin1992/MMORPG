// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGGameInstance.h"
#include "Global/SimpleNetGlobalInfo.h"

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
	}
}

void UMMORPGGameInstance::CreateClient()
{
	FSimpleNetGlobalInfo::Get()->Init();//初始化服务器全局配置
	Client = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);
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

FSimpleNetManage* UMMORPGGameInstance::GetClient()
{
	return Client;
}
