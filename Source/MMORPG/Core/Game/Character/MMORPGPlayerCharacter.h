// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMORPGCharacter.h"
#include "../../Common/Interface/KneadingInterface.h"
#include "MMORPGPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API AMMORPGPlayerCharacter : public AMMORPGCharacter, public IKneadingInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void UpdateKneadingBody() override;
	virtual void UpdateKneadingBody(const FMMORPGCharacterAppearance& InCA) override;
};
