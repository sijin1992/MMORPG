// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/MMORPGCharacterBase.h"
#include "InputActionValue.h"
#include "MMORPGCharacter.generated.h"


UCLASS(config=Game)
class AMMORPGCharacter : public AMMORPGCharacterBase
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	AMMORPGCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void Fly();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFly();
			
	void SwitchFight();

	UFUNCTION(Server, Reliable)
	void Fast();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFast();

	UFUNCTION(Server, Reliable)
	void DodgeLeft();//向左闪避飞行

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDodgeLeft();

	UFUNCTION(Server, Reliable)
	void DodgeRight();//向右闪避飞行

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDodgeRight();
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void OnRep_ActionStateChanged() override;

	void FightChanged();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE virtual class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

