#include "EchoSocket.h"
#include "Protocol/ServerPktHeader.h"

using boost::asio::ip::tcp;

EchoSocket::EchoSocket(tcp::socket&& socket) : NetworkSession(std::move(socket)), _OverSpeedPings(0), _authed(false), _sendBufferSize(4096)
{
}

EchoSocket::~EchoSocket() = default;


void EchoSocket::OnStart()
{
    // some...
}


void EchoSocket::OnClose()
{
    // some...
}

void EchoSocket::OnRecv()
{

}

bool EchoSocket::ReadHeaderHandler()
{
    // ASSERT(_headerBuffer.GetActiveSize() == sizeof(ClientPktHeader));

    //if (_authCrypt.IsInitialized())
    //    _authCrypt.DecryptRecv(_headerBuffer.GetReadPointer(), sizeof(ClientPktHeader));

    //ClientPktHeader* header = reinterpret_cast<ClientPktHeader*>(_headerBuffer.GetReadPointer());
    //EndianConvertReverse(header->size);
    //EndianConvert(header->cmd);

    //if (!header->IsValidSize() || !header->IsValidOpcode())
    //{
    //    // TC_LOG_ERROR("network", "EchoSocket::ReadHeaderHandler(): client {} sent malformed packet (size: {}, cmd: {})",
    //        GetRemoteIpAddress().to_string(), header->size, header->cmd);
    //    return false;
    //}

    //header->size -= sizeof(header->cmd);
    //_packetBuffer.Resize(header->size);
    return true;
}

EchoSocket::ReadDataHandlerResult EchoSocket::ReadDataHandler()
{
    return ReadDataHandlerResult::Ok;
}