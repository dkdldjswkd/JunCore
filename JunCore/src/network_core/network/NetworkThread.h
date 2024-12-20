﻿#pragma once
#include <boost/asio/ip/tcp.hpp>
#include <thread>
#include <mutex>
#include <shared_mutex>
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
	void Run();
	void Update();
	void Stop();
	void Wait();

public:
	int32 GetConnectionCount() const;
	void AddNewSession(SessionPtr _new_network_session_ptr);
	tcp::socket* GetSocketForAccept();
	tcp::socket* GetSocketForConnect();

private:
	std::atomic<int32> connections_;
	std::atomic<bool> stopped_;

	std::thread* thread_;

	SessionPtrVec active_session_vec_;

	std::shared_mutex new_session_lock_;
	SessionPtrVec new_session_vec_;

	boost::asio::io_context io_context_;
	tcp::socket accept_socket_;
	tcp::socket connect_socket_;
	boost::asio::steady_timer update_timer_;
};