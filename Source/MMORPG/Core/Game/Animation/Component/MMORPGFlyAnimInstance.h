// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/AnimInstanceComponentBase.h"
#include "MMORPGFlyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UMMORPGFlyAnimInstance : public UAnimInstanceComponentBase
{
	GENERATED_BODY()
	
public:
	UMMORPGFlyAnimInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FlyAnimAttrubute")
	EDodgeFly DodgeFly;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FlyAnimAttrubute")
	bool bLand;//是否着地

public:

	virtual void InitAnimInstance(ACharacter* InCharacter) override;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
