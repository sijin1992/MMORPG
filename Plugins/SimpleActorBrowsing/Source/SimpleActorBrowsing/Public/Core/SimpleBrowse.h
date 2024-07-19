//浏览角色

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"

namespace SimpleActorAction
{	//加上SIMPLEACTORBROWSING_API，暴露类或方法
	class SIMPLEACTORBROWSING_API FSimpleBrowse : public FTickableGameObject
	{
	public:
		FSimpleBrowse();
		FSimpleBrowse(APlayerController* InController, AActor* InTarget);

		/// <summary>
		/// 注册
		/// </summary>
		/// <param name="InController">PlayerController</param>
		/// <param name="InTarget">旋转的目标</param>
		void Register(APlayerController* InController, AActor* InTarget);

		virtual void Tick(float DeltaTime);

		virtual TStatId GetStatId() const;

		
		void OpenBrowsing();//开始浏览
		void EndBrowsing();//结束浏览

		void ResetTarget(AActor* InNewTarget);//设置新的旋转目标
	protected:
		bool bRotateCharacter;			//是否开启旋转
		APlayerController* Controller;	//旋转控制器
		TWeakObjectPtr<AActor> Target;	//旋转目标的弱指针
		FVector2D MousePos;				//鼠标位置
	};
}