// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_MainBase.h"
#include "../../MMORPGMacroType.h"
#if UE_MMORPG_DEBUG_DS
#include "../../Robot/MMORPGRobot.h"
#endif
#include "UI_GameMain.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UUI_GameMain : public UUI_MainBase
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:

	virtual void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg) override;

	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel) override;

protected:
#if UE_MMORPG_DEBUG_DS
	FMMORPGRobot Robot;//只是为了测试
#endif
};
