﻿#pragma once
#include "AsyncAcceptor.h"
#include "NetworkThread.h"
#include "Session.h"
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <thread>
#include <iostream>

using boost::asio::ip::tcp;

class NetworkManager
{
public:
	friend Session;

public:
	NetworkManager();
	virtual ~NetworkManager();

public:
	bool StartServer(std::string const& _bind_ip, uint16 _port, int _worker_cnt);
	void StopServer();

	// todo 수정 반드시 필요
	bool StartClient(int _worker_cnt);

private:
	void AsyncAccept();
	virtual void OnAccept(SessionPtr session_ptr);
	virtual void OnSessionClose(SessionPtr session_ptr);

public:
	template<typename T>
	void RegisterPacketHandler(int32 _packet_id, std::function<void(SessionPtr, const T&)> _packet_handle)
	{
		packet_handler_[_packet_id] = [_packet_handle](SessionPtr _session_ptr, const std::vector<char>& _serialized_packet) {
			T message;
			if (message.ParseFromArray(_serialized_packet.data(), _serialized_packet.size())) {
				_packet_handle(_session_ptr, message);
			}
		};
	}

	void HandlePacket(SessionPtr session, int32 packet_id, const std::vector<char>& _serialized_packet) 
	{
		auto it = packet_handler_.find(packet_id);
		if (it != packet_handler_.end())
		{
			it->second(session, _serialized_packet);
		}
		else
		{
			// LOG_ERROR
			std::cout << "HandlePacket Error!!!!" << std::endl;
		}
	}

	virtual void InitPacketHandlers() {}

public:
	void Connect(const tcp::endpoint& endpoint);
	virtual void OnConnect(SessionPtr session_ptr);

private:
	// acceptor
	AsyncAcceptor* acceptor_ = nullptr;
	boost::asio::io_context* io_context_ = nullptr; 
	std::thread accept_thread_;

	// worker
	std::vector<NetworkThread*> network_threads_;

	// packet handler
	std::unordered_map<int32, std::function<void(SessionPtr /*session ptr*/, const std::vector<char>&/*serialized packet*/)>> packet_handler_;
};