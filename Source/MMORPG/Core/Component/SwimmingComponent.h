// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "SwimmingComponent.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API USwimmingComponent : public UMotionComponent
{
	GENERATED_BODY()

public:
	FResetBool bDiving;//是否潜水

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SwimForwardAxis(float InValue);

	void ResetFastSwim();

	void GoUnderWater();
};
