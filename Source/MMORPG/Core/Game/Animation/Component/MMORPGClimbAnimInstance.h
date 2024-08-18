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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ClimbAnimAttrubute")
	EClimbState ClimbState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ClimbAnimAttrubute")
	bool bJumpToClimb;//是否跳起来触发攀爬

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ClimbAnimAttrubute")
	bool bJump;//攀爬中是否跳
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void ClimbJump();//攀爬跳跃

	EClimbMontageState CalculationClimbJumpState();//计算攀爬跳跃的方向
};
