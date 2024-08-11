// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionComponent.h"
#include "../../Game/Character/Core/MMORPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UMotionComponent::UMotionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMotionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	MMORPGCharacterBase = Cast<AMMORPGCharacterBase>(GetOwner());
	if (MMORPGCharacterBase.IsValid())
	{
		CharacterMovementComponent = Cast<UCharacterMovementComponent>(MMORPGCharacterBase->GetMovementComponent());
		CapsuleComponent = MMORPGCharacterBase->GetCapsuleComponent();
		CameraComponent = MMORPGCharacterBase->GetFollowCamera();
	}
	
}


// Called every frame
void UMotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

