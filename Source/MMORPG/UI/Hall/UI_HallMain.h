// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "SimpleNetChannelType.h"
#include "MMORPGType.h"
#include "UI_HallMain.generated.h"

class FSimpleChannel;
class UUI_Print;
class UUI_CharacterCreatePanel;
class UUI_RenameCreate;
class UUI_EditorCharacter;
/**
 * 
 */
UCLASS()
class MMORPG_API UUI_HallMain : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UUI_Print* UI_Print;									//Log界面

	UPROPERTY(meta = (BindWidget))
	UUI_CharacterCreatePanel* UI_CharacterCreatePanel;		//创建角色界面

	UPROPERTY(meta = (BindWidget))
	UUI_RenameCreate* UI_RenameCreate;						//命名界面

	UPROPERTY(meta = (BindWidget))
	UUI_EditorCharacter* UI_EditorCharacter;				//角色编辑界面

public:
	enum ECAType
	{
		CA_EDITOR,
		CA_CREATE,
	};

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

public:
	void PrintLog(const FString& InMsg);
	void PrintLog(const FText& InMsg);

	void PlayRenameIn();								//播放角色命名界面渐入动画
	void PlayRenameOut();								//播放角色命名界面渐出动画

	void ResetCharacterCreatePanel(bool bSpawnCharacter = true);//还原创角界面

	void SpawnRecentCharacter();						//生成最近游玩的角色

	void HighlightDefaultSelection();					//高亮显示默认角色

	void CheckReName(FString& InCharacterName);			//重命名检查

	void CreateCharacter(const FMMORPGCharacterAppearance& InCA);	//创建角色

	void SetSlotPosition(const int32 InSlotPos);

	void DeleteCharacter(int32 InSlot);					//删除角色

	void SetEditCharacter(const FMMORPGCharacterAppearance* InCA);//设置编辑角色界面信息

	void DestroyCharacter();							//删除角色舞台

	void EditCharacter(int32 InSlot);					//从编辑角色界面打开捏脸界面

	void ResetEditorType();								//设置捏脸界面为编辑模式

	void ResetCreateType();								//设置捏脸界面为创角模式

	void ResetCharacterAppearance(FMMORPGCharacterAppearance* InCA);		//当取消编辑角色时，需要重置当前编辑的角色形象

	void SetEditorCharacterPanelEnable(bool bEnable);	//设置编辑角色界面是否可编辑

	void JumpDSServer(int32 InSlotID);

protected:

	void HallMainIn();
	void HallMainOut();

protected:
	//循环绑定
	void BindClientRcv();

	//通信协议的代理函数
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel) override;

	/// <summary>
	/// 连接服务器信息
	/// </summary>
	/// <param name="InType">连接错误状态类型</param>
	/// <param name="InMsg"></param>
	UFUNCTION()
	void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg);

	void PrintLogByCheckNameType(ECheckNameType InCheckNameType);

private:
	//接收代理
	FDelegateHandle RecvDelegate;

	FMMORPGCharacterAppearance StartCAData;

	ECAType CAType;//描述玩家属于在大厅里面还是正在编辑角色
};
