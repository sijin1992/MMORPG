// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/SimpleBrowse.h"
#include "Core/SimpleZoom.h"
#include "Core/SimplePanelMove.h"
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

	//移动
	void BeginMove();
	void EndMove();

private:
	SimpleActorAction::FSimpleBrowse SimpleBrowse;			//旋转实例
	SimpleActorAction::FSimpleZoom SimpleZoom;				//缩放实例
	SimpleActorAction::FSimplePanelMove SimplePanelMove;	//上下移动实例
};
