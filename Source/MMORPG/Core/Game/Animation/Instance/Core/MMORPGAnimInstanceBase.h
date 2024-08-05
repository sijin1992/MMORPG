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

	virtual void InitAnimInstance(ACharacter* InCharacter);

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	bool bDeath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	bool bInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	bool bFight;

	///FootIK相关
	//本项目两条腿使用
	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
	FName LeftBoneName;//左脚骨骼节点名
	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
	FName RightBoneName;//右脚骨骼节点名
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
	float LeftOffset;//左脚偏移
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
	float RightOffset;//右脚偏移
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
	float ButtZOffset;//质心
	//通用
	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
	bool bFootIK;//开关
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
	int32 FootIKID;
	//多条腿
	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
	TArray<FName> BoneNames;
public:
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetFootIKOffset(const FName& InBoneName);
};
