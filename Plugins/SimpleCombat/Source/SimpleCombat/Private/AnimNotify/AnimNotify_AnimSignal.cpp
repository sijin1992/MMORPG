// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_AnimSignal.h"
#include "CombatInterface/SimpleCombatInterface.h"

UAnimNotify_AnimSignal::UAnimNotify_AnimSignal()
	:SignalValue(INDEX_NONE)
{

}

FString UAnimNotify_AnimSignal::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_AnimSignal::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	//获取角色,角色会继承接口
	if (ISimpleCombatInterface* InSimpleCombatInterface = Cast<ISimpleCombatInterface>(MeshComp->GetOuter()))
	{
		InSimpleCombatInterface->AnimSignal(SignalValue);
	}
}