#include "EchoSocket.h"
#include "Protocol/ServerPktHeader.h"

using boost::asio::ip::tcp;

EchoSocket::EchoSocket(tcp::socket&& socket) : Socket(std::move(socket)), _OverSpeedPings(0), _authed(false), _sendBufferSize(4096)
{
}

EchoSocket::~EchoSocket() = default;

void EchoSocket::Start()
{
	AsyncRead();
	HandleSendAuthSession(); // accept session에게 connect noty를 한다.
}

bool EchoSocket::Update()
{
    EchoPacket* queued;
    if (_bufferQueue.Dequeue(queued))
    {
        // Allocate buffer only when it's needed but not on every Update() call.
        MessageBuffer buffer(_sendBufferSize);
        do
        {
			ServerPktHeader header(queued->size() + 2, queued->GetOpcode());

			// header 암호화
			//if (queued->NeedsEncryption())
			//    _authCrypt.EncryptSend(header.header, header.getHeaderLength());

			// buffer의 Size가 'Header + Packet'의 Size보다 작으면, buffer를 전송하고 새로운 buffer를 할당한다.
			if (buffer.GetRemainingSpace() < queued->size() + header.getHeaderLength())
            {
                QueuePacket(std::move(buffer));
                buffer.Resize(_sendBufferSize);
            }

			// buffer에 header와 packet을 쓴다.
            if (buffer.GetRemainingSpace() >= queued->size() + header.getHeaderLength())
            {
                buffer.Write(header.header, header.getHeaderLength());
                if (!queued->empty())
                {
					buffer.Write(queued->contents(), queued->size());
                }
            }
            else    // single packet larger than buffer size
            {
                MessageBuffer packetBuffer(queued->size() + header.getHeaderLength());
                packetBuffer.Write(header.header, header.getHeaderLength());
                if (!queued->empty())
                {
                    packetBuffer.Write(queued->contents(), queued->size());
                }

                QueuePacket(std::move(packetBuffer));
            }

            delete queued;
        } while (_bufferQueue.Dequeue(queued));

        if (buffer.GetActiveSize() > 0)
            QueuePacket(std::move(buffer));
    }

    if (!BaseSocket::Update())
        return false;

    //_queryProcessor.ProcessReadyCallbacks();
    return true;
}

void EchoSocket::HandleSendAuthSession()
{
    EchoPacket packet(0x1EC/*SMSG_AUTH_CHALLENGE*/, 40);
    //packet << uint32(1);                                    // 1...31
    //packet.append(_authSeed);

    //packet.append(Trinity::Crypto::GetRandomBytes<32>());               // new encryption seeds
   
    SendPacket(packet);  //SendPacketAndLogOpcode(packet);
}

void EchoSocket::OnClose()
{
    //{
    //    std::lock_guard<std::mutex> sessionGuard(_worldSessionLock);
    //    _worldSession = nullptr;
    //}
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

void EchoSocket::SendPacket(EchoPacket const& packet)
{
    if (!IsOpen())
        return;

    //if (sPacketLog->CanLogPacket())
    //    sPacketLog->LogPacket(packet, SERVER_TO_CLIENT, GetRemoteIpAddress(), GetRemotePort());

    _bufferQueue.Enqueue(packet); // _bufferQueue.Enqueue(new EncryptablePacket(packet, _authCrypt.IsInitialized()));
}