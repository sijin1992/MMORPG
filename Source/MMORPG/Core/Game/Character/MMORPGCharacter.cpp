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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMMORPGCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMMORPGCharacter::Look);

	}

	PlayerInputComponent->BindAction("SwitchFight", IE_Pressed, this, &AMMORPGCharacter::SwitchFight);
	PlayerInputComponent->BindAction("Fly", IE_Pressed, this, &AMMORPGCharacter::Fly);
	PlayerInputComponent->BindAction("Fast", IE_Pressed, this, &AMMORPGCharacter::Fast);
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

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);

		if (ActionState == ECharacterActionState::FLIGHT_STATE)
		{
			GetFlyComponent()->FlyForwardAxis(MovementVector.Y);
		}
		else
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

void AMMORPGCharacter::Fly_Implementation()
{
	MulticastFly();
}

void AMMORPGCharacter::MulticastFly_Implementation()
{
	ResetActionState(ECharacterActionState::FLIGHT_STATE);

	GetFlyComponent()->ResetFly();
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

void AMMORPGCharacter::Fast()
{
	if (ActionState == ECharacterActionState::FLIGHT_STATE)
	{
		GetFlyComponent()->ResetFastFly();
	}
}

void AMMORPGCharacter::DodgeLeft()
{
	if (ActionState == ECharacterActionState::FLIGHT_STATE)
	{
		GetFlyComponent()->ResetDodgeFly(EDodgeFly::DODGE_LEFT);
	}
}

void AMMORPGCharacter::DodgeRight()
{
	if (ActionState == ECharacterActionState::FLIGHT_STATE)
	{
		GetFlyComponent()->ResetDodgeFly(EDodgeFly::DODGE_RIGHT);
	}
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