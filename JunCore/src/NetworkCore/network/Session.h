#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <Define.h>
#include <vector>
#include <queue>
#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include "Utilities/MessageBuffer.h"
#include "../Packets/PacketHeader.h"
#include <ring_buffer.h>

using boost::asio::ip::tcp;

#define READ_BLOCK_SIZE 4096

class NetworkThread;
class NetworkCore;

class Session
{
	friend NetworkCore;
	friend NetworkThread;

public:
	Session(tcp::socket&& socket);
	~Session();

public:
	void SendPacket(MessageBuffer&& buffer);

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
	// Session 내부 Recv 핸들링 시 사용
	void AsyncRecv();
	void ReadHandler(boost::system::error_code error, size_t transferredBytes);
	MessageBuffer& get_recv_buffer();
	void schedule_send();

private:
	std::function<void()> accept_handler_ = nullptr;
	std::function<void()> close_handler_  = nullptr;
	std::function<void()> recv_handler_   = nullptr;

private:
	bool process_send_queue();
	void WriteHandlerWrapper(boost::system::error_code /*error*/, std::size_t /*transferedBytes*/);

private:
	tcp::socket _socket;
	/*ring_buffer*/ MessageBuffer _recv_buffer; // ring_buffer 교체할것.
	std::queue<MessageBuffer> _send_queue;

	// addr
	boost::asio::ip::address _remoteAddress;
	uint16 _remotePort;

	// state
	std::atomic<bool> _closed;
	std::atomic<bool> _is_writing;
};

using NetworkSessionPtr		= std::shared_ptr<Session>;
using NetworkSessionPtrVec	= std::vector<NetworkSessionPtr>;
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