// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/SimpleBrowse.h"
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
	void ExecutionRotateCharacter();//执行旋转
	void StopRotateCharacter();//停止旋转
	void ResetTarget(AActor* InTarget);

	virtual void SetupInputComponent() override;



private:
	SimpleActorAction::FSimpleBrowse SimpleBrowse;

};
