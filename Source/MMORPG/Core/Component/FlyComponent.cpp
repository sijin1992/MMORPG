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
	
	bFastFly = false;

	DodgeFlyTime = 0.0f;
	// ...
}


// Called when the game starts
void UFlyComponent::BeginPlay()
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


void UFlyComponent::Print(float InTime, const FString& InString)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, InTime, FColor::Red, FString::Printf(TEXT("%s"), *InString));
	}
}

// Called every frame
void UFlyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MMORPGCharacterBase.IsValid() && CharacterMovementComponent.IsValid() && CapsuleComponent.IsValid() && CameraComponent.IsValid())
	{
		if (MMORPGCharacterBase->GetActionState() == ECharacterActionState::FLIGHT_STATE)
		{
			//设置角色跟随像机旋转
			FRotator CameraRotator = CameraComponent->GetComponentRotation();//获取像机旋转
			FRotator CapsuleRotator = CapsuleComponent->GetComponentRotation();//获取胶囊体旋转

			if (!bFastFly)
			{
				CameraRotator.Pitch = 0.0f;//修正Pitch，旋转时不控制Pitch,防止朝上朝下飞再落地后导致身体倾斜的问题
			}

			FRotator NewRot = FMath::RInterpTo(CapsuleRotator, CameraRotator, DeltaTime, 8.0f);//插值
			
			MMORPGCharacterBase->SetActorRotation(NewRot);//设置旋转

			//设置旋转的角速度映射到-1~1
			if (1)
			{
				//自己算角速度
				float PerFrameNum = 1.0f / DeltaTime;//算出1S的帧数

				FRotator DeltaTimeRot = MMORPGCharacterBase->GetActorRotation() - LastRotator;//计算2帧之间的角度差

				FRotator OneSecondRotator = DeltaTimeRot *= PerFrameNum;//角速度

				Print(DeltaTime, OneSecondRotator.ToString());

				RotationRate.X = FMath::GetMappedRangeValueClamped(FVector2D(-360, 360), FVector2D(-1.0f, 1.0f), OneSecondRotator.Yaw);

				RotationRate.Y = FMath::GetMappedRangeValueClamped(FVector2D(-360, 360), FVector2D(-1.0f, 1.0f), OneSecondRotator.Pitch);

				LastRotator = MMORPGCharacterBase->GetActorRotation();//保存上一次的旋转
			}
			else
			{
				//UE5无法获取角速度的BUG
				FVector PhysicsAngularVelocityInDegrees = CapsuleComponent->GetPhysicsAngularVelocityInDegrees();

				Print(DeltaTime, PhysicsAngularVelocityInDegrees.ToString());

				RotationRate.X = FMath::GetMappedRangeValueClamped(FVector2D(-360, 360), FVector2D(-1.0f, 1.0f), PhysicsAngularVelocityInDegrees.Z);
			}


			//闪避飞行时间计时
			if (DodgeFlyTime > 0.0f)
			{
				DodgeFlyTime -= DeltaTime;
				if (DodgeFlyTime <= 0.0f)
				{
					DodgeFly = EDodgeFly::DODGE_NONE;
					DodgeFlyTime = 0.0f;
				}
			}
		}
	}
}

void UFlyComponent::ResetFly()
{
	if (MMORPGCharacterBase.IsValid() && CharacterMovementComponent.IsValid())
	{
		if (MMORPGCharacterBase->GetActionState() == ECharacterActionState::FLIGHT_STATE)
		{
			CharacterMovementComponent->bOrientRotationToMovement = false;//飞行状态，不自动转向移动方向
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);//设置飞行模式
		}
		else
		{
			CharacterMovementComponent->bOrientRotationToMovement = true;//其他状态自动转向移动方向
			CharacterMovementComponent->MaxFlySpeed = 600.0f;
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);//设置走路模式
			//修正旋转
			FRotator NewRot = MMORPGCharacterBase->GetActorRotation();
			NewRot.Pitch = 0;
			MMORPGCharacterBase->SetActorRotation(NewRot);//设置旋转
		}

		bFastFly = false;
	}
}

void UFlyComponent::FlyForwardAxis(float InAxisValue)
{
	if (MMORPGCharacterBase.IsValid() && CharacterMovementComponent.IsValid() && CapsuleComponent.IsValid() && CameraComponent.IsValid())
	{
		if (bFastFly)
		{
			const FVector ForwardDirection = CameraComponent->GetForwardVector();
			MMORPGCharacterBase->AddMovementInput(ForwardDirection, 1.0f);
		}
		else
		{
			const FVector ForwardDirection = CameraComponent->GetForwardVector();
			MMORPGCharacterBase->AddMovementInput(ForwardDirection, InAxisValue);
		}
	}
}

void UFlyComponent::ResetFastFly()
{
	if (CharacterMovementComponent.IsValid())
	{
		if (bFastFly)
		{
			bFastFly = false;
			CharacterMovementComponent->MaxFlySpeed = 600.0f;
		}
		else
		{
			bFastFly = true;
			CharacterMovementComponent->MaxFlySpeed = 2000.0f;
		}
	}
}

void UFlyComponent::ResetDodgeFly(EDodgeFly InFlyState)
{
	if (bFastFly)
	{
		DodgeFly = InFlyState;
		DodgeFlyTime = 1.6f;
	}
}

