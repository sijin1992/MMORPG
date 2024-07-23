// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStage.h"
#include "Components/CapsuleComponent.h"
#include "../HallPlayerController.h"
#include "../HallPlayerState.h"

//关闭优化
#if PLATFORM_WINDOWS
#pragma optimize("", off)
#endif

// Sets default values
ACharacterStage::ACharacterStage()
	:SlotID(INDEX_NONE)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACharacterStage::UpdateKneadingBody()
{
	if (SlotID != INDEX_NONE)
	{
		if (AHallPlayerState* InPlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AHallPlayerState>())
		{
			if (FMMORPGCharacterAppearance* InCA = InPlayerState->GetCharacterCA(SlotID))
			{
				UpdateKneadingBody(*InCA);
			}
			else if (InPlayerState->GetCurrentTmpCreateCharacter() && InPlayerState->GetCurrentTmpCreateCharacter()->SlotPosition == SlotID)
			{
				UpdateKneadingBody(*InPlayerState->GetCurrentTmpCreateCharacter());
			}
		}
	}
}

void ACharacterStage::UpdateKneadingBody(const FMMORPGCharacterAppearance& InCA)
{
	SetLegSize(InCA.LegSize);
	SetWaistSize(InCA.WaistSize);
	SetArmSize(InCA.ArmSize);
	SetHeadSize(InCA.HeadSize);
	SetChestSize(InCA.ChestSize);

	//需要保证Mesh和骨骼一致
	SetMeshPosition(GetMesh());
}

// Called when the game starts or when spawned
void ACharacterStage::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetCapsuleComponent())//获取胶囊体
	{
		GetCapsuleComponent()->OnClicked.AddDynamic(this, &ACharacterStage::OnClicked);//绑定敲击事件
		//GetCapsuleComponent()->OnInputTouchBegin
	}

	InitKneadingLocation(GetMesh()->GetComponentLocation());
}

// Called every frame
void ACharacterStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterStage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterStage::OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (AHallPlayerController* InPlayerController = GetWorld()->GetFirstPlayerController<AHallPlayerController>())
	{
		InPlayerController->ResetTarget(this);
		InPlayerController->ExecutionRotateCharacter();
	}
}

void ACharacterStage::SetSlotID(int32 InID)
{
	SlotID = InID;
}

//打开优化
#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif
