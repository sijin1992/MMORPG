#include "Core/SimpleZoom.h"

namespace SimpleActorAction
{
	SimpleActorAction::FSimpleZoom::FSimpleZoom()
		:Distance(INDEX_NONE)
	{

	}

	void SimpleActorAction::FSimpleZoom::Register(AActor* InTarget, float InDistance)
	{
		Target = InTarget;
		Distance = InDistance;
		Location = InTarget->GetActorLocation();
	}

	void FSimpleZoom::ZoomPositive(float InValue)
	{
		if (Target.IsValid())
		{
			float Max = Location.X + Distance / 2.0f;
			FVector NewLocation = Target->GetActorLocation();
			NewLocation.X += InValue;
			if (NewLocation.X < Max)
			{
				Target->SetActorLocation(NewLocation);
			}
		}
	}

	void FSimpleZoom::ZoomNegative(float InValue)
	{
		if (Target.IsValid())
		{
			float Min = Location.X - Distance / 2.0f;
			FVector NewLocation = Target->GetActorLocation();
			NewLocation.X += InValue;
			if (NewLocation.X > Min)
			{
				Target->SetActorLocation(NewLocation);
			}
		}
	}

}
