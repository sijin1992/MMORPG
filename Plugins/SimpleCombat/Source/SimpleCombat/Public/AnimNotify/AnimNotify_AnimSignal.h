// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AnimSignal.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLECOMBAT_API UAnimNotify_AnimSignal : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_AnimSignal();

	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	int32 SignalValue;
};
