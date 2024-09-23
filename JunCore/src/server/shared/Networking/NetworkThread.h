#ifndef NetworkThread_h__
#define NetworkThread_h__

#include <boost/asio/ip/tcp.hpp>
#include <data_type.h>
#include <thread>
#include <mutex>

using boost::asio::ip::tcp;

template<class SocketType>
class NetworkThread
{
private:
	using SocketContainer = std::vector<std::shared_ptr<SocketType>>;

public:
    NetworkThread() : _connections(0), _stopped(false), _thread(nullptr), _ioContext(1), _acceptSocket(_ioContext), _updateTimer(_ioContext)
    {
    }

    virtual ~NetworkThread()
    {
        Stop();
        if (_thread)
        {
            Wait();
            delete _thread;
        }
    }

public:
    void Stop()
    {
        _stopped = true;
        _ioContext.stop();
    }

    bool Start()
    {
        if (_thread)
            return false;

        _thread = new std::thread(&NetworkThread::Run, this);
        return true;
    }

    void Wait()
    {
        ASSERT(_thread);

        _thread->join();
        delete _thread;
        _thread = nullptr;
    }

    int32 GetConnectionCount() const
    {
        return _connections;
    }

    virtual void AddSocket(std::shared_ptr<SocketType> sock)
    {
        std::lock_guard<std::mutex> lock(_newSocketsLock);

        ++_connections;
        _newSockets.push_back(sock);
        SocketAdded(sock);
    }

    tcp::socket* GetSocketForAccept() { return &_acceptSocket; }

protected:
    virtual void SocketAdded(std::shared_ptr<SocketType> /*sock*/) { }
    virtual void SocketRemoved(std::shared_ptr<SocketType> /*sock*/) { }

    void AddNewSockets()
    {
        std::lock_guard<std::mutex> lock(_newSocketsLock);

        if (_newSockets.empty())
            return;

        for (std::shared_ptr<SocketType> sock : _newSockets)
        {
            if (!sock->IsOpen())
            {
                SocketRemoved(sock);
                --_connections;
            }
            else
                _sockets.push_back(sock);
        }

        _newSockets.clear();
    }

    void Run()
    {
        // TC_LOG_DEBUG("misc", "Network Thread Starting");

        _updateTimer.expires_from_now(boost::posix_time::milliseconds(1));
        _updateTimer.async_wait([this](boost::system::error_code const&) { Update(); });
        _ioContext.run();

        // TC_LOG_DEBUG("misc", "Network Thread exits");
        _newSockets.clear();
        _sockets.clear();
    }

    void Update()
    {
        if (_stopped)
            return;

        _updateTimer.expires_from_now(boost::posix_time::milliseconds(1));
        _updateTimer.async_wait([this](boost::system::error_code const&) { Update(); });

        AddNewSockets();

        _sockets.erase(
            // remove_if
            std::remove_if(_sockets.begin(), _sockets.end()
                , [this](std::shared_ptr<SocketType> sock)
				{
					if (!sock->Update())
					{
						if (sock->IsOpen())
							sock->CloseSocket();

						this->SocketRemoved(sock);

						--this->_connections;
						return true;
					}

					return false;
				}
            )
            // ~ end
            , _sockets.end()
        );
	}

private:
    std::atomic<int32> _connections;
    std::atomic<bool> _stopped;

    std::thread* _thread;

    SocketContainer _sockets;

    std::mutex _newSocketsLock;
    SocketContainer _newSockets;

    boost::asio::io_context _ioContext;
    tcp::socket _acceptSocket;
    DeadlineTimer _updateTimer;
};

#endif