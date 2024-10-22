#include "EchoSocketMgr.h"
#include <boost/system/error_code.hpp>

static void OnSocketAccept(tcp::socket&& sock, uint32 threadIndex)
{
    sEchoSocketMgr.OnSocketOpen(std::forward<tcp::socket>(sock), threadIndex);
}

class EchoSocketThread : public NetworkThread<EchoSocket>
{
public:
    void SocketAdded(std::shared_ptr<EchoSocket> sock) override
    {
        sock->SetSendBufferSize(sEchoSocketMgr.GetApplicationSendBufferSize());
    }

    void SocketRemoved(std::shared_ptr<EchoSocket> sock) override
    {
        // ...
    }
};

EchoSocketMgr::EchoSocketMgr() : BaseSocketMgr(), _socketSystemSendBufferSize(-1), _socketApplicationSendBufferSize(65536), _tcpNoDelay(true)
{
}

EchoSocketMgr& EchoSocketMgr::Instance()
{
    static EchoSocketMgr instance;
    return instance;
}

bool EchoSocketMgr::StartEchoNetwork(boost::asio::io_context& ioContext, std::string const& bindIp, uint16 port, int threadCount)
{
    _tcpNoDelay = false; // sConfigMgr->GetBoolDefault("Network.TcpNodelay", true); // 네이글 알고리즘 해제 보류

    // set socket
    _socketSystemSendBufferSize      = -1;      // sConfigMgr->GetIntDefault("Network.OutKBuff", -1); // (-1 means use default)
    _socketApplicationSendBufferSize = 65536;   // sConfigMgr->GetIntDefault("Network.OutUBuff", 65536);

    if (!BaseSocketMgr::StartNetwork(ioContext, bindIp, port, threadCount))
        return false;

    _acceptor->AsyncAcceptWithCallback<&OnSocketAccept>();
    return true;
}

void EchoSocketMgr::StopNetwork()
{
    BaseSocketMgr::StopNetwork();
}

void EchoSocketMgr::OnSocketOpen(tcp::socket&& sock, uint32 threadIndex)
{
    // set some options here
    if (_socketSystemSendBufferSize >= 0)
    {
        boost::system::error_code err;
        sock.set_option(boost::asio::socket_base::send_buffer_size(_socketSystemSendBufferSize), err);
        if (err && err != boost::system::errc::not_supported)
        {
            // TC_LOG_ERROR("misc", "EchoSocketMgr::OnSocketOpen sock.set_option(boost::asio::socket_base::send_buffer_size) err = {}", err.message());
            return;
        }
    }

    // Set TCP_NODELAY.
    if (_tcpNoDelay)
    {
        boost::system::error_code err;
        sock.set_option(boost::asio::ip::tcp::no_delay(true), err);
        if (err)
        {
            //TC_LOG_ERROR("misc", "EchoSocketMgr::OnSocketOpen sock.set_option(boost::asio::ip::tcp::no_delay) err = {}", err.message());
            return;
        }
    }

    BaseSocketMgr::OnSocketOpen(std::forward<tcp::socket>(sock), threadIndex);
}

NetworkThread<EchoSocket>* EchoSocketMgr::CreateThreads() const
{
     return new EchoSocketThread[GetNetworkThreadCount()];
}