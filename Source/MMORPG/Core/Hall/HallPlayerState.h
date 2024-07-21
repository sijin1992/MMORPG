// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MMORPGType.h"
#include "HallPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API AHallPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	FCharacterAppearacnce& GetCharacterAppearance();

	bool IsCharacterExistInSlot(const int32 InPos);						//判断插槽里的角色是否存在

	FMMORPGCharacterAppearance* GetRecentCharacter();					//获取用户最近使用的角色数据

	FMMORPGCharacterAppearance* GetCharacterCA(const int32 InPos);		//根据插槽下标获取角色数据

	FMMORPGCharacterAppearance* AddCharacterCA(const int32 InPos);		//根据插槽下标增加角色数据

	int32 AddCharacterCA(const FMMORPGCharacterAppearance& InCA);		//增加角色数据

	FMMORPGCharacterAppearance* GetCurrentTmpCreateCharacter() { return &CurrentTmpCreateCharacter; }		//获取客户端临时角色数据
private:
	FCharacterAppearacnce CharacterAppearances;
	FMMORPGCharacterAppearance CurrentTmpCreateCharacter;				//客户端临时角色数据
};
