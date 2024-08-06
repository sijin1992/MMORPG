#include "MMORPGGateRobot.h"
#include "Protocol/HallProtocol.h"

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

void FMMORPGGateRobot::RunRobot()
{
	int32 ID = 1;
	int32 InSlotID = 1;
	SIMPLE_CLIENT_SEND(GateClient, SP_LoginToDSServerRequests, ID, InSlotID);
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
}

void FMMORPGGateRobot::Tick(float DeltaTime)
{
	if (GateClient)
	{
		GateClient->Tick(DeltaTime);
	}
}

void FMMORPGGateRobot::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber)
	{
		case SP_LoginToDSServerResponses:
		{
			//收到登录DSServer回调10
			FSimpleAddr DsAddr;//DSServerAddrInfo

			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginToDSServerResponses, DsAddr);

			FString DSAddrString = FSimpleNetManage::GetAddrString(DsAddr);

			break;
		}
	}
}

void FMMORPGGateRobot::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{
	if (InType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS)//如果握手成功
	{
		
	}
}
