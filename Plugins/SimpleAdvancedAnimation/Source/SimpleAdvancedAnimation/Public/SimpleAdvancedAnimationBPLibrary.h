// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleAdvancedAnimType.h"
#include "SimpleAdvancedAnimationBPLibrary.generated.h"

UCLASS()
class SIMPLEADVANCEDANIMATION_API USimpleAdvancedAnimationBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "SimpleAdvancedAnimation|FootIK")
	static int32 CreateFootIK(ACharacter* InCharacter, const TArray<FName>& InBoneNames, float InTraceStart = 50.0f, float InTraceDistance = 50.0f, float InInterpSpeed = 18.0f);

	UFUNCTION(BlueprintCallable, Category = "SimpleAdvancedAnimation|FootIK")
	static float FindOffset(const int32 InKeyHandle, const FName& InKeyName);

	UFUNCTION(BlueprintCallable, Category = "SimpleAdvancedAnimation|FootIK")
	static FFootIKInfo FindFootIKInfo(const int32 InKeyHandle, const FName& InKeyName);

	/// <summary>
	/// 寻找最小的质心
	/// </summary>
	/// <param name="InOffsets"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, Category = "SimpleAdvancedAnimation|FootIK")
	static float GetButtZOffset(const TArray<float>& InOffsets);

	UFUNCTION(BlueprintCallable, Category = "SimpleAdvancedAnimation|FootIK")
	static void Destroy();
};
