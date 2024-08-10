// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGFlyAnimInstance.h"
#include "../../Character/Core/MMORPGCharacterBase.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../Component/FlyComponent.h"

UMMORPGFlyAnimInstance::UMMORPGFlyAnimInstance()
	:Super(),
	DodgeFly(EDodgeFly::DODGE_NONE)
{

}

void UMMORPGFlyAnimInstance::InitAnimInstance(ACharacter* InCharacter)
{
	Super::InitAnimInstance(InCharacter);
}

void UMMORPGFlyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UMMORPGFlyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AMMORPGCharacterBase* InCharacterBase = Cast<AMMORPGCharacterBase>(TryGetPawnOwner()))
	{
		FVector SpeedVector = InCharacterBase->GetVelocity();
		//处理旋转
		FRotator CharacterRot = InCharacterBase->GetActorRotation();
		//更改速度方向
		SpeedVector = CharacterRot.UnrotateVector(SpeedVector);

		if (UCharacterMovementComponent* InCharacterMovementComponent = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent()))
		{
			float MaxFlySpeed = InCharacterMovementComponent->MaxFlySpeed;
			//将移动速度映射到-1~1范围
			FlySpeed.X = FMath::GetMappedRangeValueClamped(FVector2D(-MaxFlySpeed, MaxFlySpeed), FVector2D(-1.0f, 1.0f), SpeedVector.X);
			FlySpeed.Y = FMath::GetMappedRangeValueClamped(FVector2D(-MaxFlySpeed, MaxFlySpeed), FVector2D(-1.0f, 1.0f), SpeedVector.Y);
			FlySpeed.Z = FMath::GetMappedRangeValueClamped(FVector2D(-MaxFlySpeed, MaxFlySpeed), FVector2D(-1.0f, 1.0f), SpeedVector.Z);
		}
		//角速度设置
		RotationRate = InCharacterBase->GetFlyComponent()->RotationRate;

		//是否加速飞行
		bFastFly = *InCharacterBase->GetFlyComponent()->bFastFly;

		//闪避飞行
		DodgeFly = InCharacterBase->GetFlyComponent()->DodgeFly;

		//飞行是否着地
		bLand = *InCharacterBase->GetFlyComponent()->bLand;
	}
}
