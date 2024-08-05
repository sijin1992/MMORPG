#pragma once

#include "CoreMinimal.h"
#include "SimpleAdvancedAnimType.generated.h"

typedef int32 FSAAHandle;

USTRUCT(BlueprintType)
struct FFootIKInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleAdvancedAnimation|FootIKInfo")
	float Offset;
};