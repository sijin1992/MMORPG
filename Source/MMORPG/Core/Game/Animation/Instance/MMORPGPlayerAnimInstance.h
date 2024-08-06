// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMORPGCharacterAnimInstance.h"
#include "MMORPGPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UMMORPGPlayerAnimInstance : public UMMORPGCharacterAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	float LegSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	float WaistSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	float ArmSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	float HeadSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	float ChestSize;

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
