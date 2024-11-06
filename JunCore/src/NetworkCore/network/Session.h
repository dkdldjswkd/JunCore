#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <Define.h>
#include <vector>
#include <queue>
#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include "Utilities/MessageBuffer.h"
#include <RingBuffer.h>
#include "../Packets/PacketBuffer.h"
#include <google/protobuf/message.h>

using boost::asio::ip::tcp;

#define READ_BLOCK_SIZE 4096

class NetworkThread;
class NetworkManager;

class Session
{
	friend NetworkManager;
	friend NetworkThread;

public:
	Session(tcp::socket&& socket);
	~Session();

public:
	void SendPacket(google::protobuf::Message& buffer)
	{
		const int32 _payload_size = buffer.ByteSizeLong();

		PacketBufferPtr _packet_buffer_ptr = std::make_shared<PacketBuffer>();
		if (_packet_buffer_ptr->GetFreeSize() < _payload_size)
		{
			// LOG_ERROR or packet_buffer_ptr resize
			return;
		}

		buffer.SerializeToArray(_packet_buffer_ptr->GetWritePos(), _payload_size);
		_packet_buffer_ptr->MoveWp(_payload_size);
		_packet_buffer_ptr->SetHeader();

		send_queue_.push(_packet_buffer_ptr);
	}

public:
	// addr
	boost::asio::ip::address GetRemoteIpAddress() const;
	uint16 GetRemotePort() const;

private:
	// NetworkThread, NetworkCore에서 사용
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
	std::queue<PacketBufferPtr> send_queue_; // LFQ로 교체?

	// addr
	boost::asio::ip::address remote_address_;
	uint16 remote_port_;

	// state
	std::atomic<bool> closed_;
	std::atomic<bool> is_writing_;

	// callback 핸들러
	std::function<void()> accept_handler_ = nullptr;
	std::function<void()> close_handler_ = nullptr;
	std::function<void()> recv_handler_ = nullptr;
};

using SessionPtr	= std::shared_ptr<Session>;
using SessionPtrVec	= std::vector<SessionPtr>;
#endif

//void SetNoDelay(bool enable)
//{
//	boost::system::error_code err;
//	_socket.set_option(tcp::no_delay(enable), err);

//       // todo : error handling
//	//if (err)
//	//{
//	//	// TC_LOG_DEBUG("network", "Session::SetNoDelay: failed to set_option(boost::asio::ip::tcp::no_delay) for {} - {} ({})",
//	//}

//	GetRemoteIpAddress().to_string(), err.value(), err.message());
//}