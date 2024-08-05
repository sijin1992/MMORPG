#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "Element/SimpleFootIK.h"
#include "SimpleAdvancedAnimType.h"

//单例
class FSimpleAdvancedAnimSystem : public FTickableGameObject
{
public:
	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

	static FSimpleAdvancedAnimSystem* Get();

	static void Destroy();
public:
	FSAAHandle CreateFootIK(ACharacter* InCharacter, const TArray<FName>& InBoneNames, float InTraceStart = 50.0f,float InTraceDistance = 50.0f, float InInterpSpeed = 18.0f);

	FSimpleFootIK* FindFootIK(const FSAAHandle InKeyHandle);

	float FindOffset(const FSAAHandle InKeyHandle, const FName& InKeyName);

	FFootIKInfo* FindFootIKInfo(const FSAAHandle InKeyHandle, const FName& InKeyName);
protected:
	TMap<FSAAHandle, FSimpleFootIK> FootIKs;
	static FSimpleAdvancedAnimSystem* AdvancedAnimSystem;
};