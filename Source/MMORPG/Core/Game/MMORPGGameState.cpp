// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGGameState.h"
#include "Engine/DataTable.h"

AMMORPGGameState::AMMORPGGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterAnimTable(TEXT("/Game/DataTable/CharacterAnimTable"));
	CharacterAnimTablePtr = CharacterAnimTable.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterStyleTable(TEXT("/Game/DataTable/CharacterStyleTable"));
	CharacterStyleTablePtr = CharacterStyleTable.Object;
}

TArray<FCharacterAnimTable*>* AMMORPGGameState::GetCharacterAnimTables()
{
	return GetTables(CharacterAnimTablePtr, CharacterAnimTables, TEXT("AnimTable"));
}

FCharacterAnimTable* AMMORPGGameState::GetCharacterAnimTable(int32 InAnimTableID)
{
	return GetTable(InAnimTableID, CharacterAnimTablePtr, CharacterAnimTables, TEXT("AnimTable"));
}

TArray<FCharacterStyleTable*>* AMMORPGGameState::GetCharacterStyleTables()
{
	return GetTables(CharacterStyleTablePtr, CharacterStyleTables, TEXT("StyleTable"));
}

FCharacterStyleTable* AMMORPGGameState::GetCharacterStyleTable(int32 InStyleTableID)
{
	return GetTable(InStyleTableID, CharacterStyleTablePtr, CharacterStyleTables, TEXT("StyleTable"));
}
