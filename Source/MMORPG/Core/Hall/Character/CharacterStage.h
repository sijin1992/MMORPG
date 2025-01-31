// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../Common/Interface/KneadingInterface.h"
#include "CharacterStage.generated.h"

UCLASS()
class MMORPG_API ACharacterStage : public ACharacter, public IKneadingInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterStage();

	virtual void UpdateKneadingBody() override;
	virtual void UpdateKneadingBody(const FMMORPGCharacterAppearance& InCA) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	void SetSlotID(int32 InID);
	int32 GetSlotID() { return SlotID; }
private:
	int32 SlotID;//角色插槽下标
};
