// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlyComponent.generated.h"

class AMMORPGCharacterBase;
class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMORPG_API UFlyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFlyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ResetFly();

protected:
	UPROPERTY()
	AMMORPGCharacterBase* MMORPGCharacterBase;

	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;
};
