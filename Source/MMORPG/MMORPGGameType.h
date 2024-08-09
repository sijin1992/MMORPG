
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