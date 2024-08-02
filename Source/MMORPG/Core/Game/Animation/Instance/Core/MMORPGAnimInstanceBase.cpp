// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGAnimInstanceBase.h"
#include "../../../Character/Core/MMORPGCharacterBase.h"
#include "GameFramework/PawnMovementComponent.h"

UMMORPGAnimInstanceBase::UMMORPGAnimInstanceBase()
	:bDeath(false),
	Speed(0.0f),
	bInAir(false),
	bFight(false)
{

}

void UMMORPGAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UMMORPGAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AMMORPGCharacterBase* InCharacterBase = Cast<AMMORPGCharacterBase>(TryGetPawnOwner()))
	{
		Speed = InCharacterBase->GetVelocity().Size();
		bInAir = InCharacterBase->GetMovementComponent()->IsFalling();
		bFight = InCharacterBase->IsFight();
	}
}
