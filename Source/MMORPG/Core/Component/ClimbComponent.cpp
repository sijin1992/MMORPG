// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbComponent.h"
#include "../Game/Character/Core/MMORPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

const float MaxDistance = 99999999.0f;

UClimbComponent::UClimbComponent()
	:Super(),
	ClimbState(EClimbState::CLIMB_NONE)
{

}

void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MMORPGCharacterBase.IsValid() && CharacterMovementComponent.IsValid() && CapsuleComponent.IsValid() && CameraComponent.IsValid())
	{
		//做命令激活
		TraceClimbingState(DeltaTime);
	}
}

void UClimbComponent::ClimbForwardAxis(float InAxisValue)
{
	if (MMORPGCharacterBase.IsValid() && CharacterMovementComponent.IsValid() && CapsuleComponent.IsValid() && CameraComponent.IsValid())
	{
		if (AController* Controller = MMORPGCharacterBase->GetController())
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
			// add movement 
			MMORPGCharacterBase->AddMovementInput(ForwardDirection, InAxisValue);
		}
	}
}

void UClimbComponent::ClimbRightAxis(float InAxisValue)
{
	if (MMORPGCharacterBase.IsValid() && CharacterMovementComponent.IsValid() && CapsuleComponent.IsValid() && CameraComponent.IsValid())
	{
		const FRotator Rotation = MMORPGCharacterBase->GetActorRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		MMORPGCharacterBase->AddMovementInput(RightDirection, InAxisValue);
	}
}

void UClimbComponent::PhysClimbing(float deltaTime, int32 Iterations)
{
	if (MMORPGCharacterBase.IsValid() && CharacterMovementComponent.IsValid() && CapsuleComponent.IsValid() && CameraComponent.IsValid())
	{
		if (deltaTime < UCharacterMovementComponent::MIN_TICK_TIME)
		{
			return;
		}
		//求移动速度
		CharacterMovementComponent->Velocity = CharacterMovementComponent->GetLastInputVector() * CharacterMovementComponent->MaxCustomMovementSpeed;

		const FVector Adjusted = CharacterMovementComponent->Velocity * deltaTime;

		FHitResult Hit(1.0f);
		CharacterMovementComponent->SafeMoveUpdatedComponent(Adjusted, CharacterMovementComponent->UpdatedComponent->GetComponentQuat(), true, Hit);
	}
}

void UClimbComponent::TraceClimbingState(float DelaTime)
{
	FVector ForwardDirection = MMORPGCharacterBase->GetActorForwardVector();
	FVector Location = MMORPGCharacterBase->GetActorLocation();

	//从胸口发出射线
	FHitResult HitChestResult;
	float ChestDistance = MaxDistance;//到碰撞点的距离
	{
		FVector StartTraceChestLocation = Location;
		StartTraceChestLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight() / 4.0f;
		FVector EndTraceChestLocation = StartTraceChestLocation + ForwardDirection * 100.0f;


		TArray<AActor*> ChestActorsToIgnore;
		UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			StartTraceChestLocation,
			EndTraceChestLocation,
			ETraceTypeQuery::TraceTypeQuery1,
			true,
			ChestActorsToIgnore,
			EDrawDebugTrace::ForOneFrame,
			HitChestResult,
			true
		);

		if (HitChestResult.bBlockingHit)
		{
			ChestDistance = FVector::Distance(StartTraceChestLocation, HitChestResult.Location);
		}
	}

	//从头顶发出射线
	FHitResult HitHeadResult;
	float HeadDistance = MaxDistance;//到碰撞点的距离
	{
		FVector StartTraceHeadLocation = Location;
		StartTraceHeadLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight();
		FVector EndTraceHeadLocation = StartTraceHeadLocation + ForwardDirection * 100.0f;

		TArray<AActor*> HeadActorsToIgnore;
		UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			StartTraceHeadLocation,
			EndTraceHeadLocation,
			ETraceTypeQuery::TraceTypeQuery1,
			true,
			HeadActorsToIgnore,
			EDrawDebugTrace::ForOneFrame,
			HitHeadResult,
			true
		);

		if (HitHeadResult.bBlockingHit)
		{
			HeadDistance = FVector::Distance(StartTraceHeadLocation, HitHeadResult.Location);
		}
	}

	if (HitChestResult.bBlockingHit && HitHeadResult.bBlockingHit)//头和胸都打到墙了,就是爬墙状态
	{
		if (ChestDistance <= 50.0f && HeadDistance <= 50.0f)
		{
			if (ClimbState == EClimbState::CLIMB_CLIMBING)
			{
			}
			else
			{
				ClimbState = EClimbState::CLIMB_CLIMBING;
				CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Custom);
				CharacterMovementComponent->bOrientRotationToMovement = false;
				MMORPGCharacterBase->ResetActionState(ECharacterActionState::CLIMB_STATE);
			}
		}
	}
	else if (HitChestResult.bBlockingHit && !HitHeadResult.bBlockingHit)//胸都打到墙了,头没有，攀爬后翻越
	{
		ClimbState = EClimbState::CLIMB_CLIMBOVEROBSTACLES;
	}
	else if (!HitChestResult.bBlockingHit && !HitHeadResult.bBlockingHit)//头和胸都没有打到，就是NONE
	{
		ClimbState = EClimbState::CLIMB_NONE;
	}
}
