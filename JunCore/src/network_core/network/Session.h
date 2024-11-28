#pragma once
#include <common/define.h>
#include <vector>
#include <queue>
#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <common/MessageBuffer.h>
#include "../packet/PacketBuffer.h"

using boost::asio::ip::tcp;

#define READ_BLOCK_SIZE 4096

class NetworkThread;
class NetworkManager;

class Session : public std::enable_shared_from_this<Session>
{
	friend NetworkManager;
	friend NetworkThread;

public:
	Session(tcp::socket&& _socket, NetworkManager* _network_manager);
	~Session();

public:
	template <typename T>
	void SendPacket(int32 _packet_id, const T& _packet)
	{
		const int32 _payload_size = _packet.ByteSizeLong();

		PacketBufferPtr _packet_buffer_ptr = std::make_shared<PacketBuffer>();
		if (_packet_buffer_ptr->GetFreeSize() < _payload_size)
		{
			// LOG_ERROR or packet_buffer_ptr resize
			return;
		}

		_packet.SerializeToArray(_packet_buffer_ptr->GetWritePos(), _payload_size);
		_packet_buffer_ptr->MoveWp(_payload_size);
		_packet_buffer_ptr->SetHeader(_packet_id);

		{
			std::lock_guard<std::mutex> lock(send_queue_lock_);

			send_queue_.push(_packet_buffer_ptr);
		}
	}

public:
	// addr
	boost::asio::ip::address GetRemoteIpAddress() const;
	uint16 GetRemotePort() const;

private:
	// NetworkThread, network_core에서 사용
	void Start();	// Recv Handler 등록
	bool Update();	// Send Queue 처리
	void CloseSocket();
	bool IsOpen() const;

private:
	// Recv 처리
	void AsyncRecv();
	void ReadHandler(boost::system::error_code error, size_t transferredBytes);
	MessageBuffer& GetRecvBuffer();

private:
	// Send 처리
	bool ProcessSendQueue();
	void ScheduleSend();
	void OnSendReady(boost::system::error_code /*error*/, std::size_t /*transferedBytes*/);

private:
	tcp::socket socket_;
	MessageBuffer recv_buffer_;
	std::mutex send_queue_lock_;
	std::queue<PacketBufferPtr> send_queue_; // LFQ로 교체?

	// addr
	boost::asio::ip::address remote_address_;
	uint16 remote_port_;

	// state
	std::atomic<bool> closed_;
	std::atomic<bool> is_writing_;

	// callback 핸들러
	std::function<void(std::shared_ptr<Session>)> disconnect_handler_ = nullptr;

	// network manager
	NetworkManager* network_manager_ = nullptr;
};
using SessionPtr	= std::shared_ptr<Session>;
using SessionPtrVec	= std::vector<SessionPtr>;

//void SetNoDelay(bool enable)
//{
//	boost::system::error_code err;
//	_socket.set_option(tcp::no_delay(enable), err);

//       // todo : error handling
//	//if (err)
//	//{
// 	 //    // LOG_ERROR
//	//}

//	GetRemoteIpAddress().to_string(), err.value(), err.message());
//}