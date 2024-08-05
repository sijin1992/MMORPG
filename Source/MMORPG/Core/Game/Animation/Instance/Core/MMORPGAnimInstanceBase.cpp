// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGAnimInstanceBase.h"
#include "../../../Character/Core/MMORPGCharacterBase.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SimpleAdvancedAnimationBPLibrary.h"

UMMORPGAnimInstanceBase::UMMORPGAnimInstanceBase()
	:bDeath(false),
	Speed(0.0f),
	bInAir(false),
	bFight(false),
	bFootIK(false),
	LeftBoneName(TEXT("foot_l")),
	RightBoneName(TEXT("foot_r")),
	FootIKID(INDEX_NONE)
{

}

void UMMORPGAnimInstanceBase::InitAnimInstance(ACharacter* InCharacter)
{
	if (bFootIK)
	{
		BoneNames.Add(LeftBoneName);
		BoneNames.Add(RightBoneName);
		//调用FootIK功能
		FootIKID = USimpleAdvancedAnimationBPLibrary::CreateFootIK(InCharacter, BoneNames);
	}
}

void UMMORPGAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UMMORPGAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AMMORPGCharacterBase* InCharacterBase = Cast<AMMORPGCharacterBase>(TryGetPawnOwner()))
	{
		Speed = InCharacterBase->GetVelocity().Size();
		bInAir = InCharacterBase->GetMovementComponent()->IsFalling();
		bFight = InCharacterBase->IsFight();
	}

	if (bFootIK && FootIKID != INDEX_NONE)
	{
		TArray<float> OffsetArray;
		//暂时只考虑双腿情况
		float LOffset = GetFootIKOffset(LeftBoneName);
		float ROffset = GetFootIKOffset(RightBoneName);

		OffsetArray.Add(LOffset);
		OffsetArray.Add(ROffset);

		//算质心偏移
		ButtZOffset = USimpleAdvancedAnimationBPLibrary::GetButtZOffset(OffsetArray);
		//算左右脚偏移
		LeftOffset = -(ButtZOffset - LOffset);
		RightOffset = ButtZOffset - ROffset;
	}

}

float UMMORPGAnimInstanceBase::GetFootIKOffset(const FName& InBoneName)
{
	if (FootIKID != INDEX_NONE)
	{
		return USimpleAdvancedAnimationBPLibrary::FindOffset(FootIKID, InBoneName);
	}
	return 0.0f;
}
