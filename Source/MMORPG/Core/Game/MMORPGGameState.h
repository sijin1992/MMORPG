// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "../../DataTable/CharacterAnimTable.h"
#include "MMORPGGameState.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API AMMORPGGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMMORPGGameState();

	FCharacterAnimTable* GetCharacterAnimTable(int32 InAnimTableID);
	TArray<FCharacterAnimTable*>* GetCharacterAnimTables();
protected:
	UPROPERTY()
	UDataTable* CharacterAnimTablePtr;//角色动画表

	TArray<FCharacterAnimTable*> CharacterAnimTables;
};
