// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyComponent.h"
#include "../Game/Character/Core/MMORPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UFlyComponent::UFlyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFlyComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	MMORPGCharacterBase = Cast<AMMORPGCharacterBase>(GetOwner());
	if (MMORPGCharacterBase)
	{
		CharacterMovementComponent = Cast<UCharacterMovementComponent>(MMORPGCharacterBase->GetMovementComponent());
	}
}


// Called every frame
void UFlyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFlyComponent::ResetFly()
{
	if (MMORPGCharacterBase && CharacterMovementComponent)
	{
		if (MMORPGCharacterBase->GetActionState() == ECharacterActionState::FLIGHT_STATE)
		{
			CharacterMovementComponent->bOrientRotationToMovement = false;//飞行状态，不自动转向移动方向
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);//设置飞行模式
		}
		else
		{
			CharacterMovementComponent->bOrientRotationToMovement = true;//其他状态自动转向移动方向
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);//设置走路模式
		}
	}
}

