// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MMORPGPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API AMMORPGPlayerState : public APlayerState
{
	GENERATED_BODY()
	
private:
	FMMORPGCharacterAppearance CA;
public:
	FMMORPGCharacterAppearance& GetCA() { return CA; };
};
