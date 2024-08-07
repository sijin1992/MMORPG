// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGPlayerController.h"
#include "Character/MMORPGCharacter.h"
#include "MMORPGGameState.h"
#include "Character/MMORPGPlayerCharacter.h"
#include "MMORPGPlayerState.h"

AMMORPGPlayerController::AMMORPGPlayerController()
{
	bShowMouseCursor = true;
}

void AMMORPGPlayerController::ReplaceCharacter_Implementation(int32 InCharacterID)
{
	if (!GetPawn())
	{
		return;
	}

	if (AMMORPGCharacterBase* MMORPGBase = GetPawn<AMMORPGCharacterBase>())
	{
		if (MMORPGBase->GetID() == InCharacterID)
		{
			return;
		}
	}

	if (AMMORPGGameState* InGameState = GetWorld()->GetGameState<AMMORPGGameState>())
	{
		if (FCharacterStyleTable* InStyleTable = InGameState->GetCharacterStyleTable(InCharacterID))
		{
			if (AMMORPGCharacter* InNewCharacter = GetWorld()->SpawnActor<AMMORPGCharacter>(
				InStyleTable->MMORPGCharacterClass, 
				GetPawn()->GetActorLocation(),
				GetPawn()->GetActorRotation()))
			{
				if (AMMORPGPlayerState* InPlayerState = GetPlayerState<AMMORPGPlayerState>())
				{
					//判断是不是主角色，是就更新捏脸数据
					if (AMMORPGPlayerCharacter* InPlayerCharacter = Cast<AMMORPGPlayerCharacter>(InNewCharacter))
					{
						InPlayerCharacter->UpdateKneadingBody(InPlayerState->GetCA());
						InPlayerCharacter->CallUpdateKneadingBodyOnClient(InPlayerState->GetCA());
					}

					APawn* InPawn = GetPawn();
					OnPossess(InNewCharacter);//替换角色
					InPawn->Destroy(true);//销毁旧的角色
				}
			}
		}
	}
}
