// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/AnimNotifyState_AddForce.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimNotifyState_AddForce::UAnimNotifyState_AddForce()
	:Super(),
	DirectionForce(1.0f,0.0f,0.0f),
	ForceSize(1800000.0f),
	TotalDurationConsuming(0.0f),
	ForceSizeConsuming(0.0f)
{

}

void UAnimNotifyState_AddForce::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	TotalDurationConsuming = TotalDuration;
	ForceSizeConsuming = ForceSize;
}

void UAnimNotifyState_AddForce::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (TotalDurationConsuming > 0.0f)
	{
		if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter()))
		{
			FVector NewDirection = GetCurrentCharacterDirection(InCharacter);

			InCharacter->GetCharacterMovement()->AddForce(ForceSizeConsuming * NewDirection);

			if (ForceSizeConsuming > 0.0f)
			{
				float PreSecondFrame = 1.0f / FrameDeltaTime;//每秒帧数
				float TotalFrame = TotalDurationConsuming * PreSecondFrame;//持续时间内的总帧数
				float ForceAttenuationFrameDeltaTime = ForceSize / TotalFrame;//每帧应该减去多少力
				//对力进行衰减
				ForceSizeConsuming -= ForceAttenuationFrameDeltaTime;
			}
		}
	}
}

void UAnimNotifyState_AddForce::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}

FVector UAnimNotifyState_AddForce::GetCurrentCharacterDirection(ACharacter* InCharacter)
{
	if (InCharacter)
	{
		if (DirectionForce.X != 0.0f)
		{
			return InCharacter->GetActorForwardVector() * DirectionForce.X;
		}
		else if (DirectionForce.Y != 0.0f)
		{
			return InCharacter->GetActorRightVector() * DirectionForce.Y;
		}
		else if (DirectionForce.Z != 0.0f)
		{
			return InCharacter->GetActorUpVector() * DirectionForce.Z;
		}
	}

	return FVector::ZeroVector;
}
