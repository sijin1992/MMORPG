// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMORPGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../../Component/FlyComponent.h"
#include "../../Component/SwimmingComponent.h"
#include "../../Component/ClimbComponent.h"


//////////////////////////////////////////////////////////////////////////
// AMMORPGCharacter

AMMORPGCharacter::AMMORPGCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMMORPGCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMMORPGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMMORPGCharacter::CharacterJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMMORPGCharacter::CharacterStopJump);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMMORPGCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMMORPGCharacter::Look);

	}

	PlayerInputComponent->BindAction("SlowDown", IE_Pressed, this, &AMMORPGCharacter::SlowDown);
	PlayerInputComponent->BindAction("SlowDown", IE_Released, this, &AMMORPGCharacter::SlowDownReleased);
	PlayerInputComponent->BindAction("SwitchFight", IE_Pressed, this, &AMMORPGCharacter::SwitchFight);
	PlayerInputComponent->BindAction("ActionSwitch", IE_Pressed, this, &AMMORPGCharacter::ActionSwitch);
	PlayerInputComponent->BindAction("Fast", IE_Pressed, this, &AMMORPGCharacter::Fast);
	PlayerInputComponent->BindAction("Fast", IE_Released, this, &AMMORPGCharacter::FastReleased);
	PlayerInputComponent->BindAction("DodgeLeft", IE_Pressed, this, &AMMORPGCharacter::DodgeLeft);
	PlayerInputComponent->BindAction("DodgeRight", IE_Pressed, this, &AMMORPGCharacter::DodgeRight);
}

void AMMORPGCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//左右
		if (ActionState == ECharacterActionState::CLIMB_STATE)
		{
			GetClimbComponent()->ClimbRightAxis(MovementVector.X);
		}
		else
		{
			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}

		//前后
		if (ActionState == ECharacterActionState::FLIGHT_STATE)
		{
			GetFlyComponent()->FlyForwardAxis(MovementVector.Y);
		}
		else if (ActionState == ECharacterActionState::SWIMMING_STATE && GetSwimmingComponent()->bDiving)
		{
			GetSwimmingComponent()->SwimForwardAxis(MovementVector.Y);
		}
		else if (ActionState == ECharacterActionState::CLIMB_STATE)
		{
			GetClimbComponent()->ClimbForwardAxis(MovementVector.Y);
		}
		else if (MovementVector.Y != 0.0f)
		{
			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
		}
	}
}

void AMMORPGCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMMORPGCharacter::SlowDown_Implementation()
{
	MulticastSlowDown();
}

void AMMORPGCharacter::MulticastSlowDown_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 190.0f;
}

void AMMORPGCharacter::SlowDownReleased_Implementation()
{
	MulticastSlowDownReleased();
}

void AMMORPGCharacter::MulticastSlowDownReleased_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AMMORPGCharacter::ActionSwitch_Implementation()
{
	MulticastActionSwitch();
}

void AMMORPGCharacter::MulticastActionSwitch_Implementation()
{
	if (UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent()))
	{
		if (CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Walking || CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Flying)
		{
			ResetActionState(ECharacterActionState::FLIGHT_STATE);

			GetFlyComponent()->ResetFly();
		}
		else if (CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Swimming)
		{
			GetSwimmingComponent()->GoUnderWater();
		}
		else if(CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Custom)	//取消攀爬
		{
			if (!GetClimbComponent()->IsDropClimbState())
			{
				GetClimbComponent()->ReleaseClimeb();
				GetClimbComponent()->DropClimbState();
				ClimbMontageChanged(EClimbMontageState::CLIMB_DROP);
				//施加向后的力
				FVector Dir = -GetActorForwardVector();

				GetClimbComponent()->LaunchCharacter(Dir * 1000.0f);
			}
		}
	}

}

void AMMORPGCharacter::SwitchFight()
{
	//客户端改变值
	ResetActionState(ECharacterActionState::FIGHT_STATE);
	//客户端先播放
	FightChanged();
	//通知服务器
	SwitchActionStateOnServer(ActionState);

	LastActionState = ActionState;
}

void AMMORPGCharacter::Fast_Implementation()
{
	MulticastFast();
}

void AMMORPGCharacter::FastReleased_Implementation()
{
	if (ActionState == ECharacterActionState::SWIMMING_STATE)
	{
		GetSwimmingComponent()->ResetFastSwim();
	}
	else if (ActionState == ECharacterActionState::NORMAL_STATE)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}

void AMMORPGCharacter::MulticastFast_Implementation()
{
	if (ActionState == ECharacterActionState::FLIGHT_STATE)
	{
		GetFlyComponent()->ResetFastFly();
	}
	else if (ActionState == ECharacterActionState::SWIMMING_STATE)
	{
		GetSwimmingComponent()->ResetFastSwim();
	}
	else if (ActionState == ECharacterActionState::NORMAL_STATE)
	{
		GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	}
}

void AMMORPGCharacter::DodgeLeft_Implementation()
{
	MulticastDodgeLeft();
}

void AMMORPGCharacter::MulticastDodgeLeft_Implementation()
{
	if (ActionState == ECharacterActionState::FLIGHT_STATE)
	{
		GetFlyComponent()->ResetDodgeFly(EDodgeFly::DODGE_LEFT);
	}
}

void AMMORPGCharacter::DodgeRight_Implementation()
{
	MulticastDodgeRight();
}

void AMMORPGCharacter::MulticastDodgeRight_Implementation()
{
	if (ActionState == ECharacterActionState::FLIGHT_STATE)
	{
		GetFlyComponent()->ResetDodgeFly(EDodgeFly::DODGE_RIGHT);
	}
}

void AMMORPGCharacter::CharacterJump()
{
	Jump();
	if (ActionState == ECharacterActionState::CLIMB_STATE)
	{
		GetClimbComponent()->ResetJump();
	}
}


void AMMORPGCharacter::CharacterStopJump()
{
	StopJumping();
}

void AMMORPGCharacter::FightChanged()
{
	if (FCharacterAnimTable* InAnimTable = GetAnimTable())
	{
		if (InAnimTable->SwitchFightMontage)
		{
			FName AnimSlotName = ActionState == ECharacterActionState::FIGHT_STATE ? TEXT("0") : TEXT("1");
			PlayAnimMontage(InAnimTable->SwitchFightMontage, 1.0f, AnimSlotName);
		}
	}
}

void AMMORPGCharacter::ClimbMontageChanged(EClimbMontageState InClimbMontageState)
{
	if (FCharacterAnimTable* InAnimTable = GetAnimTable())
	{
		if (InAnimTable->ClimbMontage)
		{
			PlayAnimMontage(InAnimTable->ClimbMontage, 1.0f, *FString::FromInt((int32)InClimbMontageState));
		}
	}
}

void AMMORPGCharacter::OnRep_ActionStateChanged()
{
	if (GetLocalRole() != ROLE_Authority)//如果是服务器就不执行
	{
		if (ActionState == ECharacterActionState::FIGHT_STATE ||
			LastActionState == ECharacterActionState::FIGHT_STATE)
		{
			FightChanged();
		}

		LastActionState = ActionState;
	}
}