// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_PartnerInfo.h"
#include "Components/Image.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "UI_PartnerList.h"

UUI_PartnerInfo::UUI_PartnerInfo(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	CharacterID(INDEX_NONE)
{

}

void UUI_PartnerInfo::NativeConstruct()
{
	Super::NativeConstruct();

	ClickedCheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_PartnerInfo::OnClickedCharacter);
}

void UUI_PartnerInfo::NativeDestruct()
{
	Super::NativeDestruct();


}

void UUI_PartnerInfo::ShowSelected(bool bShow)
{
	ClickedCheckBox->SetCheckedState(bShow == true ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void UUI_PartnerInfo::OnClickedCharacter(bool bClicked)
{
	if (UUI_PartnerList* InList = GetParents<UUI_PartnerList>())
	{
		InList->CallCharacterListByPredicate([&](UUI_PartnerInfo* InInfo)->bool
			{
				if (InInfo->GetCharacterID() == CharacterID)
				{
					InInfo->ShowSelected(bClicked);
				}
				else
				{
					InInfo->ShowSelected(!bClicked);
				}
				return true;
			});
	}
}