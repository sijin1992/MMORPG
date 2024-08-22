// Fill out your copyright notice in the Description page of Project Settings.


#include "MMORPGClimbAnimInstance.h"
#include "../../Character/Core/MMORPGCharacterBase.h"
#include "../../../Component/ClimbComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMMORPGClimbAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMMORPGClimbAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AMMORPGCharacterBase* InCharacterBase = Cast<AMMORPGCharacterBase>(TryGetPawnOwner()))
	{
		//角速度设置
		RotationRate = InCharacterBase->GetClimbComponent()->RotationRate;
		//攀爬状态设置
		ClimbState = InCharacterBase->GetClimbComponent()->ClimbState;

		bJumpToClimb = InCharacterBase->GetClimbComponent()->bJumpToClimb;

		bJump = *InCharacterBase->GetClimbComponent()->bJump;

		if (UCharacterMovementComponent* InCharacterMovementComponent = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent()))
		{
			//Print(DeltaSeconds, FString::FormatAsNumber(InCharacterMovementComponent->MaxCustomMovementSpeed));
			ResetAxisSpeed(InCharacterMovementComponent->MaxCustomMovementSpeed);
		}

		if (bJump)
		{
			InCharacterBase->GetClimbComponent()->bJump = false;
			ClimbJump();
		}

		if (ClimbState == EClimbState::CLIMB_TOTOP)//攀爬到顶部
		{
			InCharacterBase->GetClimbComponent()->ClearClimbingState();
			InCharacterBase->ClimbMontageChanged(EClimbMontageState::CLIMB_UPATTOP);
		}
		else if (ClimbState == EClimbState::CLIMB_THROWOVER)//翻越
		{
			InCharacterBase->GetClimbComponent()->ClearClimbingState();
			if (InCharacterBase->GetClimbComponent()->IsLowThrowOver())
			{
				//随机播放从左还是从右翻越
				InCharacterBase->ClimbMontageChanged((EClimbMontageState)FMath::RandRange((int32)EClimbMontageState::CLIMB_THROWOVER_L, (int32)EClimbMontageState::CLIMB_THROWOVER_R));
			}
			else
			{
				InCharacterBase->ClimbMontageChanged(EClimbMontageState::CLIMB_THROWOVER_HIGH);
			}
		}
		else if (ClimbState == EClimbState::CLIMB_TURN)
		{
			//旋转攀爬
			EClimbTurnState ClimbTurnState = InCharacterBase->GetClimbComponent()->GetTurnState();
			switch (ClimbTurnState)
			{
			case EClimbTurnState::OUTSIDE_RIGHT:
				InCharacterBase->ClimbMontageChanged(EClimbMontageState::CLIMB_TURN_RIGHT_OUT);
				break;
			case EClimbTurnState::OUTSIDE_LEFT:
				InCharacterBase->ClimbMontageChanged(EClimbMontageState::CLIMB_TURN_LEFT_OUT);
				break;
			case EClimbTurnState::INSIDE_RIGHT:
				InCharacterBase->ClimbMontageChanged(EClimbMontageState::CLIMB_TURN_RIGHT_IN);
				break;
			case EClimbTurnState::INSIDE_LEFT:
				InCharacterBase->ClimbMontageChanged(EClimbMontageState::CLIMB_TURN_LEFT_IN);
				break;
			}

			InCharacterBase->GetClimbComponent()->ResetClimbState();
		}
	}
}

void UMMORPGClimbAnimInstance::ClimbJump()
{
	if (AMMORPGCharacterBase* InCharacterBase = Cast<AMMORPGCharacterBase>(TryGetPawnOwner()))
	{
		EClimbMontageState InClimbJumpState = CalculationClimbJumpState();
		if (InClimbJumpState != EClimbMontageState::CLIMB_JUMP_MAX)
		{
			//Print(0.04f, FString::FromInt((int32)InClimbJumpState));
			InCharacterBase->ClimbMontageChanged(InClimbJumpState);
		}
	}
}

EClimbMontageState UMMORPGClimbAnimInstance::CalculationClimbJumpState()
{
	if (AMMORPGCharacterBase* InCharacterBase = Cast<AMMORPGCharacterBase>(TryGetPawnOwner()))
	{
		if (UCharacterMovementComponent* InCharacterMovementComponent = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent()))
		{
			FVector2D Axis(InCharacterMovementComponent->GetLastInputVector().Y, InCharacterMovementComponent->GetLastInputVector().Z);
			Axis.Normalize();//单位化

			//区分左右
			FVector2D XAxis(1.0f, 0.0f);
			float XCosValue = FVector2D::DotProduct(Axis, XAxis);//点乘
			float XAxisCosAngle = (180.0f) / PI * FMath::Acos(XCosValue);
			//Print(0.04f, FString::SanitizeFloat(XAxisCosAngle));

			//区分上下
			FVector2D YAxis(0.0f, 1.0f);
			float YCosValue = FVector2D::DotProduct(Axis, YAxis);//点乘
			float YAxisCosAngle = (180.0f) / PI * FMath::Acos(YCosValue);
			bool bUPAxis = FMath::IsWithinInclusive(YAxisCosAngle, 0.0f, 90.0f);
			//Print(0.04f, FString::SanitizeFloat(YAxisCosAngle));

			//区分单位圆里的8个方向
			if (FMath::IsWithinInclusive(XAxisCosAngle, 67.5f, 112.5f) && bUPAxis)//上
			{
				return EClimbMontageState::CLIMB_JUMP_U;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 22.5f, 67.5f) && bUPAxis)//右上
			{
				return EClimbMontageState::CLIMB_JUMP_UR;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 112.5f, 157.5f) && bUPAxis)//左上
			{
				return EClimbMontageState::CLIMB_JUMP_UL;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 67.5f, 112.5f) && !bUPAxis)//下
			{
				return EClimbMontageState::CLIMB_JUMP_D;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 22.5f, 67.5f) && !bUPAxis)//右下
			{
				return EClimbMontageState::CLIMB_JUMP_DR;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 112.5f, 157.5f) && !bUPAxis)//左下
			{
				return EClimbMontageState::CLIMB_JUMP_DL;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 157.5f, 180.0f))//左
			{
				return EClimbMontageState::CLIMB_JUMP_L;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 0.0f, 22.5f))//右
			{
				return EClimbMontageState::CLIMB_JUMP_R;
			}
		}
	}

	return EClimbMontageState::CLIMB_JUMP_MAX;
}
