#ifndef SocketMgr_h__
#define SocketMgr_h__

#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include "Define.h"
#include "AsyncAcceptor.h"
#include "NetworkThread.h"

using boost::asio::ip::tcp;

template<class SocketType>
class SocketMgr
{
protected:
    SocketMgr() : _acceptor(nullptr), _threads(nullptr), _threadCount(0)
    {
    }

public:
    virtual ~SocketMgr()
    {
        // todo
        // ASSERT(!_threads && !_acceptor && !_threadCount, "StopNetwork must be called prior to SocketMgr destruction");
    }

protected:
    virtual NetworkThread<SocketType>* CreateThreads() const = 0;

    AsyncAcceptor* _acceptor;
    NetworkThread<SocketType>* _threads;
    int32 _threadCount;
};

#endif