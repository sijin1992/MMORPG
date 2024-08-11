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
		//角速度设置
		RotationRate = InCharacterBase->GetFlyComponent()->RotationRate;

		//是否加速飞行
		bFast = *InCharacterBase->GetFlyComponent()->bFast;

		//闪避飞行
		DodgeFly = InCharacterBase->GetFlyComponent()->DodgeFly;

		//飞行是否着地
		bLand = *InCharacterBase->GetFlyComponent()->bLand;
	}
}
