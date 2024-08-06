#include "MethodUnit.h"

namespace MethodUnit 
{
	template<class T>
	void ServerCallAllPlayerController(UWorld* NewWorld, TFunction<EServerCallType(T*)> InImplement)
	{
		for (FConstPlayerControllerIterator It = NewWorld->GetPlayerControllerIterator(); It; ++It)
		{
			if (T* InPlayerController = Cast<T>(It->Get()))
			{
				//服务器函数是否已经完成
				if (InImplement(InPlayerController) == EServerCallType::PROGRESS_COMPLETE)
				{
					break;
				}
			}
		}
	}

	template<class T>
	void ServerCallAllPlayer(UWorld* NewWorld, TFunction<EServerCallType(T*)> InImplement)
	{
		ServerCallAllPlayerController<APlayerController>(NewWorld, [&](APlayerController* InPlayerController)->EServerCallType
		{
				if (T* InPawn = Cast<T>(InPlayerController->GetPawn()))
				{
					return InImplement(InPawn);
				}

				return EServerCallType::INPROGRESS;
		});
	}
}