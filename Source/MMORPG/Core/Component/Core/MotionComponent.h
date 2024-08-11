// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MotionComponent.generated.h"

class AMMORPGCharacterBase;
class UCapsuleComponent;
class UCharacterMovementComponent;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMORPG_API UMotionComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TWeakObjectPtr<AMMORPGCharacterBase> MMORPGCharacterBase;

	UPROPERTY()
	TWeakObjectPtr<UCapsuleComponent> CapsuleComponent;//胶囊体弱指针，用弱指针用于继承自UObject

	UPROPERTY()
	TWeakObjectPtr<UCameraComponent> CameraComponent;//像机组件弱指针

	UPROPERTY()
	TWeakObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	UPROPERTY()
	FRotator LastRotator;//上一次的旋转

public:
	FResetBool bFast;//是否加速

public:	
	// Sets default values for this component's properties
	UMotionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void LockView(float DeltaTime, bool bClearPitch = true);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
