// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Login.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "UI_LoginMain.h"
#include "Stream/SimpleIOStream.h"

void UUI_Login::NativeConstruct()
{
	Super::NativeConstruct();
	
	SignInButton->OnReleased.AddDynamic(this, &UUI_Login::SignIn);
	RegisterButton->OnReleased.AddDynamic(this, &UUI_Login::Register);
}

void UUI_Login::NativeDestruct()
{
	Super::NativeDestruct();
}

bool UUI_Login::EncryptionToLocal(const FString& InPaths)
{
	FString AccountString = Account->GetText().ToString();
	FString PasswordString = Password->GetText().ToString();

	//路径纠正
	FString InCypPaths = InPaths;
	FPaths::NormalizeFilename(InCypPaths);//将\\转换成/
	InCypPaths.RemoveFromEnd(TEXT("/"));//如果路径最后一个时/，将将其移除

	if (!AccountString.IsEmpty() && !PasswordString.IsEmpty() && !InPaths.IsEmpty())
	{
		//将FString转换成uin8字节流
		TArray<uint8> PasswordBuff;
		FSimpleIOStream Stream(PasswordBuff);

		Stream << PasswordString;

		//Hash加密
		FString HashKey = FMD5::HashAnsiString(*AccountString);
		//再Hash32次
		for (int32 i = 0; i < 32; i++)
		{
			HashKey = FMD5::HashAnsiString(*HashKey);
		}

		//判断密码位数够不够16位,不够的要凑够16的倍数
		if (!HashKey.IsEmpty())
		{
			int32 Remain = 0;					//余数
			int32 Total = PasswordBuff.Num();	//总位数
			if (Total % 16)						//如果不能被16整除
			{
				float Value = (float)Total / 16.0f;
				int32 Ceil = FMath::CeilToInt(Value);//进行向上取整

				int32 CeilTotal = Ceil * 16;
				Remain = CeilTotal - Total;
			}

			for (int32 i = 0; i < Remain; i++)
			{
				uint8 Temp = 0;
				Stream << Temp;
			}

			ANSICHAR* InKey = TCHAR_TO_UTF8(*HashKey);
			//AES加密
			FAES::EncryptData(PasswordBuff.GetData(), PasswordBuff.Num(), InKey);
			//删除目录：删除目录下所有的旧的账户和密码
			IFileManager::Get().DeleteDirectory(*InCypPaths);
			//储存
			FFileHelper::SaveArrayToFile(PasswordBuff, *(InCypPaths / AccountString + TEXT(".arpg")));

			return true;
		}
	}

	return false;
}

bool UUI_Login::DecryptionFromLocal(const FString& InPaths)
{
	//路径纠正
	FString InCypPaths = InPaths;
	FPaths::NormalizeFilename(InCypPaths);//将\\转换成/
	InCypPaths.RemoveFromEnd(TEXT("/"));//如果路径最后一个时/，将将其移除

	//查找文件列表
	TArray<FString> Filenames;
	IFileManager::Get().FindFilesRecursive(Filenames, *InPaths, TEXT("*.*"), true, false);
	//遍历文件列表
	for (auto& Temp : Filenames)
	{
		FString FilenameHeard = FPaths::GetCleanFilename(Temp);
		if (FilenameHeard.Contains(TEXT(".arpg")))
		{
			//D:\new_new_vide\111.arpg
			//读取密码
			TArray<uint8> InByte;
			FFileHelper::LoadFileToArray(InByte, *Temp);
			//移除后变成账户了
			FilenameHeard.RemoveFromEnd(TEXT(".arpg"));

			if (InByte.Num() > 0)
			{
				//对账户进行Hash加密算出密钥
				FString HashKey = FMD5::HashAnsiString(*FilenameHeard);
				//再Hash32次
				for (int32 i = 0; i < 32; i++)
				{
					HashKey = FMD5::HashAnsiString(*HashKey);
				}

				if (!HashKey.IsEmpty())
				{
					FSimpleIOStream Stream(InByte);

					//开始解密
					ANSICHAR* InKey = TCHAR_TO_UTF8(*HashKey);
					FAES::DecryptData(InByte.GetData(), InByte.Num(), InKey);

					FString PasswordString;
					Stream >> PasswordString;	//将密码从数据流里读出来

					//将账户和密码直接设置到UI里
					Account->SetText(FText::FromString(FilenameHeard));
					Password->SetText(FText::FromString(PasswordString));

					return true;
				}
			}
		}
	}
	return false;
}

void UUI_Login::SignIn()
{
	if (UUI_LoginMain* InLoginMain = GetParents<UUI_LoginMain>())
	{
		FString AccountString = Account->GetText().ToString();
		FString PasswordString = Password->GetText().ToString();
		InLoginMain->SignIn(AccountString, PasswordString);
	}
}

void UUI_Login::Register()
{
	if (UUI_LoginMain* InLoginMain = GetParents<UUI_LoginMain>())
	{
		InLoginMain->Register();
	}
}
