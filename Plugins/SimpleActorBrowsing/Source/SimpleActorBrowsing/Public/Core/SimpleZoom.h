//浏览角色-缩放
#pragma once

#include "CoreMinimal.h"

namespace SimpleActorAction
{
	class SIMPLEACTORBROWSING_API FSimpleZoom
	{
	public:
		FSimpleZoom();

		void Register(AActor* InTarget, float InDistance);

		void ZoomPositive(float InValue);//加
		void ZoomNegative(float InValue);//减

	protected:
		TWeakObjectPtr<AActor> Target;	//缩放目标的弱指针
		float Distance;					//距离
		FVector Location;				//初始位置
	};
}