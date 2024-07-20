// Fill out your copyright notice in the Description page of Project Settings.


#include "HallPlayerState.h"

FCharacterAppearacnce& AHallPlayerState::GetCharacterAppearance()
{
	return CharacterAppearances;
}

bool AHallPlayerState::IsCharacterExistInSlot(const int32 InPos)
{
	return GetCharacterCA(InPos) != nullptr;
}

FMMORPGCharacterAppearance* AHallPlayerState::GetRecentCharacter()
{
	FDateTime MaxDateTime;
	int32 Index = INDEX_NONE;
	for (int32 i = 0; i < CharacterAppearances.Num(); i++)
	{
		FDateTime DateTime;
		FDateTime::Parse(CharacterAppearances[i].Date, DateTime);
		if (DateTime > MaxDateTime)
		{
			MaxDateTime = DateTime;
			Index = i;
		}
	}

	if (Index == INDEX_NONE)
	{
		return nullptr;
	}

	return &CharacterAppearances[Index];
}

FMMORPGCharacterAppearance* AHallPlayerState::GetCharacterCA(const int32 InPos)
{
	return CharacterAppearances.FindByPredicate([InPos](const FMMORPGCharacterAppearance& InCA) {return InPos == InCA.SlotPosition; });
}

FMMORPGCharacterAppearance* AHallPlayerState::AddCharacterCA(const int32 InPos)
{
	FMMORPGCharacterAppearance* InCAInstance = nullptr;
	if (FMMORPGCharacterAppearance* InCA = GetCharacterCA(InPos))
	{
		InCAInstance = InCA;
	}
	else
	{
		CharacterAppearances.Add(FMMORPGCharacterAppearance());
		FMMORPGCharacterAppearance& InCARef = CharacterAppearances.Last();
		InCARef.SlotPosition = InPos;
		InCAInstance = &InCARef;
	}
	return InCAInstance;
}
