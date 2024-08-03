// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGGameState.h"
#include "Engine/DataTable.h"

AMMORPGGameState::AMMORPGGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterAnimTable(TEXT("/Game/DataTable/CharacterAnimTable"));
	CharacterAnimTablePtr = CharacterAnimTable.Object;
}

FCharacterAnimTable* AMMORPGGameState::GetCharacterAnimTable(int32 InAnimTableID)
{
	if (TArray<FCharacterAnimTable*>* InAnimTables = GetCharacterAnimTables())
	{
		if (InAnimTables->Num())
		{
			if (auto Anim = InAnimTables->FindByPredicate([&](FCharacterAnimTable* InAnimTable)
				{
					return InAnimTable->ID == InAnimTableID;
				}))
			{
				return *Anim;
			}
		}
	}
	return NULL;
}

TArray<FCharacterAnimTable*>* AMMORPGGameState::GetCharacterAnimTables()
{
	if (!CharacterAnimTables.Num())
	{
		if (CharacterAnimTablePtr)
		{
			CharacterAnimTablePtr->GetAllRows(TEXT("AnimTable"), CharacterAnimTables);
		}
	}

	return &CharacterAnimTables;
}
