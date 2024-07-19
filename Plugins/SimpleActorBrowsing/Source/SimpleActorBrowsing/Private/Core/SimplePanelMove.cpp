#include "Core/SimplePanelMove.h"

namespace SimpleActorAction
{
	SimpleActorAction::FSimplePanelMove::FSimplePanelMove()
		:bMove(false),
		Distance(INDEX_NONE)
	{

	}

	void SimpleActorAction::FSimplePanelMove::Register(APlayerController* InController, AActor* InTarget, float InDistance)
	{
		Controller = InController;
		Target = InTarget;
		Distance = InDistance;

		Location = InTarget->GetActorLocation();
	}

	void SimpleActorAction::FSimplePanelMove::Tick(float DeltaTime)
	{
		if (Controller && Target.IsValid())
		{
			if (bMove)
			{
				float Max = Location.Z + Distance / 2.0f;
				float Min = Location.Z - Distance / 2.0f;

				FVector CurrentPos = Target->GetActorLocation();

				FVector2D NewMousePos;
				Controller->GetMousePosition(NewMousePos.X, NewMousePos.Y);
				float MoveSpeed = (NewMousePos.Y - MousePos.Y) * -1.0f;//注意这里要乘以-1

				CurrentPos.Z += MoveSpeed * 1.0f;
				if (CurrentPos.Z < Max && CurrentPos.Z > Min)
				{
					Target->SetActorLocation(CurrentPos);
				}

				MousePos = NewMousePos;
			}
			else
			{
				Controller->GetMousePosition(MousePos.X, MousePos.Y);
			}
		}
	}

	TStatId SimpleActorAction::FSimplePanelMove::GetStatId() const
	{
		return TStatId();
	}

	void SimpleActorAction::FSimplePanelMove::BeginMove()
	{
		bMove = true;
	}

	void SimpleActorAction::FSimplePanelMove::EndMove()
	{
		bMove = false;
	}

	void SimpleActorAction::FSimplePanelMove::ResetTarget(AActor* InNewTarget)
	{
		Target = InNewTarget;
	}
}