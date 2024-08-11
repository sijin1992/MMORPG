// Fill out your copyright notice in the Description page of Project Settings.


#include "SwimmingComponent.h"
#include "../Game/Character/Core/MMORPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

void USwimmingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USwimmingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CharacterMovementComponent.IsValid())
	{
		if (CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Swimming
			&& MMORPGCharacterBase->GetActionState() != ECharacterActionState::SWIMMING_STATE)
		{
			MMORPGCharacterBase->ResetActionState(ECharacterActionState::SWIMMING_STATE);
		}
		else if (CharacterMovementComponent->MovementMode != EMovementMode::MOVE_Swimming
			&& MMORPGCharacterBase->GetActionState() == ECharacterActionState::SWIMMING_STATE)
		{
			MMORPGCharacterBase->ResetActionState(ECharacterActionState::SWIMMING_STATE);
		}
		else if (CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Swimming
			&& MMORPGCharacterBase->GetActionState() == ECharacterActionState::SWIMMING_STATE)
		{

		}
	}
}
