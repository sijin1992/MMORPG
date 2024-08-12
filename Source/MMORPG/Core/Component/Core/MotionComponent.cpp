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

void UMotionComponent::Print(float InTime, const FString& InString)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, InTime, FColor::Red, FString::Printf(TEXT("%s"), *InString));
	}
}


void UMotionComponent::LockView(float DeltaTime, bool bClearPitch)
{
	//设置角色跟随像机旋转
	FRotator CameraRotator = CameraComponent->GetComponentRotation();//获取像机旋转
	FRotator CapsuleRotator = CapsuleComponent->GetComponentRotation();//获取胶囊体旋转

	if (bClearPitch)
	{
		CameraRotator.Pitch = 0.0f;//修正Pitch，旋转时不控制Pitch,防止朝上朝下飞再落地后导致身体倾斜的问题
	}

	FRotator NewRot = FMath::RInterpTo(CapsuleRotator, CameraRotator, DeltaTime, 8.0f);//插值

	MMORPGCharacterBase->SetActorRotation(NewRot);//设置旋转
}

void UMotionComponent::ResetRotationRate(float DeltaTime)
{
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

// Called every frame
void UMotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//加速时间计时
	bFast.Tick(DeltaTime);
}

