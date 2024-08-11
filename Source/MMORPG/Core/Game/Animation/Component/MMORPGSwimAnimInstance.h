// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/AnimInstanceComponentBase.h"
#include "MMORPGSwimAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UMMORPGSwimAnimInstance : public UAnimInstanceComponentBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SwimAnimAttrubute")
	bool bDiving;//是否潜泳
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
