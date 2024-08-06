// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGPlayerCharacter.h"
#include "../../Common/MMORPGGameInstance.h"
#include "../MMORPGGameMode.h"

void AMMORPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitKneadingLocation(GetMesh()->GetComponentLocation());

	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy)//服务器
	{
		if (UMMORPGGameInstance* InGameInstance = GetWorld()->GetGameInstance<UMMORPGGameInstance>())
		{
			//RPC调用
			//请求捏脸数据
			CallServerUpdateKneading(InGameInstance->GetUserData().ID);
		}
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