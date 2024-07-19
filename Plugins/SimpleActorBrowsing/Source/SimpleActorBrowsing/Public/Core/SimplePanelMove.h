//浏览角色-移动上下视口

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"

namespace SimpleActorAction
{	//加上SIMPLEACTORBROWSING_API，暴露类或方法
	class SIMPLEACTORBROWSING_API FSimplePanelMove : public FTickableGameObject
	{
	public:
		FSimplePanelMove();
		/// <summary>
		/// 注册
		/// </summary>
		/// <param name="InController">PlayerController</param>
		/// <param name="InTarget">旋转的目标</param>
		void Register(APlayerController* InController, AActor* InTarget, float InDistance);

		virtual void Tick(float DeltaTime);

		virtual TStatId GetStatId() const;


		void BeginMove();//开始移动
		void EndMove();//结束移动

		void ResetTarget(AActor* InNewTarget);//设置新的旋转目标
	protected:
		bool bMove;						//是否开启移动
		APlayerController* Controller;	//移动控制器
		TWeakObjectPtr<AActor> Target;	//旋转目标的弱指针
		FVector2D MousePos;				//鼠标位置
		FVector Location;				//初始位置
		float Distance;					//移动距离
	};
}