// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGPlayerCharacter.h"
#include "../../Common/MMORPGGameInstance.h"
#include "../MMORPGGameMode.h"
#include "ThreadManage.h"
#include "../../../MMORPGMacroType.h"

void AMMORPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitKneadingLocation(GetMesh()->GetComponentLocation());

	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy)//服务器
	{
#if !UE_MMORPG_DEBUG_DS
		UpdateKneadingRequest();
#endif
	}
	else if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy)//模拟玩家
	{

	}
}
//客户端运行
void AMMORPGPlayerCharacter::CallUpdateKneadingBodyOnClient_Implementation(const FMMORPGCharacterAppearance& InCA)
{
	UpdateKneadingBody(InCA);
}

void AMMORPGPlayerCharacter::UpdateKneadingRequest()
{
	if (UMMORPGGameInstance* InGameInstance = GetWorld()->GetGameInstance<UMMORPGGameInstance>())
	{
#if UE_MMORPG_DEBUG_DS
		//测试代码
		CallServerUpdateKneading(1);
#else
		//正式打包代码
		//RPC调用
		//请求捏脸数据
		CallServerUpdateKneading(InGameInstance->GetUserData().ID);
#endif
	}
}

//DS服务器上运行
void AMMORPGPlayerCharacter::CallServerUpdateKneading_Implementation(int32 InUserID)
{
	if (AMMORPGGameMode* InGameMode = GetWorld()->GetAuthGameMode<AMMORPGGameMode>())
	{
		UserID = InUserID;
		InGameMode->LoginCharacterUpdateKneadingRequest(InUserID);
	}
}

void AMMORPGPlayerCharacter::UpdateKneadingBody()
{
}

void AMMORPGPlayerCharacter::UpdateKneadingBody(const FMMORPGCharacterAppearance& InCA)
{
	SetLegSize(InCA.LegSize);
	SetWaistSize(InCA.WaistSize);
	SetArmSize(InCA.ArmSize);
	SetHeadSize(InCA.HeadSize);
	SetChestSize(InCA.ChestSize);

	//需要保证Mesh和骨骼一致
	SetMeshPosition(GetMesh());
}