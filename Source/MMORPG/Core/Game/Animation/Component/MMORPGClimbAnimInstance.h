// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/AnimInstanceComponentBase.h"
#include "MMORPGClimbAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UMMORPGClimbAnimInstance : public UAnimInstanceComponentBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
