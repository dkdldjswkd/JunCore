#ifndef __WORLDSOCKET_H__
#define __WORLDSOCKET_H__

#include <boost/asio/ip/tcp.hpp>
#include <Common.h>
#include <Networking/Socket.h>
#include <Server/WorldPacket.h>
#include <mutex>
#include "WorldSession.h"

using boost::asio::ip::tcp;
class WorldSocket : public Socket<WorldSocket>
{
	using BaseSocket = Socket<WorldSocket>;

public:
    WorldSocket(tcp::socket&& socket);
    ~WorldSocket();

    WorldSocket(const WorldSocket& right) = delete;
    WorldSocket& operator=(const WorldSocket& right) = delete;

    void Start() override;
    bool Update() override;

    void SendPacket(WorldPacket const& packet);

    void SetSendBufferSize(std::size_t sendBufferSize) { _sendBufferSize = sendBufferSize; }

private:
    std::array<uint8, 4> _authSeed;
    // AuthCrypt _authCrypt;

    // TimePoint _LastPingTime;
    uint32 _OverSpeedPings;

    std::mutex _worldSessionLock;
    WorldSession* _worldSession;
    bool _authed;

    MessageBuffer _headerBuffer;
    MessageBuffer _packetBuffer;
    std::size_t _sendBufferSize;

    std::string _ipCountry;
};

#endif