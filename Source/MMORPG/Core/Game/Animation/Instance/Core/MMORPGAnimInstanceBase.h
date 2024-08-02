// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MMORPGAnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UMMORPGAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMMORPGAnimInstanceBase();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	bool bDeath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	bool bInAir;
};
