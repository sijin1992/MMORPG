// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/SimpleBrowse.h"
#include "Core/SimpleZoom.h"
#include "HallPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API AHallPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHallPlayerController();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaSeconds) override;
public:
	virtual void SetupInputComponent() override;

	//旋转
	void ExecutionRotateCharacter();//执行旋转
	void StopRotateCharacter();//停止旋转
	void ResetTarget(AActor* InTarget);

	//缩放
	void Zoom(float InDeltaTime);

private:
	SimpleActorAction::FSimpleBrowse SimpleBrowse;
	SimpleActorAction::FSimpleZoom SimpleZoom;
};
