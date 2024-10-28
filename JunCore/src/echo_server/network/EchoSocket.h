#ifndef __WORLDSOCKET_H__
#define __WORLDSOCKET_H__

#include <network/Socket.h>
#include <boost/asio/ip/tcp.hpp>
#include <Define.h>
#include <mutex>

using boost::asio::ip::tcp;

class EchoSocket : public Socket<EchoSocket>
{
    typedef Socket<EchoSocket> BaseSocket;

public:
    EchoSocket(tcp::socket&& socket);
    ~EchoSocket();

    EchoSocket(EchoSocket const& right) = delete;
    EchoSocket& operator=(EchoSocket const& right) = delete;

    void OnStart() override;

    void SetSendBufferSize(std::size_t sendBufferSize) { _sendBufferSize = sendBufferSize; }

protected:
    void OnClose() override;
    void OnRecv() override;
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
    std::size_t _sendBufferSize;
};
#endif
