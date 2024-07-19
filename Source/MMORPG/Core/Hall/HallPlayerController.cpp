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
}

void AHallPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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

void AHallPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MouseClick", IE_Released, this, &AHallPlayerController::StopRotateCharacter);
}
