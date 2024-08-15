// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceComponentBase.h"
#include "../../../Character/Core/MMORPGCharacterBase.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../../Component/FlyComponent.h"

void UAnimInstanceComponentBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAnimInstanceComponentBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UAnimInstanceComponentBase::Print(float InTime, const FString& InString)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, InTime, FColor::Red, FString::Printf(TEXT("%s"), *InString));
	}
}

void UAnimInstanceComponentBase::ResetAxisSpeed(float InNewSpeed)
{
	if (AMMORPGCharacterBase* InCharacterBase = Cast<AMMORPGCharacterBase>(TryGetPawnOwner()))
	{
		FVector SpeedVector = InCharacterBase->GetVelocity();
		//处理旋转
		FRotator CharacterRot = InCharacterBase->GetActorRotation();
		//更改速度方向
		SpeedVector = CharacterRot.UnrotateVector(SpeedVector);

		//if (InCharacterBase->GetLocalRole() == ENetRole::ROLE_SimulatedProxy)
		//{
			//GEngine->AddOnScreenDebugMessage(-1, 0.06f, FColor::Red, *InCharacterBase->GetVelocity().ToString());
			//GEngine->AddOnScreenDebugMessage(-1, 0.06f, FColor::Green, *SpeedVector.ToString());
		//}

		if (UCharacterMovementComponent* InCharacterMovementComponent = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent()))
		{
			//将移动速度映射到-1~1范围
			AxisSpeed.X = FMath::GetMappedRangeValueClamped(FVector2D(-InNewSpeed, InNewSpeed), FVector2D(-1.0f, 1.0f), SpeedVector.X);
			AxisSpeed.Y = FMath::GetMappedRangeValueClamped(FVector2D(-InNewSpeed, InNewSpeed), FVector2D(-1.0f, 1.0f), SpeedVector.Y);
			AxisSpeed.Z = FMath::GetMappedRangeValueClamped(FVector2D(-InNewSpeed, InNewSpeed), FVector2D(-1.0f, 1.0f), SpeedVector.Z);
		}
	}
}
