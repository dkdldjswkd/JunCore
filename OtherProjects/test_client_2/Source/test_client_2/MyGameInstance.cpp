#include "MyGameInstance.h"

void UMyGameInstance::Init()
{
    Super::Init();

    NetworkClient* Client = new NetworkClient();
    if (Client->Connect(TEXT("127.0.0.1"), 8085))
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully connected!"));
    }
}