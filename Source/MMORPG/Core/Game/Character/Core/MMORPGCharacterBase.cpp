// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGCharacterBase.h"
#include "../../MMORPGGameState.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMMORPGCharacterBase::AMMORPGCharacterBase()
	:bFight(false),
	ID(INDEX_NONE)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMMORPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld())
	{
		if (AMMORPGGameState* InGameState = GetWorld()->GetGameState<AMMORPGGameState>())
		{
			if (FCharacterAnimTable* InAnimTable = InGameState->GetCharacterAnimTable(GetID()))
			{
				AnimTable = InAnimTable;
			}
		}
	}
}

void AMMORPGCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//条件注册,COND_SimulatedOnly表示只更新模拟玩家
	DOREPLIFETIME_CONDITION(AMMORPGCharacterBase, bFight, COND_SimulatedOnly);
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

void AMMORPGCharacterBase::SwitchFightOnServer_Implementation(bool bNewFight)
{
	bFight = bNewFight;
}
