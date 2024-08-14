// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "ClimbComponent.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UClimbComponent : public UMotionComponent
{
	GENERATED_BODY()

public:
	void ClimbForwardAxis(float InAxisValue);

	void ClimbRightAxis(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Climbing")
	void PhysClimbing(float deltaTime, int32 Iterations);
};
