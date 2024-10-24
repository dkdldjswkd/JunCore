#include "EchoSocketMgr.h"
#include <boost/system/error_code.hpp>

class EchoSocketThread : public NetworkThread<EchoSocket>
{
public:
    void SocketAdded(std::shared_ptr<EchoSocket> sock) override
    {
        sock->SetSendBufferSize(65536 /*sEchoSocketMgr.GetApplicationSendBufferSize()*/);
    }

    void SocketRemoved(std::shared_ptr<EchoSocket> sock) override
    {
        // ...
    }
};

EchoSocketMgr& EchoSocketMgr::Instance()
{
    static EchoSocketMgr instance;
    return instance;
}