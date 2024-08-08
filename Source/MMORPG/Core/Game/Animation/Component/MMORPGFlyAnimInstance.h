// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Instance/Core/MMORPGAnimInstanceBase.h"
#include "MMORPGFlyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UMMORPGFlyAnimInstance : public UMMORPGAnimInstanceBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FlyAnimAttrubute")
	FVector FlySpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FlyAnimAttrubute")
	FVector2D RotationRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FlyAnimAttrubute")
	bool bFastFly;

public:

	virtual void InitAnimInstance(ACharacter* InCharacter) override;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
