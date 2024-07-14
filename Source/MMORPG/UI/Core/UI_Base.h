// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Base.generated.h"

class FSimpleChannel;
/**
 * 
 */
UCLASS()
class MMORPG_API UUI_Base : public UUserWidget
{
	GENERATED_BODY()

protected:
	//获取PlayerController的模板函数
	template<class T>
	T* GetPlayerController()
	{
		return GetWorld() != nullptr ? GetWorld()->GetFirstPlayerController<T>() : nullptr;
	}

	//获取Pawn的模板函数
	template<class T>
	T* GetPawn()
	{
		return GetWorld() != nullptr ? 
			(GetWorld()->GetFirstPlayerController() != nullptr ? GetWorld()->GetFirstPlayerController()->GetPawn<T>() : nullptr)
			: nullptr;
	}

	//获取GameInstance的模板函数
	template<class T>
	T* GetGameInstance()
	{
		return GetWorld() != nullptr ? GetWorld()->GetGameInstance<T>() : nullptr;
	}

public:
	void SetParents(UWidget* InWidget) { ParentWidget = InWidget; }

	//获取父级组件的模板函数
	template<class T>
	T* GetParents()
	{
		return Cast<T>(ParentWidget);
	}


protected:
	/// <summary>
	/// 与服务器通信的协议函数
	/// </summary>
	/// <param name="ProtocolNumber">协议号</param>
	/// <param name="Channel">通道</param>
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel) {}

protected:
	UPROPERTY()
	UWidget* ParentWidget;
};
