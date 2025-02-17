#pragma once

#include "CoreMinimal.h"
#include "Sockets.h"
#include "Networking.h"

class TEST_CLIENT_2_API NetworkClient
{
public:
    NetworkClient();
    ~NetworkClient();

    bool Connect(const FString& ip, uint16 port);
    void Disconnect();
    bool SendPacket(const TArray<uint8>& packet);
    void ReceivePacket();

private:
    void RecvHandler();

    FSocket* Socket;
    TSharedPtr<FInternetAddr> RemoteAddress;
};