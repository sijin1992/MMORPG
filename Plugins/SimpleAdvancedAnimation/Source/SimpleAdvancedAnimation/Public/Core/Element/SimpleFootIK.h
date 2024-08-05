#pragma once

#include "CoreMinimal.h"
#include "SimpleAdvancedAnimType.h"

struct FSimpleFootIK
{
public:
	FSimpleFootIK();

	void Init(ACharacter* InCharacter, const TArray<FName>& InBoneNames, float InTraceStart = 50.0f, float InTraceDistance = 50.0f, float InInterpSpeed = 18.0f);

	void Tick(float DeltaTime);

	float FindOffset(const FName& InKeyName);

	FFootIKInfo* FindFootIKInfo(const FName& InKeyName);

	bool IsPendingKill() { return bPendingKill; }
protected:
	/// <summary>
	/// 射线检测
	/// </summary>
	/// <param name="BoneName">骨骼名字</param>
	/// <param name="TraceDistance">检测距离</param>
	/// <returns></returns>
	float FootTrace(const FName& BoneName, float InTraceDistance);

protected:
	ACharacter* Character;

	TMap<FName, FFootIKInfo> IKInfos;
	float TraceStart;//射线初始位置偏移
	float TraceDistance;//射线预定检测距离(从胶囊体底部开始)
	float InterpSpeed;//初始渐变速度
	bool bPendingKill;
};