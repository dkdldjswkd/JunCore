#ifndef __WORLDSOCKET_H__
#define __WORLDSOCKET_H__
//
//#include "Common.h"
//#include "AuthCrypt.h"
//#include "ServerPktHeader.h"
#include <Networking/Socket.h>
//#include "Util.h"
#include "WorldPacket.h"
#include "WorldSession.h"
//#include "MPSCQueue.h"
#include <boost/asio/ip/tcp.hpp>
#include <Define.h>
#include <mutex>

using boost::asio::ip::tcp;

class tmp_MPSCQueue // 임시 mpsc queue (추후 개선)
{
public:
    void Enqueue(WorldPacket _packet)
    {
        std::lock_guard<std::mutex> lock(_mutex);
		_queue.emplace(std::move(_packet));
    }

    bool Dequeue(OUT WorldPacket*& _packet) {
		std::lock_guard<std::mutex> lock(_mutex);
		if (_queue.empty())
			return false;

		_packet = &_queue.front();
		_queue.pop();
		return true;
    }

private:
    std::mutex _mutex;
	std::queue<WorldPacket> _queue;
};

class WorldSocket : public Socket<WorldSocket>
{
    typedef Socket<WorldSocket> BaseSocket;

public:
    WorldSocket(tcp::socket&& socket);
    ~WorldSocket();

    WorldSocket(WorldSocket const& right) = delete;
    WorldSocket& operator=(WorldSocket const& right) = delete;

    void Start() override;
    bool Update() override;

    void SendPacket(WorldPacket const& packet);

    void SetSendBufferSize(std::size_t sendBufferSize) { _sendBufferSize = sendBufferSize; }

protected:
    void OnClose() override;
    void ReadHandler() override;
    bool ReadHeaderHandler();

    enum class ReadDataHandlerResult
    {
        Ok = 0,
        Error = 1,
        WaitingForQuery = 2
    };

    ReadDataHandlerResult ReadDataHandler();

private:
    //void CheckIpCallback(PreparedQueryResult result);

    ///// writes network.opcode log
    ///// accessing WorldSession is not threadsafe, only do it when holding _worldSessionLock
    //void LogOpcodeText(OpcodeClient opcode, std::unique_lock<std::mutex> const& guard) const;
    ///// sends and logs network.opcode without accessing WorldSession
    //void SendPacketAndLogOpcode(WorldPacket const& packet);
    void HandleSendAuthSession();
    //void HandleAuthSession(WorldPacket& recvPacket);
    //void HandleAuthSessionCallback(std::shared_ptr<AuthSession> authSession, PreparedQueryResult result);
    //void LoadSessionPermissionsCallback(PreparedQueryResult result);
    //void SendAuthResponseError(uint8 code);

    //bool HandlePing(WorldPacket& recvPacket);

    //std::array<uint8, 4> _authSeed;
    //AuthCrypt _authCrypt;

    //TimePoint _LastPingTime;
    uint32 _OverSpeedPings;

    //std::mutex _worldSessionLock;
    WorldSession* _worldSession;
    bool _authed;

    MessageBuffer _headerBuffer;
    MessageBuffer _packetBuffer;
    tmp_MPSCQueue _bufferQueue; //MPSCQueue<EncryptablePacket, &EncryptablePacket::SocketQueueLink> _bufferQueue;
    std::size_t _sendBufferSize;

    //QueryCallbackProcessor _queryProcessor;
    //std::string _ipCountry;
};

//class EncryptablePacket : public WorldPacket
//{
//public:
//    EncryptablePacket(WorldPacket const& packet, bool encrypt) : WorldPacket(packet), _encrypt(encrypt)
//    {
//        SocketQueueLink.store(nullptr, std::memory_order_relaxed);
//    }
//
//    bool NeedsEncryption() const { return _encrypt; }
//
//    std::atomic<EncryptablePacket*> SocketQueueLink;
//
//private:
//    bool _encrypt;
//};
//
//namespace WorldPackets
//{
//    class ServerPacket;
//}
//#pragma pack(push, 1)
//
//struct ClientPktHeader
//{
//    uint16 size;
//    uint32 cmd;
//
//    bool IsValidSize() const { return size >= 4 && size < 10240; }
//    bool IsValidOpcode() const { return cmd < NUM_OPCODE_HANDLERS; }
//};
//
//#pragma pack(pop)
//
//struct AuthSession;

#endif
