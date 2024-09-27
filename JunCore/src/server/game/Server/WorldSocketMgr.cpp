//#include "Config.h"
//#include "NetworkThread.h"
//#include "ScriptMgr.h"
#include "WorldSocket.h"
#include "WorldSocketMgr.h"
#include <boost/system/error_code.hpp>

WorldSocketMgr::WorldSocketMgr() : BaseSocketMgr(), _socketSystemSendBufferSize(-1), _socketApplicationSendBufferSize(65536), _tcpNoDelay(true)
{
}

WorldSocketMgr& WorldSocketMgr::Instance()
{
    static WorldSocketMgr instance;
    return instance;
}

bool WorldSocketMgr::StartWorldNetwork(boost::asio::io_context& ioContext, std::string const& bindIp, uint16 port, int threadCount)
{
    _tcpNoDelay = false; // sConfigMgr->GetBoolDefault("Network.TcpNodelay", true); // 네이글 알고리즘 해제 보류

    int const max_connections = JUNCORE_MAX_LISTEN_CONNECTIONS;
    //TC_LOG_DEBUG("misc", "Max allowed socket connections {}", max_connections);

    // set socket
    _socketSystemSendBufferSize = -1; //  sConfigMgr->GetIntDefault("Network.OutKBuff", -1); // (-1 means use default)
    _socketApplicationSendBufferSize = 65536; // sConfigMgr->GetIntDefault("Network.OutUBuff", 65536);

    if (!BaseSocketMgr::StartNetwork(ioContext, bindIp, port, threadCount))
        return false;

    //_acceptor->AsyncAcceptWithCallback<&OnSocketAccept>();

    //sScriptMgr->OnNetworkStart();
    return true;
}

void WorldSocketMgr::StopNetwork()
{
    BaseSocketMgr::StopNetwork();

    // todo
    //sScriptMgr->OnNetworkStop();
}

// todo
void WorldSocketMgr::OnSocketOpen(tcp::socket&& sock, uint32 threadIndex)
{
    //// set some options here
    //if (_socketSystemSendBufferSize >= 0)
    //{
    //    boost::system::error_code err;
    //    sock.set_option(boost::asio::socket_base::send_buffer_size(_socketSystemSendBufferSize), err);
    //    if (err && err != boost::system::errc::not_supported)
    //    {
    //        TC_LOG_ERROR("misc", "WorldSocketMgr::OnSocketOpen sock.set_option(boost::asio::socket_base::send_buffer_size) err = {}", err.message());
    //        return;
    //    }
    //}

    // Set TCP_NODELAY.
    if (_tcpNoDelay)
    {
        boost::system::error_code err;
        sock.set_option(boost::asio::ip::tcp::no_delay(true), err);
        if (err)
        {
            //TC_LOG_ERROR("misc", "WorldSocketMgr::OnSocketOpen sock.set_option(boost::asio::ip::tcp::no_delay) err = {}", err.message());
            return;
        }
    }

    ////sock->m_OutBufferSize = static_cast<size_t> (m_SockOutUBuff);

    //BaseSocketMgr::OnSocketOpen(std::forward<tcp::socket>(sock), threadIndex);
}

NetworkThread<WorldSocket>* WorldSocketMgr::CreateThreads() const
{
    // todo

    return new NetworkThread<WorldSocket>(); // 아래 코드로 변경 필요
    // return new WorldSocketThread[GetNetworkThreadCount()];
}


//static void OnSocketAccept(tcp::socket&& sock, uint32 threadIndex)
//{
//    sWorldSocketMgr.OnSocketOpen(std::forward<tcp::socket>(sock), threadIndex);
//}
//
//class WorldSocketThread : public NetworkThread<WorldSocket>
//{
//public:
//    void SocketAdded(std::shared_ptr<WorldSocket> sock) override
//    {
//        sock->SetSendBufferSize(sWorldSocketMgr.GetApplicationSendBufferSize());
//        sScriptMgr->OnSocketOpen(sock);
//    }
//
//    void SocketRemoved(std::shared_ptr<WorldSocket> sock) override
//    {
//        sScriptMgr->OnSocketClose(sock);
//    }
//};
//
//