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
	if (MMORPGCharacterBase.IsValid())
	{
		if (CharacterMovementComponent.IsValid())
		{
			CharacterMovementComponent->MaxAcceleration = 2500.0f;//设置最大加速度
			CharacterMovementComponent->BrakingDecelerationFlying = 1400.0f;//飞行摩擦力/飞行减速
		}

		//MMORPGCharacterBase->LandedDelegate.AddDynamic(this, &UFlyComponent::Landed);
		if (CapsuleComponent.IsValid())
		{
			CapsuleComponent->OnComponentHit.AddDynamic(this, &UFlyComponent::LandHit);
		}

		bFastFly.Fun.BindLambda([&]() 
			{
				DodgeFly = EDodgeFly::DODGE_NONE;
			});

		bLand.Fun.BindLambda([&]()
			{
				if (MMORPGCharacterBase.IsValid())
				{
					MMORPGCharacterBase->ResetActionState(ECharacterActionState::FLIGHT_STATE);
					ResetFly();
				}
			});
	}
}


void UFlyComponent::Print(float InTime, const FString& InString)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, InTime, FColor::Red, FString::Printf(TEXT("%s"), *InString));
	}
}

void UFlyComponent::Landed(const FHitResult& InHit)
{
	if (MMORPGCharacterBase.IsValid())
	{
		if (MMORPGCharacterBase->GetActionState() == ECharacterActionState::FLIGHT_STATE && bFastFly)
		{
			bLand = true;
			bLand = 1.6f;
		}
	}
}

void UFlyComponent::LandHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (MMORPGCharacterBase.IsValid())
	{
		if (MMORPGCharacterBase->GetActionState() == ECharacterActionState::FLIGHT_STATE && bFastFly)
		{
			float CosValue = FVector::DotProduct(CapsuleComponent->GetForwardVector(), Hit.ImpactNormal);//胶囊体向前的向量与射线碰撞点的法线向量点乘
			float CosAngle = (180.0f) / PI * FMath::Acos(CosValue);//通过弧度求弧度所对应的角度

			if (CosAngle >= 125.0f)
			{
				if (Hit.ImpactNormal.Z > 0.5f)//与山体碰撞时不要执行着陆
				{
					Reset();

					bLand = true;
					bLand = 1.6f;
				}
			}
		}
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
			if (MMORPGCharacterBase->GetLocalRole() == ENetRole::ROLE_Authority ||
				MMORPGCharacterBase->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)//服务器玩家和主机玩家也需要计算,模拟玩家不需要执行，因为服务器会顺带到客户端
			{
				if (!bLand)
				{
					LockView(DeltaTime , *bFastFly);

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
				}
			}
		}

		//闪避飞行时间计时
		bFastFly.Tick(DeltaTime);
		//着陆时间计时
		bLand.Tick(DeltaTime);
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
			Reset();
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
		bFastFly = 1.6f;
	}
}

void UFlyComponent::Reset()
{
	CharacterMovementComponent->bOrientRotationToMovement = true;//其他状态自动转向移动方向
	CharacterMovementComponent->MaxFlySpeed = 600.0f;
	CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);//设置走路模式
	//修正旋转
	FRotator NewRot = MMORPGCharacterBase->GetActorRotation();
	NewRot.Pitch = 0;
	MMORPGCharacterBase->SetActorRotation(NewRot);//设置旋转
}

