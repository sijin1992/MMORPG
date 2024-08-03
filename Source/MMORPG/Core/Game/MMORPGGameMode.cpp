// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMORPGGameMode.h"
#include "Character/MMORPGCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MMORPGHUD.h"
#include "../../MMORPGMacroType.h"
#include "ThreadManage.h"
#include "MMORPGGameState.h"
#include "MMORPGPlayerState.h"
#include "Character/MMORPGPlayerCharacter.h"

AMMORPGGameMode::AMMORPGGameMode()
{
	HUDClass = AMMORPGHUD::StaticClass();
	GameStateClass = AMMORPGGameState::StaticClass();
	PlayerStateClass = AMMORPGPlayerState::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMMORPGGameMode::BeginPlay()
{
	Super::BeginPlay();

	LinkServer();
}

void AMMORPGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//销毁代理
	if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
	{
		if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetController())
		{
			InGameInstance->GetClient()->GetController()->RecvDelegate.Remove(RecvDelegate);
		}
	}
}

void AMMORPGGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

//DS Server Timer
void AMMORPGGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	GThread::Get()->GetCoroutines().BindLambda(0.5f, [&](APlayerController* InNewPlayer)
		{
			if (InNewPlayer)
			{
				if (AMMORPGPlayerCharacter* InPlayerCharacter = InNewPlayer->GetPawn<AMMORPGPlayerCharacter>())
				{
					if (AMMORPGGameState* InGameState = GetGameState<AMMORPGGameState>())
					{
						if (FCharacterAnimTable* InAnimTable = InGameState->GetCharacterAnimTable(InPlayerCharacter->GetID()))
						{
							InPlayerCharacter->AnimTable = InAnimTable;
						}
					}
				}
			}
		}, NewPlayer);
	
}

void AMMORPGGameMode::BindClientRcv()
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

void AMMORPGGameMode::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{

}

void AMMORPGGameMode::LinkServer()
{
	//创建客户端
	if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
	{
		InGameInstance->CreateClient();
		if (InGameInstance->GetClient())
		{
			//握手时绑定消息代理
			InGameInstance->GetClient()->NetManageMsgDelegate.BindUObject(this, &AMMORPGGameMode::LinkServerInfo);
			//这里作为测试，正式项目用InGameInstance->LinkServer();
			InGameInstance->LinkServer(TEXT("127.0.0.1"),11231);
			//开始循环绑定代理
			BindClientRcv();
		}
	}
}

void AMMORPGGameMode::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{

}
