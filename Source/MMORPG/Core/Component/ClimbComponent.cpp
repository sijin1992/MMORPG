// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbComponent.h"
#include "../Game/Character/Core/MMORPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ThreadManage.h"

const float MaxDistance = 99999999.0f;

UClimbComponent::UClimbComponent()
	:Super(),
	ClimbState(EClimbState::CLIMB_NONE),
	bJumpToClimb(false),
	ThrowOverHeight(0.0f)
{

}

void UClimbComponent::BeginPlay()
{
	Super::BeginPlay();

	bThrowOver.Fun.BindLambda([&]()
		{
			AdjustmentClimbing(false);//翻越动画结束后还原角色高度
		});

	bTurn.Fun.BindLambda([&]()
		{
			CharacterMovementComponent->GravityScale = 1.0f;//关闭重力

			//暂时暴力解决BUG
			FVector ForwardDirection = MMORPGCharacterBase->GetActorForwardVector();
			FVector Location = MMORPGCharacterBase->GetActorLocation();

			//旋转攀爬后进行射线检测
			FHitResult HitChestResult;
			float ChestDistance = Scanning(HitChestResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation)
				{
					StartTraceLocation = Location;
					StartTraceLocation.Z -= CapsuleComponent->GetScaledCapsuleHalfHeight() / 2.0f;
					EndTraceLocation = StartTraceLocation + ForwardDirection * CapsuleComponent->GetScaledCapsuleHalfHeight() * 2.0f;
				});

			//修正旋转攀爬后距离墙体的距离
			if (ChestDistance >= 29.0f)
			{
				Location += ForwardDirection * (ChestDistance - 29.0f);
			}
			else
			{
				Location -= ForwardDirection * (29.0f - ChestDistance);
			}
			MMORPGCharacterBase->SetActorLocation(Location);
		});
}

void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MMORPGCharacterBase.IsValid() && CharacterMovementComponent.IsValid() && CapsuleComponent.IsValid() && CameraComponent.IsValid())
	{
		//做命令激活
		TraceClimbingState(DeltaTime);

		bJump.Tick(DeltaTime);

		bThrowOver.Tick(DeltaTime);

		bTurn.Tick(DeltaTime);

		AdjustPendingLaunchVelocity(DeltaTime);
	}
}

void UClimbComponent::ClimbForwardAxis(float InAxisValue)
{
	if (MMORPGCharacterBase.IsValid() && CharacterMovementComponent.IsValid() && CapsuleComponent.IsValid() && CameraComponent.IsValid())
	{
		const FRotator Rotation = MMORPGCharacterBase->GetActorRotation();
		const FRotator YawRotation(Rotation.Pitch, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
		// add movement 
		MMORPGCharacterBase->AddMovementInput(ForwardDirection, InAxisValue);
	}
}

void UClimbComponent::ClimbRightAxis(float InAxisValue)
{
	if (MMORPGCharacterBase.IsValid() && CharacterMovementComponent.IsValid() && CapsuleComponent.IsValid() && CameraComponent.IsValid())
	{
		const FRotator Rotation = MMORPGCharacterBase->GetActorRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		MMORPGCharacterBase->AddMovementInput(RightDirection, InAxisValue);
	}
}

void UClimbComponent::PhysClimbing(float deltaTime, int32 Iterations)
{
	if (MMORPGCharacterBase.IsValid() && CharacterMovementComponent.IsValid() && CapsuleComponent.IsValid() && CameraComponent.IsValid())
	{
		if (deltaTime < UCharacterMovementComponent::MIN_TICK_TIME)
		{
			return;
		}

		if (!CharacterMovementComponent->HasAnimRootMotion())
		{
			//求移动速度
			CharacterMovementComponent->Velocity = PendingLaunchVelocity + CharacterMovementComponent->GetLastInputVector() * CharacterMovementComponent->MaxCustomMovementSpeed;
		}
		else
		{
			CharacterMovementComponent->Velocity = PendingLaunchVelocity + CharacterMovementComponent->ConstrainAnimRootMotionVelocity(CharacterMovementComponent->AnimRootMotionVelocity, CharacterMovementComponent->Velocity);
			//Print(0.04f, CharacterMovementComponent->AnimRootMotionVelocity.ToString());
		}

		const FVector Adjusted = CharacterMovementComponent->Velocity * deltaTime;

		FHitResult Hit(1.0f);
		CharacterMovementComponent->SafeMoveUpdatedComponent(Adjusted, CharacterMovementComponent->UpdatedComponent->GetComponentQuat(), true, Hit);
	}
}

void UClimbComponent::ResetJump()
{
	bJump = true;
	//bJump = 1.6f;
}

void UClimbComponent::TraceClimbingState(float DeltaTime)
{
	//如果正在旋转攀爬,就不再检测
	if (bTurn)
	{
		return;
	}

	FVector ForwardDirection = MMORPGCharacterBase->GetActorForwardVector();
	FVector UpDirection = MMORPGCharacterBase->GetActorUpVector();
	FVector RightDirection = MMORPGCharacterBase->GetActorRightVector();
	FVector Location = MMORPGCharacterBase->GetActorLocation();
	FRotator ActorRotation = MMORPGCharacterBase->GetActorRotation();

	//从胸口发出射线(下移至腿部)
	FHitResult HitChestResult;
	float ChestDistance = Scanning(HitChestResult, [&](FVector& StartTraceChestLocation, FVector& EndTraceChestLocation)
		{
			StartTraceChestLocation = Location;
			StartTraceChestLocation.Z -= CapsuleComponent->GetScaledCapsuleHalfHeight() / 2.0f;
			EndTraceChestLocation = StartTraceChestLocation + ForwardDirection * CapsuleComponent->GetScaledCapsuleHalfHeight() * 2.0f;
		});

	//从头顶发出射线
	FHitResult HitHeadResult;
	//到碰撞点的距离
	float HeadDistance = Scanning(HitHeadResult, [&](FVector& StartTraceHeadLocation, FVector& EndTraceHeadLocation)
		{
			StartTraceHeadLocation = Location;
			StartTraceHeadLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight();
			EndTraceHeadLocation = StartTraceHeadLocation + ForwardDirection * CapsuleComponent->GetScaledCapsuleHalfHeight() * 2.0f;
		});

	//从脚向地面发出射线
	FHitResult HitGroundResult;
	float GroundDistance = Scanning(HitGroundResult, [&](FVector& StartTraceGroundLocation, FVector& EndTraceGroundLocation)
		{
			StartTraceGroundLocation = Location;
			StartTraceGroundLocation.Z -= CapsuleComponent->GetScaledCapsuleHalfHeight();
			EndTraceGroundLocation = StartTraceGroundLocation + (-UpDirection) * 40.0f;
		});

	//向右发出射线
	FHitResult HitRightResult;
	float RightDistance = Scanning(HitRightResult, [&](FVector& StartRightLocation, FVector& EndRightLocation)
		{
			StartRightLocation = Location + RightDirection * 10.0f - ForwardDirection * 10.0f;
			EndRightLocation = StartRightLocation + RightDirection * CapsuleComponent->GetScaledCapsuleHalfHeight();
		});

	//向左发出射线
	FHitResult HitLeftResult;
	float LeftDistance = Scanning(HitLeftResult, [&](FVector& StartLeftLocation, FVector& EndLeftLocation)
		{
			StartLeftLocation = Location - RightDirection * 10.0f - ForwardDirection * 10.0f;
			EndLeftLocation = StartLeftLocation - RightDirection * CapsuleComponent->GetScaledCapsuleHalfHeight();
		});

	//从右70处向前发出射线
	FHitResult HitRightForwardResult;
	float RightForwardDistance = Scanning(HitRightForwardResult, [&](FVector& StartRightForwardLocation, FVector& EndRightForwardLocation)
		{
			StartRightForwardLocation = Location + RightDirection * 70.0f;
			EndRightForwardLocation = StartRightForwardLocation + ForwardDirection * 100.0f;
		});

	//从左70处向前发出射线
	FHitResult HitLeftForwardResult;
	float LeftForwardDistance = Scanning(HitLeftForwardResult, [&](FVector& StartLeftForwardLocation, FVector& EndLeftForwardLocation)
		{
			StartLeftForwardLocation = Location - RightDirection * 70.0f;
			EndLeftForwardLocation = StartLeftForwardLocation + ForwardDirection * 100.0f;
		});

	//检测旋转攀爬
	auto CheckTurn = [&]()
		{
			if (!HitRightForwardResult.bBlockingHit && !HitLeftResult.bBlockingHit && !HitRightResult.bBlockingHit)
			{
				FHitResult HitTurnRightOutResult;
				float TurnRightOutDistance = Scanning(HitTurnRightOutResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation)
					{
						StartTraceLocation = Location + RightDirection * 70.0f + ForwardDirection * 40.0f;
						EndTraceLocation = StartTraceLocation - RightDirection * CapsuleComponent->GetScaledCapsuleHalfHeight();
					});

				if (HitTurnRightOutResult.bBlockingHit && TurnRightOutDistance != 0.0f)
				{
					if (TurnRightOutDistance >= 8.0f)
					{
						bTurn = true;
						bTurn = 1.2f;
						TurnState = EClimbTurnState::OUTSIDE_RIGHT;
					}
				}
			}
			else if (!HitLeftForwardResult.bBlockingHit && !HitLeftResult.bBlockingHit && !HitRightResult.bBlockingHit)
			{
				FHitResult HitTurnLeftOutResult;
				float TurnLeftOutDistance = Scanning(HitTurnLeftOutResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation)
					{
						StartTraceLocation = Location - RightDirection * 70.0f + ForwardDirection * 40.0f;
						EndTraceLocation = StartTraceLocation + RightDirection * CapsuleComponent->GetScaledCapsuleHalfHeight();
					});

				if (HitTurnLeftOutResult.bBlockingHit && TurnLeftOutDistance != 0.0f)
				{
					if (TurnLeftOutDistance >= 8.0f)
					{
						bTurn = true;
						bTurn = 1.2f;
						TurnState = EClimbTurnState::OUTSIDE_LEFT;
					}
				}
			}
			else if (HitLeftResult.bBlockingHit)
			{
				if (FMath::IsNearlyEqual(LeftDistance, 43.0f, 0.7f))
				{
					bTurn = true;
					bTurn = 1.2f;
					TurnState = EClimbTurnState::INSIDE_LEFT;
				}
			}
			else if (HitRightResult.bBlockingHit)
			{
				if (FMath::IsNearlyEqual(RightDistance, 49.0f, 0.7f))
				{
					bTurn = true;
					bTurn = 1.2f;
					TurnState = EClimbTurnState::INSIDE_RIGHT;
				}
			}

			if (bTurn)
			{
				ClimbState = EClimbState::CLIMB_TURN;
				CharacterMovementComponent->GravityScale = 0.0f;//关闭重力
			}
		};


	//处理状态

	if (HitChestResult.bBlockingHit && HitHeadResult.bBlockingHit)//头和胸都打到墙了,就是爬墙状态
	{
		if (ChestDistance <= 45.0f)
		{
			//修正角色距离墙的位置
			float CompensationValue = ChestDistance - 30.0f;
			if (CompensationValue > 0.0f)
			{
				FVector TargetPoint = ForwardDirection * CompensationValue * (DeltaTime * 8);
				TargetPoint += MMORPGCharacterBase->GetActorLocation();
				MMORPGCharacterBase->SetActorLocation(TargetPoint);
			}

			//攀爬状态
			if (ClimbState == EClimbState::CLIMB_CLIMBING)//如果当前攀爬状态已经是正在攀爬状态
			{
				if (HitGroundResult.bBlockingHit)//如果检测到落地了
				{
					if (GroundDistance <= 1.0f)
					{
						ClimbState = EClimbState::CLIMB_GROUND;
						ReleaseClimeb();
					}
					else if(HitRightResult.bBlockingHit || HitLeftResult.bBlockingHit || HitRightForwardResult.bBlockingHit || HitLeftForwardResult.bBlockingHit)
					{//检测是否旋转攀爬
						if (!bTurn)
						{
							CheckTurn();
						}
					}
				}
				else if (HitRightResult.bBlockingHit || HitLeftResult.bBlockingHit || HitRightForwardResult.bBlockingHit || HitLeftForwardResult.bBlockingHit)
				{//检测是否旋转攀爬
					if (!bTurn)
					{
						CheckTurn();
					}
				}

				//修复攀爬圆顶时没有站立的BUG
				FVector ZAxis = FVector(0.0f, 0.0f, 1.0f);
				float CosValue = FVector::DotProduct(ZAxis, HitChestResult.ImpactNormal);
				float CosAngle = (180.0f) / PI * FMath::Acos(CosValue);
				if (CosAngle < 35.0f)
				{
					ClimbState = EClimbState::CLIMB_NONE;
					ReleaseClimeb();
				}
			}
			else if(ClimbState != EClimbState::CLIMB_GROUND && ClimbState != EClimbState::CLIMB_DROP)//如果当前攀爬状态不是落地状态且不是下落状态
			{
				ClimbState = EClimbState::CLIMB_CLIMBING;
				SetClimbing();
			}
		}
		else
		{
			//如果距离墙远了
			if (ClimbState == EClimbState::CLIMB_GROUND)//如果是落地状态且距离较远
			{
				ClimbState = EClimbState::CLIMB_NONE;
			}
		}

		if (!HitGroundResult.bBlockingHit)
		{
			if (bJumpToClimb)
			{
				bJumpToClimb = false;
			}
			else
			{
				bJumpToClimb = true;
			}
		}
		else
		{
			bJumpToClimb = false;
		}
	}
	else if (HitChestResult.bBlockingHit && !HitHeadResult.bBlockingHit)//胸打到墙了,头没有
	{
		if (ClimbState == EClimbState::CLIMB_CLIMBING && !HitGroundResult.bBlockingHit)//攀爬中到顶了
		{
			ClimbState = EClimbState::CLIMB_TOTOP;

			GThread::Get()->GetCoroutines().BindLambda(1.0f, [this]()
				{
					ReleaseClimeb();
				});
		}
		else if (ClimbState != EClimbState::CLIMB_TOTOP 
			&& ClimbState != EClimbState::CLIMB_THROWOVER
			&& !bThrowOver)//不是攀爬状态，且不是到顶，说明是翻越状态
		{
			if (ChestDistance <= 18.0f)
			{
				//1、面向墙
				{
					FRotator NewRot = FRotationMatrix::MakeFromX(MMORPGCharacterBase->GetActorForwardVector() - HitChestResult.Normal).Rotator();

					ActorRotation.Yaw = NewRot.Yaw;
					ActorRotation.Pitch = 0.0f;
					ActorRotation.Roll = 0.0f;
					MMORPGCharacterBase->SetActorRotation(ActorRotation);
				}

				//从角色头顶前方40处向下打射线
				FHitResult HitThrowOverResult;

				FVector StartTraceLocation = Location + ForwardDirection * 40.0f;
				StartTraceLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight();
				FVector EndTraceLocation = StartTraceLocation - UpDirection * CapsuleComponent->GetScaledCapsuleHalfHeight() * 2.0f;
				TArray<AActor*> ThrowOverActorsToIgnore;

				UKismetSystemLibrary::LineTraceSingle(
					GetWorld(),
					StartTraceLocation,
					EndTraceLocation,
					ETraceTypeQuery::TraceTypeQuery1,
					false,
					ThrowOverActorsToIgnore,
					EDrawDebugTrace::ForOneFrame,
					HitThrowOverResult,
					true
				);

				if (HitThrowOverResult.bBlockingHit)
				{
					HitThrowOverResult.Location.Z += CapsuleComponent->GetScaledCapsuleHalfHeight();//修正翻越时的距离

					ThrowOverPoint = HitThrowOverResult.Location;

					ThrowOverHeight = HitThrowOverResult.Distance;

					ClimbState = EClimbState::CLIMB_THROWOVER;

					AdjustmentClimbing();//播放翻越动画时提高角色高度,防止下陷

					if (IsLowThrowOver())
					{
						bThrowOver = 0.8f;
					}
					else
					{
						bThrowOver = 1.7f;
					}

					bThrowOver = true;
				}
			}
		}
	}
	else if (!HitChestResult.bBlockingHit && !HitHeadResult.bBlockingHit)//头和胸都没有打到，就是NONE
	{
		if (ClimbState == EClimbState::CLIMB_CLIMBING)
		{
			ClimbState = EClimbState::CLIMB_NONE;
			ReleaseClimeb();
		}
	}

	//攀爬曲面
	if (HitChestResult.bBlockingHit)
	{
		if (ClimbState == EClimbState::CLIMB_CLIMBING)
		{
			FRotator NewRot = FRotationMatrix::MakeFromX(MMORPGCharacterBase->GetActorForwardVector() - HitChestResult.Normal).Rotator();

			ActorRotation.Yaw = NewRot.Yaw;
			ActorRotation.Pitch = NewRot.Pitch;
			ActorRotation.Roll = 0.0f;
			MMORPGCharacterBase->SetActorRotation(ActorRotation);
		}
	}

	//翻越,将角色平滑的移动到墙上
	if (bThrowOver)
	{
		FVector VInterpToLocation = FMath::VInterpTo(Location, ThrowOverPoint, DeltaTime, 27.0f);
		MMORPGCharacterBase->SetActorLocation(VInterpToLocation);
	}

	if (ClimbState == EClimbState::CLIMB_DROP)
	{
		if (CharacterMovementComponent->MovementMode != EMovementMode::MOVE_Falling)
		{
			ClearClimbingState();
		}
	}
}

void UClimbComponent::SetClimbing()
{
	SetClimbState(EMovementMode::MOVE_Custom, ECharacterActionState::CLIMB_STATE, false);
}

void UClimbComponent::ReleaseClimeb()
{
	SetClimbState(EMovementMode::MOVE_Walking, ECharacterActionState::NORMAL_STATE, true);
}

void UClimbComponent::ClearClimbingState()
{
	ClimbState = EClimbState::CLIMB_NONE;
}

bool UClimbComponent::IsLowThrowOver()
{
	return ThrowOverHeight > 40.0f;
}

void UClimbComponent::AdjustmentClimbing(bool bStart /*= true*/)
{
	FVector RelativeLocation = MMORPGCharacterBase->GetMesh()->GetRelativeLocation();//获取相对位置
	float AdjustValue = 5.0f;
	if (bStart)
	{
		RelativeLocation.Z += AdjustValue;
	}
	else
	{
		RelativeLocation.Z -= AdjustValue;
	}
	MMORPGCharacterBase->GetMesh()->SetRelativeLocation(RelativeLocation);
}

void UClimbComponent::SetClimbState(EMovementMode InMovementMode, ECharacterActionState InCharacterActionState, bool bOrientRotationToMovement)
{
	FRotator InActorRotation = MMORPGCharacterBase->GetActorRotation();

	CharacterMovementComponent->SetMovementMode(InMovementMode);
	CharacterMovementComponent->bOrientRotationToMovement = bOrientRotationToMovement;
	MMORPGCharacterBase->ResetActionState(InCharacterActionState);

	InActorRotation.Pitch = 0.0f;
	MMORPGCharacterBase->SetActorRotation(InActorRotation);
	bJumpToClimb = false;
}

void UClimbComponent::LaunchCharacter(const FVector& LaunchVelocity)
{
	PendingLaunchVelocity = LaunchVelocity;
}

void UClimbComponent::DropClimbState()
{
	ClimbState = EClimbState::CLIMB_DROP;
}

bool UClimbComponent::IsDropClimbState()
{
	return ClimbState == EClimbState::CLIMB_DROP;
}

void UClimbComponent::ResetClimbState()
{
	ClimbState = EClimbState::CLIMB_CLIMBING;
}

float UClimbComponent::Scanning(FHitResult& HitResult, TFunction<void(FVector&, FVector&)> TraceLocation)
{
	float Distance = MaxDistance;

	FVector StartTraceLocation, EndTraceLocation;
	TraceLocation(StartTraceLocation, EndTraceLocation);

	TArray<AActor*> ActorsToIgnore;
	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		StartTraceLocation,
		EndTraceLocation,
		ETraceTypeQuery::TraceTypeQuery1,
		true,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		Distance = FVector::Distance(StartTraceLocation, HitResult.Location);
	}

	return Distance;
}

void UClimbComponent::AdjustPendingLaunchVelocity(float DeltaTime)
{
	auto AxisCheck = [](double& InValue, float DeltaTime)
		{
			if (FMath::IsNearlyEqual(InValue, 0, 1))
			{
				InValue = 0;
			}
			else if (InValue > 0)
			{
				InValue -= InValue * DeltaTime;
			}
			else if(InValue < 0)
			{
				InValue += -InValue * DeltaTime;
			}
		};

	AxisCheck(PendingLaunchVelocity.X, DeltaTime);
	AxisCheck(PendingLaunchVelocity.Y, DeltaTime);
	AxisCheck(PendingLaunchVelocity.Z, DeltaTime);

	Print(DeltaTime, PendingLaunchVelocity.ToString());
}
