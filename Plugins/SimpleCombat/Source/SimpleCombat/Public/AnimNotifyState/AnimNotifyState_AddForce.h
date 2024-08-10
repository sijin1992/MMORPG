// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AddForce.generated.h"

class ACharacter;
/**
 * 
 */
UCLASS()
class SIMPLECOMBAT_API UAnimNotifyState_AddForce : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_AddForce();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotifyState_AddForce")
	FVector DirectionForce;//力的方向,基于世界

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotifyState_AddForce")
	float ForceSize;//力的大小

private:
	float TotalDurationConsuming;//施加力的持续时间,在动画力设置

	float ForceSizeConsuming;//力的消耗

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	FVector GetCurrentCharacterDirection(ACharacter* InCharacter);//获取当前基于角色的所施加的力的方向
};
