#include "Core/Common/MMORPGGameInstance.h"

#define UE_MMORPG_DEBUG_DS 1

#if PLATFORM_WINDOWS
#define SEND_DATA(InProtocols,...) \
if (UMMORPGGameInstance* InGameInstanceMacro = GetGameInstance<UMMORPGGameInstance>()) \
{ \
	SIMPLE_CLIENT_SEND(InGameInstanceMacro->GetClient(), InProtocols,__VA_ARGS__); \
}
#else
#define SEND_DATA(InProtocols,args...) \
if (UMMORPGGameInstance* InGameInstanceMacro = GetGameInstance<UMMORPGGameInstance>()) \
{ \
	SIMPLE_CLIENT_SEND(InGameInstanceMacro->GetClient(), InProtocols,##args); \
}
#endif