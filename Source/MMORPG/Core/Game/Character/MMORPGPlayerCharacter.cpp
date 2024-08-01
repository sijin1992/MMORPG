// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGPlayerCharacter.h"

void AMMORPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitKneadingLocation(GetMesh()->GetComponentLocation());
}

void AMMORPGPlayerCharacter::UpdateKneadingBody()
{
}

void AMMORPGPlayerCharacter::UpdateKneadingBody(const FMMORPGCharacterAppearance& InCA)
{
	SetLegSize(InCA.LegSize);
	SetWaistSize(InCA.WaistSize);
	SetArmSize(InCA.ArmSize);
	SetHeadSize(InCA.HeadSize);
	SetChestSize(InCA.ChestSize);

	//需要保证Mesh和骨骼一致
	SetMeshPosition(GetMesh());
}