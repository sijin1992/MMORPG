// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_GameMain.h"
#include "ThreadManage.h"
#include "../../Core/Game/Character/MMORPGPlayerCharacter.h"

void UUI_GameMain::NativeConstruct()
{
	Super::NativeConstruct();

#if UE_MMORPG_DEBUG_DS
	if (AMMORPGPlayerCharacter* InPlayer = GetPawn<AMMORPGPlayerCharacter>())
	{
		Robot.GetGateRobot().StartDelegate.BindUObject(InPlayer, &AMMORPGPlayerCharacter::UpdateKneadingRequest);
	}
	
	Robot.InitGate("127.0.0.1", 11222);
#endif
}

void UUI_GameMain::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_GameMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

#if UE_MMORPG_DEBUG_DS
	Robot.Tick(InDeltaTime);
#endif
}

void UUI_GameMain::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{
	if (InType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS)
	{
	}
}

void UUI_GameMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber)
	{
	case 101:
		break;
	}
}
