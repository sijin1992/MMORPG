#include "MMORPGGateRobot.h"
#include "Protocol/HallProtocol.h"
#include "UObject/SimpleController.h"

FMMORPGGateRobot::FMMORPGGateRobot()
	:GateClient(NULL)
{

}

FMMORPGGateRobot::~FMMORPGGateRobot()
{
	if (GateClient)
	{
		FSimpleNetManage::Destroy(GateClient);
		GateClient = NULL;
	}
}

void FMMORPGGateRobot::Init(const FString& Host, const int32 Port)
{
	FSimpleNetGlobalInfo::Get()->Init();//初始化服务器全局配置

	GateClient = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);

	GateClient->NetManageMsgDelegate.BindRaw(this, &FMMORPGGateRobot::LinkServerInfo);//绑定原生

	if (!GateClient->Init(Host,Port))
	{
		delete GateClient;
		GateClient = NULL;
	}
	
	GateClient->GetController()->RecvDelegate.AddLambda([&](uint32 ProtocolNumber, FSimpleChannel* Channel) 
		{
			RecvProtocol(ProtocolNumber, Channel);
		});
}

void FMMORPGGateRobot::Tick(float DeltaTime)
{
	if (GateClient)
	{
		GateClient->Tick(DeltaTime);
	}
}

void FMMORPGGateRobot::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{
	if (InType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS)//如果握手成功
	{
		RunCharacterAppearanceRequests();
	}
}

void FMMORPGGateRobot::RunCharacterAppearanceRequests()
{
	int32 ID = 1;
	SIMPLE_CLIENT_SEND(GateClient, SP_CharacterAppearanceRequests, ID);
}

void FMMORPGGateRobot::RunLoginToDSServerRequests()
{
	int32 ID = 1;
	int32 InSlotID = 0;
	SIMPLE_CLIENT_SEND(GateClient, SP_LoginToDSServerRequests, ID, InSlotID);
}

void FMMORPGGateRobot::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber)
	{
		case SP_CharacterAppearanceResponses:
		{
			//接收角色数据
			FString CharacterJson;
			SIMPLE_PROTOCOLS_RECEIVE(SP_CharacterAppearanceResponses, CharacterJson);
			if (!CharacterJson.IsEmpty())
			{
				RunLoginToDSServerRequests();
			}
			break;
		}
		case SP_LoginToDSServerResponses:
		{
			//收到登录DSServer回调
			FSimpleAddr DsAddr;//DSServerAddrInfo

			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginToDSServerResponses, DsAddr);

			FString DSAddrString = FSimpleNetManage::GetAddrString(DsAddr);

			StartDelegate.ExecuteIfBound();
			break;
		}
	}
}
