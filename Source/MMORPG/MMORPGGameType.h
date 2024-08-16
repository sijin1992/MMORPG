
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
	CLIMB_WALLCLIMB				UMETA(DisplayName = "Right"),//直接翻越
	CLIMB_CLIMBING				UMETA(DisplayName = "Right"),//攀爬
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