#ifndef NetworkThread_h__
#define NetworkThread_h__

#include <boost/asio/ip/tcp.hpp>
#include <data_type.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <network/NetworkSession.h>

using boost::asio::ip::tcp;

class NetworkThread
{
public:
    NetworkThread() : connections_(0), stopped_(false), thread_(nullptr), io_context_(1), accept_socket_(io_context_), update_timer_(io_context_)
    {
    }

    virtual ~NetworkThread()
    {
        Stop();
        if (thread_)
        {
            Wait();
            delete thread_;
        }
    }

public:
    bool Start()
    {
        if (thread_)
            return false;

        thread_ = new std::thread(&NetworkThread::Run, this);
        return true;
    }

    void Stop()
    {
        stopped_ = true;
        io_context_.stop();
    }

    void Wait()
    {
        //ASSERT(_thread);

        thread_->join();
        delete thread_;
        thread_ = nullptr;
    }

    int32 GetConnectionCount() const
    {
        return connections_;
    }

    void AddNewSession(NetworkSessionPtr _new_network_session_ptr)
    {
        std::lock_guard<std::mutex> lock(new_session_lock);

        ++connections_;
        new_session_vec_.emplace_back(_new_network_session_ptr);
        // callback 고려
    }

    tcp::socket* GetSocketForAccept() { return &accept_socket_; }

protected:
    void Run()
    {
        // TC_LOG_DEBUG("misc", "Network Thread Starting");
        update_timer_.expires_from_now(boost::posix_time::milliseconds(1));
        update_timer_.async_wait([this](boost::system::error_code const&) { Update(); });
        io_context_.run();

        // TC_LOG_DEBUG("misc", "Network Thread exits");
        new_session_vec_.clear();
        active_session_vec_.clear();
    }

    void Update()
    {
        if (stopped_)
            return;

        update_timer_.expires_from_now(boost::posix_time::milliseconds(1));
        update_timer_.async_wait([this](boost::system::error_code const&) { Update(); });

        AddNewSockets();

        active_session_vec_.erase(
            std::remove_if(active_session_vec_.begin(), active_session_vec_.end()
                , [this](NetworkSessionPtr network_session_ptr)
				{
					if (network_session_ptr->Update() == false)
					{
						if (network_session_ptr->is_open())
							network_session_ptr->close_socket();

						// callbck 고려 SocketRemoved

						--this->connections_;
						return true;
					}

					return false;
				}
            )
            , active_session_vec_.end()
        );
	}

private:
    void AddNewSockets()
    {
        std::lock_guard<std::mutex> lock(new_session_lock);

        if (new_session_vec_.empty())
            return;

        for (auto _session : new_session_vec_)
        {
            if (!_session->is_open())
            {
                // callback 고려, SocketRemoved
                --connections_;
            }
            else
                active_session_vec_.push_back(_session);
        }

        new_session_vec_.clear();
    }

private:
    std::atomic<int32> connections_;
    std::atomic<bool> stopped_;

    std::thread* thread_;

    NetworkSessionPtrVec active_session_vec_;

    std::mutex new_session_lock;
    NetworkSessionPtrVec new_session_vec_;

    boost::asio::io_context io_context_;
    tcp::socket accept_socket_;
    DeadlineTimer update_timer_;
};

#endif