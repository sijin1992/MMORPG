// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGClimbAnimInstance.h"
#include "../../Character/Core/MMORPGCharacterBase.h"
#include "../../../Component/ClimbComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMMORPGClimbAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMMORPGClimbAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AMMORPGCharacterBase* InCharacterBase = Cast<AMMORPGCharacterBase>(TryGetPawnOwner()))
	{
		//角速度设置
		RotationRate = InCharacterBase->GetClimbComponent()->RotationRate;

		if (UCharacterMovementComponent* InCharacterMovementComponent = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent()))
		{
			//Print(DeltaSeconds, FString::FormatAsNumber(InCharacterMovementComponent->MaxCustomMovementSpeed));
			ResetAxisSpeed(InCharacterMovementComponent->MaxCustomMovementSpeed);
		}
	}
}
