// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleAdvancedAnimationBPLibrary.h"
#include "SimpleAdvancedAnimation.h"
#include "Core/SimpleAdvancedAnimSystem.h"

USimpleAdvancedAnimationBPLibrary::USimpleAdvancedAnimationBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

int32 USimpleAdvancedAnimationBPLibrary::CreateFootIK(ACharacter* InCharacter, const TArray<FName>& InBoneNames, float InTraceStart /*= 50.0f*/, float InTraceDistance /*= 50.0f*/, float InInterpSpeed /*= 18.0f*/)
{
	return FSimpleAdvancedAnimSystem::Get()->CreateFootIK(InCharacter, InBoneNames, InTraceStart, InTraceDistance, InInterpSpeed);
}

float USimpleAdvancedAnimationBPLibrary::FindOffset(const int32 InKeyHandle, const FName& InKeyName)
{
	return FSimpleAdvancedAnimSystem::Get()->FindOffset(InKeyHandle, InKeyName);
}

FFootIKInfo USimpleAdvancedAnimationBPLibrary::FindFootIKInfo(const int32 InKeyHandle, const FName& InKeyName)
{
	if (FFootIKInfo* InFootIKInfo = FSimpleAdvancedAnimSystem::Get()->FindFootIKInfo(InKeyHandle,InKeyName))
	{
		return *InFootIKInfo;
	}
	return FFootIKInfo();
}

float USimpleAdvancedAnimationBPLibrary::GetButtZOffset(const TArray<float>& InOffsets)
{
	float TempValue = 0.0f;
	for (auto& Temp:InOffsets)
	{
		TempValue = FMath::Min(TempValue, Temp);
	}
	return TempValue < 0.0f ? TempValue : 0.0f;
}

void USimpleAdvancedAnimationBPLibrary::Destroy()
{
	FSimpleAdvancedAnimSystem::Destroy();
}
