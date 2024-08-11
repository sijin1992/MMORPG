// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Instance/Core/MMORPGAnimInstanceBase.h"
#include "AnimInstanceComponentBase.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UAnimInstanceComponentBase : public UMMORPGAnimInstanceBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FlyAnimAttrubute")
	FVector AxisSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FlyAnimAttrubute")
	FVector2D RotationRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FlyAnimAttrubute")
	bool bFast;//是否加速
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
