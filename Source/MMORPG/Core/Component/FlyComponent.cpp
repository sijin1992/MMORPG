// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyComponent.h"
#include "../Game/Character/Core/MMORPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

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
		CapsuleComponent = MMORPGCharacterBase->GetCapsuleComponent();
		CameraComponent = MMORPGCharacterBase->GetFollowCamera();
	}
}


// Called every frame
void UFlyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MMORPGCharacterBase && CharacterMovementComponent && CapsuleComponent && CameraComponent)
	{
		if (MMORPGCharacterBase->GetActionState() == ECharacterActionState::FLIGHT_STATE)
		{
			//设置角色跟随像机旋转
			FRotator CameraRotator = CameraComponent->GetComponentRotation();//获取像机旋转
			FRotator CapsuleRotator = CapsuleComponent->GetComponentRotation();//获取胶囊体旋转

			FRotator NewRot = FMath::RInterpTo(CapsuleRotator, CameraRotator, DeltaTime, 8.0f);//插值
			
			MMORPGCharacterBase->SetActorRotation(NewRot);//设置旋转

			//设置旋转的角速度映射到-1~1
			FVector PhysicsAngularVelocityInDegrees = CapsuleComponent->GetPhysicsAngularVelocityInDegrees();
			RotationRate.X = FMath::GetMappedRangeValueClamped(FVector2D(-360,360), FVector2D(-1.0f,1.0f), PhysicsAngularVelocityInDegrees.Z);
		}
	}
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

