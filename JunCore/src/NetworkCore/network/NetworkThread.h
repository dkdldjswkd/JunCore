#ifndef NetworkThread_h__
#define NetworkThread_h__

#include <boost/asio/ip/tcp.hpp>
#include <data_type.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <network/Session.h>

using boost::asio::ip::tcp;

class NetworkThread
{
public:
	NetworkThread();
	virtual ~NetworkThread();

public:
	bool Start();
	void Stop();
	void Wait();
	int32 GetConnectionCount() const;
	void AddNewSession(NetworkSessionPtr _new_network_session_ptr);
	tcp::socket* GetSocketForAccept();

protected:
	void Run();
	void Update();

private:
	void AddNewSockets();

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