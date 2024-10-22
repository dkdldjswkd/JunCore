#ifndef __WORLDSOCKET_H__
#define __WORLDSOCKET_H__

#include <network/Socket.h>
#include "EchoPacket.h"
#include <boost/asio/ip/tcp.hpp>
#include <Define.h>
#include <mutex>

using boost::asio::ip::tcp;

class tmp_MPSCQueue
{
public:
    void Enqueue(EchoPacket _packet)
    {
        std::lock_guard<std::mutex> lock(_mutex);
		_queue.emplace(std::move(_packet));
    }

    bool Dequeue(OUT EchoPacket*& _packet) {
		std::lock_guard<std::mutex> lock(_mutex);
		if (_queue.empty())
			return false;

		_packet = &_queue.front();
		_queue.pop();
		return true;
    }

private:
    std::mutex _mutex;
	std::queue<EchoPacket> _queue;
};

class EchoSocket : public Socket<EchoSocket>
{
    typedef Socket<EchoSocket> BaseSocket;

public:
    EchoSocket(tcp::socket&& socket);
    ~EchoSocket();

    EchoSocket(EchoSocket const& right) = delete;
    EchoSocket& operator=(EchoSocket const& right) = delete;

    void Start() override;
    bool Update() override;

    void SendPacket(EchoPacket const& packet);

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
    void HandleSendAuthSession();
    uint32 _OverSpeedPings;
    bool _authed;

    MessageBuffer _headerBuffer;
    MessageBuffer _packetBuffer;
    tmp_MPSCQueue _bufferQueue; //MPSCQueue<EncryptablePacket, &EncryptablePacket::SocketQueueLink> _bufferQueue;
    std::size_t _sendBufferSize;
};
#endif
