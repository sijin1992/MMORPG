// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "../../MMORPGGameType.h"
#include "FlyComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMORPG_API UFlyComponent : public UMotionComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FlyAnimAttrubute")
	EDodgeFly DodgeFly;

	FResetBool bLand;//是否着地

public:	
	// Sets default values for this component's properties
	UFlyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void Landed(const FHitResult& InHit);//着陆的代理函数,这里弃用，因为这里是主动着陆才会触发，而飞行着陆是通过胶囊体碰撞地面实现的

	UFUNCTION()
	void LandHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ResetFly();

	void FlyForwardAxis(float InAxisValue);//计算轴向

	void ResetFastFly();

	void ResetDodgeFly(EDodgeFly InFlyState);//左右闪避飞行

	void Reset();
};
