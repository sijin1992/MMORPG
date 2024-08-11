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

// Called every frame
void UMotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//加速时间计时
	bFast.Tick(DeltaTime);
}

