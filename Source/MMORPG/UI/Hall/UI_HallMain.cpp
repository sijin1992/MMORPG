// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_HallMain.h"
#include "../Common/UI_Print.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "../../MMORPGMacroType.h"
#include "Kismet/GameplayStatics.h"
#include "Element/UI_CharacterCreatePanel.h"
#include "Element/UI_RenameCreate.h"
#include "Protocol/HallProtocol.h"
#include "../../Core/Hall/HallPlayerState.h"
#include "Element/KneadFace/UI_EditorCharacter.h"
#include "../../Core/Hall/HallPawn.h"
#include "../../Core/Hall/Character/CharacterStage.h"
#include "Element/UI_KneadFace.h"

#define LOCTEXT_NAMESPACE "UUI_HallMain"

void UUI_HallMain::NativeConstruct()
{
	Super::NativeConstruct();
	HallMainIn();

	if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
	{
		LinkServer(InGameInstance->GetGateStatus().GateServerAddrInfo.Addr);
	}

	CAType = ECAType::CA_CREATE;
	
	UI_CharacterCreatePanel->SetParents(this);
	UI_RenameCreate->SetParents(this);
	UI_EditorCharacter->SetParents(this);
}

void UUI_HallMain::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_HallMain::PlayRenameIn()
{
	UI_RenameCreate->PlayWidgetAnim(TEXT("RenameIn"));
}

void UUI_HallMain::PlayRenameOut()
{
	UI_RenameCreate->PlayWidgetAnim(TEXT("RenameOut"));
}

void UUI_HallMain::ResetCharacterCreatePanel(bool bSpawnCharacter)
{
	UI_CharacterCreatePanel->CreateCharacterButtons();
	//高亮Button
	HighlightDefaultSelection();

	if (bSpawnCharacter)
	{
		//生成最新的对象
		SpawnRecentCharacter();
	}
}

void UUI_HallMain::SpawnRecentCharacter()
{
	if (AHallPlayerState* InPlayerState = GetPlayerState<AHallPlayerState>())
	{
		if (FMMORPGCharacterAppearance* InCAData = InPlayerState->GetRecentCharacter())
		{
			UI_CharacterCreatePanel->SpawnCharacter(InCAData);

			SetEditCharacter(InCAData);
		}
		else
		{
			SetEditCharacter(NULL);
			DestroyCharacter();
		}
	}
}

void UUI_HallMain::HighlightDefaultSelection()
{
	if (AHallPlayerState* InPlayerState = GetPlayerState<AHallPlayerState>())
	{
		if (FMMORPGCharacterAppearance* InCAData = InPlayerState->GetRecentCharacter())
		{
			UI_CharacterCreatePanel->HighlightSelection(InCAData->SlotPosition);
		}
	}
}

void UUI_HallMain::CheckReName(FString& InCharacterName)
{
	if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
	{
		//发送检查角色名字请求
		SEND_DATA(SP_CheckCharacterNameRequests, InGameInstance->GetUserData().ID, InCharacterName);
	}
}

void UUI_HallMain::CreateCharacter(const FMMORPGCharacterAppearance& InCA)
{
	if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
	{
		FString CAJson;
		if (CAType == ECAType::CA_EDITOR)
		{
			//发送编辑角色请求
			if (AHallPlayerState* InPlayerState = GetPlayerState<AHallPlayerState>())
			{
				if (FMMORPGCharacterAppearance* InNewCA = InPlayerState->GetCharacterCA(UI_RenameCreate->GetSlotPosition()))
				{
					InNewCA->Date = InCA.Date;
					InNewCA->Name = InCA.Name;
					NetDataAnalysis::CharacterAppearacnceToString(*InNewCA, CAJson);
				}
			}
			SEND_DATA(SP_EditorCharacterRequests, InGameInstance->GetUserData().ID, CAJson);
		}
		else
		{
			//发送创建角色请求
			NetDataAnalysis::CharacterAppearacnceToString(InCA, CAJson);
			SEND_DATA(SP_CreateCharacterRequests, InGameInstance->GetUserData().ID, CAJson);
		}
	}
}

void UUI_HallMain::SetSlotPosition(const int32 InSlotPos)
{
	UI_RenameCreate->SetSlotPosition(InSlotPos);
}

void UUI_HallMain::DeleteCharacter(int32 InSlot)
{
	if (InSlot >= 0 && InSlot < 4)
	{
		if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
		{
			//发送删除角色请求
			SEND_DATA(SP_DeleteCharacterRequests, InGameInstance->GetUserData().ID, InSlot);
		}
	}
}

void UUI_HallMain::SetEditCharacter(const FMMORPGCharacterAppearance* InCA)
{
	if (InCA)
	{
		UI_EditorCharacter->SetCharacterName(FText::FromString(InCA->Name));
		UI_EditorCharacter->SetSlotID(InCA->SlotPosition);
	}
	else
	{
		UI_EditorCharacter->SetCharacterName(FText::FromString(""));
		UI_EditorCharacter->SetSlotID(INDEX_NONE);
	}
}

void UUI_HallMain::DestroyCharacter()
{
	//删除刚刚的角色形象
	if (AHallPawn* InPawn = GetPawn<AHallPawn>())
	{
		if (InPawn->CharacterStage)
		{
			InPawn->CharacterStage->Destroy();
			InPawn->CharacterStage = nullptr;
		}
	}
}

void UUI_HallMain::EditCharacter(int32 InSlot)
{
	PlayRenameIn();
	SetSlotPosition(InSlot);
	
	if (UUI_KneadFace* InKneadFace = UI_CharacterCreatePanel->CreateKneadFace())
	{
		if (AHallPlayerState* InPlayerState = GetPlayerState<AHallPlayerState>())
		{
			if (FMMORPGCharacterAppearance* InCA = InPlayerState->GetCharacterCA(InSlot))
			{
				InKneadFace->InitKneadFace(InCA);
				StartCAData = *InCA;

				ResetEditorType();
				SetEditorCharacterPanelEnable(false);

				//设置名称
				UI_RenameCreate->SetEditableName(FText::FromString(InCA->Name));
			}
		}
	}
}

void UUI_HallMain::ResetEditorType()
{
	CAType = ECAType::CA_EDITOR;
}

void UUI_HallMain::ResetCreateType()
{
	CAType = ECAType::CA_CREATE;
}

void UUI_HallMain::ResetCharacterAppearance(FMMORPGCharacterAppearance* InCA)
{
	if (CAType == ECAType::CA_EDITOR)
	{
		if (InCA)
		{
			*InCA = StartCAData;
		}
	}
}

void UUI_HallMain::SetEditorCharacterPanelEnable(bool bEnable)
{
	UI_EditorCharacter->SetIsEnabled(bEnable);
}

void UUI_HallMain::JumpDSServer(int32 InSlotID)
{
	if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
	{
		//发送登录DS服务器色请求1
		SEND_DATA(SP_LoginToDSServerRequests, InGameInstance->GetUserData().ID, InSlotID);
	}
}

void UUI_HallMain::HallMainIn()
{
	PlayWidgetAnim(TEXT("HallMainIn"));
}

void UUI_HallMain::HallMainOut()
{
	PlayWidgetAnim(TEXT("HallMainOut"));
}

void UUI_HallMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber)
	{
	case SP_CharacterAppearanceResponses:
	{
		//接收角色数据
		FString CharacterJson;
		SIMPLE_PROTOCOLS_RECEIVE(SP_CharacterAppearanceResponses, CharacterJson);
		if (!CharacterJson.IsEmpty())
		{
			if (AHallPlayerState* InPlayerState = GetPlayerState<AHallPlayerState>())
			{
				//解析角色数据
				NetDataAnalysis::StringToFCharacterAppearacnce(CharacterJson, InPlayerState->GetCharacterAppearance());

				UI_CharacterCreatePanel->InitCharacterButtons(InPlayerState->GetCharacterAppearance());

				//生成最近使用的角色
				SpawnRecentCharacter();

				//让角色按钮高亮
				HighlightDefaultSelection();
			}
		}

		break;
	}
	case SP_CheckCharacterNameResponses:
	{
		//收到检查角色名字回调
		ECheckNameType CheckNameType = ECheckNameType::UNKNOWN_ERROR;
		SIMPLE_PROTOCOLS_RECEIVE(SP_CheckCharacterNameResponses, CheckNameType);
		//打印
		PrintLogByCheckNameType(CheckNameType);
		break;
	}
	case SP_CreateCharacterResponses:
	{
		//收到创建角色回调
		ECheckNameType CheckNameType = ECheckNameType::UNKNOWN_ERROR;
		bool bCreateCharacter = false;//是否创角成功
		FString JsonString;
		SIMPLE_PROTOCOLS_RECEIVE(SP_CreateCharacterResponses, CheckNameType, bCreateCharacter, JsonString);
		if (bCreateCharacter)
		{
			//创角成功
			PrintLog(LOCTEXT("CREATECHARACTERRESPONSES_SUCCESSFULLY", "created successfully."));

			FMMORPGCharacterAppearance InCA;
			NetDataAnalysis::StringToFCharacterAppearacnce(JsonString, InCA);
			if (AHallPlayerState* InPlayerState = GetPlayerState<AHallPlayerState>())
			{
				InPlayerState->AddCharacterCA(InCA);
				//播放命名界面退出动画
				PlayRenameOut();
				//重置角色列表界面
				ResetCharacterCreatePanel(false);

				//设置编辑角色界面
				SetEditCharacter(&InCA);
				SetEditorCharacterPanelEnable(true);
			}
		}
		else
		{
			PrintLog(LOCTEXT("CREATECHARACTERRESPONSES_FAIL", "created fail."));
			//延迟打印
			GThread::Get()->GetCoroutines().BindLambda(1.5f, [=]()
				{
					PrintLogByCheckNameType(CheckNameType);
				});
		}

		break;
	}
	case SP_LoginToDSServerResponses:
	{
		//收到登录DSServer回调10
		FSimpleAddr DsAddr;//DSServerAddrInfo

		SIMPLE_PROTOCOLS_RECEIVE(SP_LoginToDSServerResponses, DsAddr);

		FString DSAddrString = FSimpleNetManage::GetAddrString(DsAddr);
		//播放关闭大厅主界面动画
		HallMainOut();
		//跳转关卡
		GThread::Get()->GetCoroutines().BindLambda(1.f, [=]()
			{
				UGameplayStatics::OpenLevel(GetWorld(), *DSAddrString);
			});
		break;
	}
	case SP_DeleteCharacterResponses:
	{
		//收到删除角色回调
		int32 InUserID = INDEX_NONE;
		int32 SlotID = INDEX_NONE;
		int32 SuccessDeleteCount = 0;
		SIMPLE_PROTOCOLS_RECEIVE(SP_DeleteCharacterResponses, InUserID, SlotID, SuccessDeleteCount);
		if (SuccessDeleteCount > 2)
		{
			if (AHallPlayerState* InPlayerState = GetPlayerState<AHallPlayerState>())
			{
				InPlayerState->RemoveCharacterAppearanceBySlot(SlotID);

				ResetCharacterCreatePanel(true);
			}

			PrintLog(LOCTEXT("DELETE_CHARACTER_SUCCESS", "The role deletion is successful, and the change operation is irreversible."));
		}
		else
		{
			PrintLog(LOCTEXT("DELETE_CHARACTER_ERROR", " Failed to delete the role. Please check if the role exists."));
		}
		break;
	}
	case SP_EditorCharacterResponses:
	{
		//收到编辑角色回调
		FSimpleAddrInfo AddrInfo;
		bool bUpdateSucceeded = false;//是否编辑角色成功
		SIMPLE_PROTOCOLS_RECEIVE(SP_EditorCharacterResponses, bUpdateSucceeded);

		if (bUpdateSucceeded)
		{
			//角色编辑成功
			PrintLog(LOCTEXT("EDITORCHARACTERRESPONSES_SUCCESSFULLY", "Edit character successfully."));
		}
		else
		{
			PrintLog(LOCTEXT("EDITORCHARACTERRESPONSES_FAIL", "Edit character fail."));
		}
		PlayRenameOut();
		ResetCharacterCreatePanel(false);
		SetEditorCharacterPanelEnable(true);
		break;
	}
	}
}

void UUI_HallMain::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{
	if (InType == ESimpleNetErrorType::HAND_SHAKE_SUCCESS)//如果握手成功
	{
		if (UMMORPGGameInstance* InGameInstance = GetGameInstance<UMMORPGGameInstance>())
		{
			//发送获取角色信息请求
			SEND_DATA(SP_CharacterAppearanceRequests, InGameInstance->GetUserData().ID);
		}
	}
}

void UUI_HallMain::PrintLogByCheckNameType(ECheckNameType InCheckNameType)
{
	switch (InCheckNameType)
	{
	case UNKNOWN_ERROR:
		PrintLog(LOCTEXT("CHECK_NAME_UNKNOWN_ERROR", "The server encountered an unknown error."));
		break;
	case NAME_NOT_EXIST:
		PrintLog(LOCTEXT("CHECK_NAME_NAME_NOT_EXIST", "The name is valid."));
		break;
	case SERVER_NOT_EXIST:
		PrintLog(LOCTEXT("CHECK_NAME_SERVER_NOT_EXIST", "Server error."));
		break;
	case NAME_EXIST:
		PrintLog(LOCTEXT("CHECK_NAME_NAME_EXIST", "The name has been registered."));
		break;
	}
}

#undef LOCTEXT_NAMESPACE