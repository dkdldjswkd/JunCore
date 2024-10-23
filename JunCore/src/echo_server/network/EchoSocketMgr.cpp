#include "EchoSocketMgr.h"
#include <boost/system/error_code.hpp>

//static void OnSocketAccept(tcp::socket&& sock, uint32 threadIndex)
//{
//    sEchoSocketMgr.OnSocketOpen(std::forward<tcp::socket>(sock), threadIndex);
//}

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

EchoSocketMgr::EchoSocketMgr() : BaseSocketMgr(), _socketApplicationSendBufferSize(65536)
{
}

EchoSocketMgr& EchoSocketMgr::Instance()
{
    static EchoSocketMgr instance;
    return instance;
}

bool EchoSocketMgr::StartEchoNetwork(std::string const& bindIp, uint16 port, int threadCount)
{
    // set socket
    _socketApplicationSendBufferSize = 65536;   // sConfigMgr->GetIntDefault("Network.OutUBuff", 65536);

    return BaseSocketMgr::StartNetwork(bindIp, port, threadCount);
}

void EchoSocketMgr::StopNetwork()
{
    BaseSocketMgr::StopNetwork();
}