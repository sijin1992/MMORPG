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

	FResetBool bThrowOver;//是否翻越

private:
	FVector ThrowOverPoint;//翻越点
	float ThrowOverHeight;//翻越高度

	FVector PendingLaunchVelocity;//施加力

public:
	UClimbComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ClimbForwardAxis(float InAxisValue);

	void ClimbRightAxis(float InAxisValue);

	//自定义的物理系统
	UFUNCTION(BlueprintCallable, Category = "Climbing")
	void PhysClimbing(float deltaTime, int32 Iterations);//在蓝图中进行调用

	void ResetJump();

	void SetClimbing();

	void ReleaseClimeb();

	void ClearClimbingState();

	bool IsLowThrowOver();//判断是高低翻越(单手翻越还是双手翻越)

	void AdjustmentClimbing(bool bStart = true);//翻越到墙顶后调整角色的高度，防止脚部下陷

	void LaunchCharacter(const FVector& LaunchVelocity);//自定义施力
private:
	void TraceClimbingState(float DeltaTime);//射线检测

	void SetClimbState(EMovementMode InMovementMode, ECharacterActionState InCharacterActionState, bool bOrientRotationToMovement);

	void AdjustPendingLaunchVelocity(float DeltaTime);//调节力的函数
};
