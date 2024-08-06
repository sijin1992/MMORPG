// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGPlayerAnimInstance.h"
#include "../../../Common/Interface/KneadingInterface.h"

void UMMORPGPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IKneadingInterface* InCharacterKneading = Cast<IKneadingInterface>(TryGetPawnOwner()))
	{
		LegSize = InCharacterKneading->GetLegSize();
		WaistSize = InCharacterKneading->GetWaistSize();
		ArmSize = InCharacterKneading->GetArmSize();
		HeadSize = InCharacterKneading->GetHeadSize();
		ChestSize = InCharacterKneading->GetChestSize();
	}
}
