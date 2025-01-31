// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGSwimAnimInstance.h"
#include "../../Character/Core/MMORPGCharacterBase.h"
#include "../../../Component/SwimmingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMMORPGSwimAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UMMORPGSwimAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AMMORPGCharacterBase* InCharacterBase = Cast<AMMORPGCharacterBase>(TryGetPawnOwner()))
	{
		//角速度设置
		RotationRate = InCharacterBase->GetSwimmingComponent()->RotationRate;
		//是否加速游泳
		bFast = *InCharacterBase->GetSwimmingComponent()->bFast;

		//是否潜泳
		bDiving = *InCharacterBase->GetSwimmingComponent()->bDiving;

		if (UCharacterMovementComponent* InCharacterMovementComponent = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent()))
		{
			ResetAxisSpeed(InCharacterMovementComponent->MaxSwimSpeed);
		}
	}

}
