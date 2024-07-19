#include "Core/SimpleBrowse.h"

namespace SimpleActorAction
{
	FSimpleBrowse::FSimpleBrowse()
		:bRotateCharacter(false),
		Controller(NULL),
		Target(NULL)
	{

	}

	FSimpleBrowse::FSimpleBrowse(APlayerController* InController, AActor* InTarget)
		:bRotateCharacter(false),
		Controller(InController),
		Target(InTarget)
	{
	}

	void FSimpleBrowse::Register(APlayerController* InController, AActor* InTarget)
	{
		Controller = InController;
		Target = InTarget;
	}

	void SimpleActorAction::FSimpleBrowse::Tick(float DeltaTime)
	{
		if (Controller && Target.IsValid())
		{
			//如果开启了，就判断角色是否旋转
			if (bRotateCharacter)
			{
				FRotator CurrentRot = Target->GetActorRotation();

				FVector2D NewMousePos;
				Controller->GetMousePosition(NewMousePos.X, NewMousePos.Y);
				float RotSpeed = (NewMousePos.X - MousePos.X) * -1.0f;//注意这里要乘以-1

				CurrentRot.Yaw += RotSpeed * 1.0f;
				Target->SetActorRotation(CurrentRot);

				MousePos = NewMousePos;
			}
			else
			{
				Controller->GetMousePosition(MousePos.X, MousePos.Y);
			}
		}
	}

	TStatId SimpleActorAction::FSimpleBrowse::GetStatId() const
	{
		return TStatId();
	}

	void FSimpleBrowse::OpenBrowsing()
	{
		bRotateCharacter = true;
	}

	void FSimpleBrowse::EndBrowsing()
	{
		bRotateCharacter = false;
	}

	void FSimpleBrowse::ResetTarget(AActor* InNewTarget)
	{
		Target = InNewTarget;
	}

}