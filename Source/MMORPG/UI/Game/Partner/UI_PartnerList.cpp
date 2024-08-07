// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_PartnerList.h"
#include "Components/VerticalBox.h"
#include "UI_PartnerInfo.h"
#include "Components/VerticalBoxSlot.h"

void UUI_PartnerList::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

void UUI_PartnerList::NativeDestruct()
{
	Super::NativeDestruct();


}

void UUI_PartnerList::Init()
{
	if (UI_PartnerInfoClass)
	{
		for (int32 i = 1; i < 3; i++)
		{
			if (UUI_PartnerInfo* InPartner = CreateWidget<UUI_PartnerInfo>(GetWorld(),UI_PartnerInfoClass))
			{
				if (i == 1)
				{
					InPartner->ShowSelected(true);
				}

				InPartner->SetParents(this);
				if (UVerticalBoxSlot* InBoxSlot = Cast<UVerticalBoxSlot>(CharacterList->AddChildToVerticalBox(InPartner)))
				{
					InBoxSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 10.0f));
					InPartner->SetCharacterID(i);
				}
			}
		}
	}
}

void UUI_PartnerList::CallCharacterListByPredicate(TFunction<bool(UUI_PartnerInfo* Info)> InFun)
{
	for (int32 i = 0; i < CharacterList->GetChildrenCount(); i++)
	{
		if (UUI_PartnerInfo* InInfo = Cast<UUI_PartnerInfo>(CharacterList->GetChildAt(i)))
		{
			if (!InFun(InInfo))
			{
				break;
			}
		}
	}
}
