#include "EchoSocket.h"
#include "Protocol/ServerPktHeader.h"

using boost::asio::ip::tcp;

EchoSocket::EchoSocket(tcp::socket&& socket) : Socket(std::move(socket)), _OverSpeedPings(0), _authed(false), _sendBufferSize(4096)
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

void EchoSocket::ReadHandler()
{
    if (!IsOpen())
        return;

    MessageBuffer& packet = GetReadBuffer();
    while (packet.GetActiveSize() > 0)
    {
        if (_headerBuffer.GetRemainingSpace() > 0)
        {
            // need to receive the header
            std::size_t readHeaderSize = std::min(packet.GetActiveSize(), _headerBuffer.GetRemainingSpace());
            _headerBuffer.Write(packet.GetReadPointer(), readHeaderSize);
            packet.ReadCompleted(readHeaderSize);

            if (_headerBuffer.GetRemainingSpace() > 0)
            {
                // Couldn't receive the whole header this time.
                // ASSERT(packet.GetActiveSize() == 0);
                break;
            }

            // We just received nice new header
            if (!ReadHeaderHandler())
            {
                CloseSocket();
                return;
            }
        }

        // We have full read header, now check the data payload
        if (_packetBuffer.GetRemainingSpace() > 0)
        {
            // need more data in the payload
            std::size_t readDataSize = std::min(packet.GetActiveSize(), _packetBuffer.GetRemainingSpace());
            _packetBuffer.Write(packet.GetReadPointer(), readDataSize);
            packet.ReadCompleted(readDataSize);

            if (_packetBuffer.GetRemainingSpace() > 0)
            {
                // Couldn't receive the whole data this time.
                // ASSERT(packet.GetActiveSize() == 0);
                break;
            }
        }

        // just received fresh new payload
        ReadDataHandlerResult result = ReadDataHandler();
        _headerBuffer.Reset();
        if (result != ReadDataHandlerResult::Ok)
        {
            if (result != ReadDataHandlerResult::WaitingForQuery)
                CloseSocket();

            return;
        }
    }

    AsyncRead();
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