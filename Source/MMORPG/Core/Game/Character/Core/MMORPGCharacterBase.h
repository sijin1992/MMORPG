// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../DataTable/CharacterAnimTable.h"
#include "MMORPGCharacterBase.generated.h"

UCLASS()
class MMORPG_API AMMORPGCharacterBase : public ACharacter
{
	GENERATED_BODY()
	friend class AMMORPGGameMode;
public:
	// Sets default values for this character's properties
	AMMORPGCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE bool IsFight() { return bFight; }
	FORCEINLINE FCharacterAnimTable* GetAnimTable() { return AnimTable; }
	FORCEINLINE int32 GetID() { return ID; }
protected:
	UPROPERTY()
	bool bFight;
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	int32 ID;

	FCharacterAnimTable* AnimTable;//角色动画
};
