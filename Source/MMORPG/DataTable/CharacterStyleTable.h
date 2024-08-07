#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStyleTable.generated.h"

class AMMORPGCharacterBase;

USTRUCT(BlueprintType)
struct FCharacterStyleTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FCharacterStyleTable();

	UPROPERTY(EditDefaultsOnly, Category = "CharacterStyle")
	int32 ID;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterStyle")
	TSubclassOf<AMMORPGCharacterBase> MMORPGCharacterClass;
};