// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGCharacterBase.h"
#include "../../MMORPGGameState.h"
#include "Net/UnrealNetwork.h"
#include "../../Animation/Instance/Core/MMORPGAnimInstanceBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../../Component/FlyComponent.h"
#include "../../../Component/SwimmingComponent.h"
#include "../../../Component/ClimbComponent.h"

// Sets default values
AMMORPGCharacterBase::AMMORPGCharacterBase()
	:ActionState(ECharacterActionState::NORMAL_STATE),
	LastActionState(ECharacterActionState::NORMAL_STATE),
	ID(INDEX_NONE),
	UserID(INDEX_NONE)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlyComponent = CreateDefaultSubobject<UFlyComponent>(TEXT("FlightComponent"));
	SwimmingComponent = CreateDefaultSubobject<USwimmingComponent>(TEXT("SwimmingComponent"));
	ClimbComponent = CreateDefaultSubobject<UClimbComponent>(TEXT("ClimbComponent"));
	//FlyComponent->SetupAttachment(RootComponent);
}

void AMMORPGCharacterBase::AnimSignal(int32 InSignal)
{
	K2_AnimSignal(InSignal);
}

// Called when the game starts or when spawned
void AMMORPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	//注册角色动画
	if (GetWorld())
	{
		if (AMMORPGGameState* InGameState = GetWorld()->GetGameState<AMMORPGGameState>())
		{
			if (FCharacterAnimTable* InAnimTable = InGameState->GetCharacterAnimTable(GetID()))
			{
				AnimTable = InAnimTable;
			}
		}

		//服务器没必要执行
		if (!GetWorld()->IsServer())
		{
			if (GetMesh())
			{
				if (UMMORPGAnimInstanceBase* InAnimInstanceBase = Cast<UMMORPGAnimInstanceBase>(GetMesh()->GetAnimInstance()))
				{
					InAnimInstanceBase->InitAnimInstance(this);
				}
			}
		}
	}
}

void AMMORPGCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//条件注册,COND_SimulatedOnly表示只更新模拟玩家
	DOREPLIFETIME_CONDITION(AMMORPGCharacterBase, ActionState, COND_SimulatedOnly);
}

// Called every frame
void AMMORPGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMMORPGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMMORPGCharacterBase::ResetActionState(ECharacterActionState InNewActionState)
{
	//客户端改变值
	if (ActionState == InNewActionState)
	{
		ActionState = ECharacterActionState::NORMAL_STATE;
	}
	else
	{
		ActionState = InNewActionState;
	}
}

void AMMORPGCharacterBase::SwitchActionStateOnServer_Implementation(ECharacterActionState InActionState)
{
	ActionState = InActionState;

	LastActionState = ActionState;
}