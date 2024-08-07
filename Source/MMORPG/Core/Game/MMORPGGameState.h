// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "../../DataTable/CharacterAnimTable.h"
#include "../../DataTable/CharacterStyleTable.h"
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
protected:
	template<class T>
	TArray<T*>* GetTables(UDataTable* InTablePtr, TArray<T*>& OutTables, const FString& MsgTag = TEXT("MyTable"))
	{
		if (!OutTables.Num())
		{
			if (InTablePtr)
			{
				InTablePtr->GetAllRows(MsgTag, OutTables);
			}
		}

		return &OutTables;
	}

	template<class T>
	T* GetTable(int32 InTableID, UDataTable* InTablePtr, TArray<T*>& OutTables, const FString& MsgTag = TEXT("MyTable"))
	{
		if (TArray<T*>* InTables = GetTables<T>(InTablePtr, OutTables, MsgTag))
		{
			if (InTables->Num())
			{
				if (auto table = InTables->FindByPredicate([&](T* InTable)
					{
						return InTable->ID == InTableID;
					}))
				{
					return *table;
				}
			}
		}
		return NULL;
	}

public:
	//读取角色动画
	TArray<FCharacterAnimTable*>* GetCharacterAnimTables();
	FCharacterAnimTable* GetCharacterAnimTable(int32 InAnimTableID);
	//读取角色样式
	TArray<FCharacterStyleTable*>* GetCharacterStyleTables();
	FCharacterStyleTable* GetCharacterStyleTable(int32 InStyleTableID);

protected:
	UPROPERTY()
	UDataTable* CharacterAnimTablePtr;//角色动画表
	TArray<FCharacterAnimTable*> CharacterAnimTables;

	UPROPERTY()
	UDataTable* CharacterStyleTablePtr;//角色样式表
	TArray<FCharacterStyleTable*> CharacterStyleTables;
};
