#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <Define.h>
#include <vector>
#include <queue>
#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include "Utilities/MessageBuffer.h"
#include <ring_buffer.h>
#include "Packets/PacketBuffer.h"

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
	template <typename T>
	void SendPacket(const T& buffer)
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
		//// 헤더 및 Packet 세팅
		//{
		//	// CHECK_RETURN(true == check_remain_size(_packet_size), false);
		//	_packet_header._size = _packet_size;
		//	_packet_header._seq = _crypt.advance_send_seq();
		//	CHECK_RETURN(_pid < MAX_PACKET_NUMBER, false);
		//	_packet_header._pid = _pid;
		//	_packet_header._check = server_config::header_match_value;
		//	_packet_header._option = packet_option{ _b_comp };
		//	_packet_header._compression_msg_size = _compression_msg_size;
		//	_packet_header._msg_size = _msg_size;

		//	memcpy(_buffer.data(), reinterpret_cast<const char*>(&_packet_header), _header_size);

		// 암호화
		//	_crypt.encrypt(_buffer.data(), _packet_size);
		//	_write_positon += _packet_size;
		//}



		// todo : server core header 세팅
//		send_queue_.push(_packet_buffer_ptr);
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
	/*ring_buffer*/ MessageBuffer recv_buffer_; // ring_buffer 교체할것.
	std::queue<MessageBufferPtr> send_queue_;

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