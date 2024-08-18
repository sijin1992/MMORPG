// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "ClimbComponent.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UClimbComponent : public UMotionComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ClimbAttrubute")
	EClimbState ClimbState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ClimbAttrubute")
	bool bJumpToClimb;//是否跳起来触发攀爬

	FResetBool bJump;//攀爬中是否跳跃

public:
	UClimbComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ClimbForwardAxis(float InAxisValue);

	void ClimbRightAxis(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Climbing")
	void PhysClimbing(float deltaTime, int32 Iterations);//在蓝图中进行调用

	void ResetJump();

	void SetClimbing();

	void ReleaseClimeb();

private:
	void TraceClimbingState(float DelaTime);//射线检测

	void SetClimbState(EMovementMode InMovementMode, ECharacterActionState InCharacterActionState, bool bOrientRotationToMovement);
};
