
#pragma once

#include "CoreMinimal.h"
#include "MMORPGGameType.generated.h"

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	NORMAL_STATE	UMETA(DisplayName = "Normal"),//正常状态
	FIGHT_STATE		UMETA(DisplayName = "Fight"),//战斗状态
	FLIGHT_STATE	UMETA(DisplayName = "Flight"),//飞行状态
	CLIMB_STATE		UMETA(DisplayName = "Climb"),//攀爬状态
	SWIMMING_STATE	UMETA(DisplayName = "Swimming"),//游泳状态
};

//闪避飞行
UENUM(BlueprintType)
enum class EDodgeFly : uint8
{
	DODGE_NONE		UMETA(DisplayName = "None"),
	DODGE_LEFT		UMETA(DisplayName = "Left"),
	DODGE_RIGHT		UMETA(DisplayName = "Right"),
};

//攀爬状态
UENUM(BlueprintType)
enum class EClimbState : uint8
{
	CLIMB_NONE					UMETA(DisplayName = "None"),
	CLIMB_CLIMBOVEROBSTACLES	UMETA(DisplayName = "Climb over Obstacles"),//爬完翻越障碍
	CLIMB_THROWOVER				UMETA(DisplayName = "Throw-over"),//直接翻越
	CLIMB_CLIMBING				UMETA(DisplayName = "Climb"),//攀爬
	CLIMB_GROUND				UMETA(DisplayName = "Ground"),//落地
	CLIMB_TOTOP					UMETA(DisplayName = "Top"),//爬到顶部
	CLIMB_DROP					UMETA(DisplayName = "Drop"),//取消攀爬，下落
};

//攀爬蒙太奇状态
UENUM(BlueprintType)
enum class EClimbMontageState : uint8
{
	CLIMB_JUMP_D				UMETA(DisplayName = "Down"),//向下跳跃0
	CLIMB_JUMP_DL				UMETA(DisplayName = "Down Left"),//向左下跳跃1
	CLIMB_JUMP_DR				UMETA(DisplayName = "Down Right"),//向右下跳跃2
	CLIMB_JUMP_L				UMETA(DisplayName = "Left"),//向左跳跃3
	CLIMB_JUMP_R				UMETA(DisplayName = "Right"),//向右跳跃4
	CLIMB_JUMP_U				UMETA(DisplayName = "Up"),//向上跳跃5
	CLIMB_JUMP_UL				UMETA(DisplayName = "Up Left"),//向左上跳跃6
	CLIMB_JUMP_UR				UMETA(DisplayName = "Up Right"),//向右上跳跃7
	CLIMB_UPATTOP				UMETA(DisplayName = "Up At Top"),//攀爬到顶部8
	CLIMB_THROWOVER_L			UMETA(DisplayName = "Throw-over left"),//从左边翻越9
	CLIMB_THROWOVER_R			UMETA(DisplayName = "Throw-over right"),//从右边翻越10
	CLIMB_THROWOVER_HIGH		UMETA(DisplayName = "Throw-over high"),//双手翻越高一点的墙11
	CLIMB_DROP					UMETA(DisplayName = "Drop"),//取消攀爬，下落动画12
	CLIMB_JUMP_MAX				UMETA(DisplayName = "Max"),//无效值
};

struct FResetBool
{
	FResetBool()
		:bSet(false),
		Time(0.0f)
	{
	}

	void Tick(float DeltaTime)
	{
		//闪避飞行时间计时
		if (Time > 0.0f)
		{
			Time -= DeltaTime;
			if (Time <= 0.0f)
			{
				Fun.ExecuteIfBound();
				Time = 0.0f;
				bSet = false;
			}
		}
	}

	FResetBool& operator=(bool bNewSet)
	{
		bSet = bNewSet;
		return *this;
	}

	FResetBool& operator=(float bNewTime)
	{
		Time = bNewTime;
		return *this;
	}

	bool operator*()
	{
		return bSet;
	}
	//explicit显性调用
	explicit operator bool()
	{
		return bSet;
	}

	bool bSet;
	float Time;

	//TFunction<void()> FuncPtr;
	FSimpleDelegate Fun;
};