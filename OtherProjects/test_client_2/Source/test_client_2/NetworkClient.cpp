#include "NetworkClient.h"
#include "Engine/Engine.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

NetworkClient::NetworkClient()
{
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    Socket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("NetworkClient"), false);
}

NetworkClient::~NetworkClient()
{
    Disconnect();
}

bool NetworkClient::Connect(const FString& ip, uint16 port) 
{
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    RemoteAddress = SocketSubsystem->CreateInternetAddr();

    bool bIsValid;
    RemoteAddress->SetIp(*ip, bIsValid);
    RemoteAddress->SetPort(port);

    if (!bIsValid)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid IP Address: %s"), *ip);
        return false;
    }

    if (Socket->Connect(*RemoteAddress))
    {
        UE_LOG(LogTemp, Log, TEXT("Connected to %s:%d"), *ip, port);
        return true;
    }

    UE_LOG(LogTemp, Error, TEXT("Failed to connect to %s:%d"), *ip, port);
    return false;
}

void NetworkClient::Disconnect()
{
    if (Socket)
    {
        Socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        UE_LOG(LogTemp, Log, TEXT("Disconnected"));
    }
}

bool NetworkClient::SendPacket(const TArray<uint8>& packet)
{
    int32 BytesSent = 0;
    return Socket->Send(packet.GetData(), packet.Num(), BytesSent);
}

void NetworkClient::ReceivePacket()
{
    uint8 Buffer[1024];
    int32 BytesRead = 0;

    if (Socket->Recv(Buffer, sizeof(Buffer), BytesRead))
    {
        FString ReceivedData = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(Buffer)));
        UE_LOG(LogTemp, Log, TEXT("Received: %s"), *ReceivedData);
    }
}
