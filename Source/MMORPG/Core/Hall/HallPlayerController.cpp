// Fill out your copyright notice in the Description page of Project Settings.


#include "HallPlayerController.h"

AHallPlayerController::AHallPlayerController()
{
	bShowMouseCursor = true;			//显示鼠标
	bEnableClickEvents = true;			//开启敲击事件
	bEnableTouchEvents = true;			//开启触摸事件
}

void AHallPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SimpleBrowse.Register(this, NULL);
	SimpleZoom.Register(GetPawn(), 400.0f);
	SimplePanelMove.Register(this, GetPawn(), 100.0f);
}

void AHallPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AHallPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MouseClick", IE_Released, this, &AHallPlayerController::StopRotateCharacter);
	InputComponent->BindAxis("Zoom", this, &AHallPlayerController::Zoom);

	InputComponent->BindAction("MouseRightClick", IE_Pressed, this, &AHallPlayerController::BeginMove);
	InputComponent->BindAction("MouseRightClick", IE_Released, this, &AHallPlayerController::EndMove);
}

void AHallPlayerController::ExecutionRotateCharacter()
{
	SimpleBrowse.OpenBrowsing();
}

void AHallPlayerController::StopRotateCharacter()
{
	SimpleBrowse.EndBrowsing();
}

void AHallPlayerController::ResetTarget(AActor* InTarget)
{
	SimpleBrowse.ResetTarget(InTarget);
}

void AHallPlayerController::Zoom(float InDeltaTime)
{
	if (InDeltaTime > 0)
	{
		SimpleZoom.ZoomPositive(InDeltaTime * 10.0f);
	}
	else if (InDeltaTime < 0)
	{
		SimpleZoom.ZoomNegative(InDeltaTime * 10.0f);
	}
}

void AHallPlayerController::BeginMove()
{
	SimplePanelMove.BeginMove();
}

void AHallPlayerController::EndMove()
{
	SimplePanelMove.EndMove();
}
