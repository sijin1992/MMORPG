#include "Core/SimpleAdvancedAnimSystem.h"

//因为是静态的,所以要声明
FSimpleAdvancedAnimSystem* FSimpleAdvancedAnimSystem::AdvancedAnimSystem = NULL;

FSimpleAdvancedAnimSystem* FSimpleAdvancedAnimSystem::Get()
{
	if (!AdvancedAnimSystem)
	{
		AdvancedAnimSystem = new FSimpleAdvancedAnimSystem();
	}

	return AdvancedAnimSystem;
}

void FSimpleAdvancedAnimSystem::Destroy()
{
	if (AdvancedAnimSystem)
	{
		delete AdvancedAnimSystem;
		AdvancedAnimSystem = NULL;
	}
}

FSAAHandle FSimpleAdvancedAnimSystem::CreateFootIK(ACharacter* InCharacter, const TArray<FName>& InBoneNames, float InTraceStart /*= 50.0f*/, float InTraceDistance /*= 50.0f*/, float InInterpSpeed /*= 18.0f*/)
{
	//随机一个数字
	FSAAHandle Handle = FMath::RandRange(0, 999999);
	if (!FootIKs.Contains(Handle))
	{
		FSimpleFootIK& InFootIK = FootIKs.Add(Handle, FSimpleFootIK());
		InFootIK.Init(InCharacter, InBoneNames, InTraceStart, InTraceDistance, InInterpSpeed);
		return Handle;
	}

	return CreateFootIK(InCharacter, InBoneNames, InTraceStart, InTraceDistance, InInterpSpeed);
}

FSimpleFootIK* FSimpleAdvancedAnimSystem::FindFootIK(const FSAAHandle InKeyHandle)
{
	return FootIKs.Find(InKeyHandle);
}

float FSimpleAdvancedAnimSystem::FindOffset(const FSAAHandle InKeyHandle, const FName& InKeyName)
{
	if (FFootIKInfo* InInfo = FindFootIKInfo(InKeyHandle,InKeyName))
	{
		return InInfo->Offset;
	}
	return 0.0f;
}

FFootIKInfo* FSimpleAdvancedAnimSystem::FindFootIKInfo(const FSAAHandle InKeyHandle, const FName& InKeyName)
{
	if (FSimpleFootIK* InFootIK = FindFootIK(InKeyHandle))
	{
		return InFootIK->FindFootIKInfo(InKeyName);
	}
	return NULL;
}

TStatId FSimpleAdvancedAnimSystem::GetStatId() const
{
	return TStatId();
}

void FSimpleAdvancedAnimSystem::Tick(float DeltaTime)
{
	for (auto& Temp : FootIKs)
	{
		Temp.Value.Tick(DeltaTime);
	}

	//移除操作
	TArray<FSAAHandle> RemoveHandle;
	for (auto& Temp : FootIKs)
	{
		if (Temp.Value.IsPendingKill())
		{
			RemoveHandle.Add(Temp.Key);
		}
	}

	for (auto& Temp : RemoveHandle)
	{
		FootIKs.Remove(Temp);
	}
}