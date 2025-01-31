// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../DataTable/CharacterAnimTable.h"
#include "CombatInterface/SimpleCombatInterface.h"
#include "../../../../MMORPGGameType.h"
#include "MMORPGCharacterBase.generated.h"

class UFlyComponent;
class USwimmingComponent;
class UClimbComponent;
class UCameraComponent;

UCLASS()
class MMORPG_API AMMORPGCharacterBase : public ACharacter, public ISimpleCombatInterface
{
	GENERATED_BODY()
	friend class AMMORPGGameMode;

	UPROPERTY(Category = MMORPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFlyComponent> FlyComponent;//飞行组件的对象指针

	UPROPERTY(Category = MMORPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USwimmingComponent> SwimmingComponent;//游泳组件的对象指针

	UPROPERTY(Category = MMORPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UClimbComponent> ClimbComponent;//攀爬组件的对象指针
public:
	// Sets default values for this character's properties
	AMMORPGCharacterBase();

	virtual void AnimSignal(int32 InSignal);

	virtual UCameraComponent* GetFollowCamera() const { return NULL; }

	virtual void ClimbMontageChanged(EClimbMontageState InClimbMontageState) {};

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName = "AnimSignal", Category = "Anim Event")
	void K2_AnimSignal(int32 InSignal);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//使用RPC需要重写这个方法告诉引擎同步方式
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Landed(const FHitResult& Hit) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE ECharacterActionState GetActionState() { return ActionState; }
	FORCEINLINE FCharacterAnimTable* GetAnimTable() { return AnimTable; }
	FORCEINLINE int32 GetID() { return ID; }
	FORCEINLINE int32 GetUserID() { return UserID; }

	//状态切换
	void ResetActionState(ECharacterActionState InNewActionState);

	//获取飞行组件
	FORCEINLINE UFlyComponent* GetFlyComponent() { return FlyComponent; }

	//获取游泳组件
	FORCEINLINE USwimmingComponent* GetSwimmingComponent() { return SwimmingComponent; }

	//获取攀爬组件
	FORCEINLINE UClimbComponent* GetClimbComponent() { return ClimbComponent; }
protected:
	//RPC服务器接口
	UFUNCTION(Server,Reliable)
	void SwitchActionStateOnServer(ECharacterActionState InActionState);
protected:
	//RPC同步
	UPROPERTY(ReplicatedUsing = OnRep_ActionStateChanged)
	ECharacterActionState ActionState;
	UPROPERTY()
	ECharacterActionState LastActionState;//上一次状态

	//RPC同步，绑定的函数
	UFUNCTION()
	virtual void OnRep_ActionStateChanged() {};

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	int32 ID;//角色ID
	UPROPERTY()
	int32 UserID;//用户ID

	FCharacterAnimTable* AnimTable;//角色动画
};
