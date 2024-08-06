// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_GameMain.h"

void UUI_GameMain::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUI_GameMain::NativeDestruct()
{
	Super::NativeDestruct();

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
	default:
		break;
	}
}
