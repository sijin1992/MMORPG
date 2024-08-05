#include "Core/Element/SimpleFootIK.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

FSimpleFootIK::FSimpleFootIK()
	:Character(NULL),
	TraceStart(50.0f),
	TraceDistance(50.0f),
	InterpSpeed(18.0f),
	bPendingKill(false)
{

}

void FSimpleFootIK::Init(ACharacter* InCharacter, const TArray<FName>& InBoneNames, float InTraceStart /*= 50.0f*/, float InTraceDistance /*= 50.0f*/, float InInterpSpeed /*= 18.0f*/)
{
	Character = InCharacter;
	TraceStart = InTraceStart;
	TraceDistance = InTraceDistance;
	InterpSpeed = InInterpSpeed;

	for (auto& Temp : InBoneNames)
	{
		IKInfos.Add(Temp, FFootIKInfo());
	}
}

void FSimpleFootIK::Tick(float DeltaTime)
{
	for (auto& Temp : IKInfos)
	{
		float OffsetTarget = FootTrace(Temp.Key, TraceDistance);
		Temp.Value.Offset = FMath::FInterpTo(Temp.Value.Offset, OffsetTarget, DeltaTime, InterpSpeed);
	}
}

float FSimpleFootIK::FindOffset(const FName& InKeyName)
{
	if (FFootIKInfo* Info = FindFootIKInfo(InKeyName))
	{
		return Info->Offset;
	}
	return 0.0f;
}

FFootIKInfo* FSimpleFootIK::FindFootIKInfo(const FName& InKeyName)
{
	return IKInfos.Find(InKeyName);
}

float FSimpleFootIK::FootTrace(const FName& BoneName, float InTraceDistance)
{
	if (Character)
	{
		//判断是否要被销毁了
		if (Character->IsPendingKill())
		{
			bPendingKill = true;
			Character = NULL;
			return 0.0f;
		}
		//射线的起始坐标
		FVector StartLocation = Character->GetMesh()->GetSocketLocation(BoneName) + FVector(0.0f,0.0f, TraceStart);
		//终点
		float CharacterLocationZ = Character->GetActorLocation().Z;
		float HalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		FVector EndLocation = FVector(StartLocation.X, StartLocation.Y, CharacterLocationZ - (HalfHeight + InTraceDistance));

		//射线
		FHitResult HitResult;
		TArray<AActor*> Ignores;
		//EDrawDebugTrace::Type::ForOneFrame显示射线
		if (UKismetSystemLibrary::LineTraceSingle(
			Character->GetWorld(),
			StartLocation,
			EndLocation,
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			Ignores,
			EDrawDebugTrace::Type::None,
			HitResult,
			true))
		{
			//(射线与地面的交点 - 射线发射时预定的终点)-减去胶囊体底部与预定射线长度的差
			return (HitResult.Location - HitResult.TraceEnd).Size() - InTraceDistance;
		}
	}
	return 0.0f;
}
