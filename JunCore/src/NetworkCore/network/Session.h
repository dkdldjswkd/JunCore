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

class Session
{
public:
	explicit Session(tcp::socket&& socket)
		:	/*socket*/	_socket(std::move(socket)),
			/*addr*/	_remoteAddress(_socket.remote_endpoint().address()), _remotePort(_socket.remote_endpoint().port()),
			/*state*/	_closed(false), _is_writing(false)
	{
	}

	~Session()
	{
		_closed = true;
		boost::system::error_code error;
		_socket.close(error);
	}

public:
	void Start();
	bool Update();
	void CloseSocket();
	bool IsOpen() const;

public:
	void SendPacket(MessageBuffer&& buffer);
	// address
	boost::asio::ip::address GetRemoteIpAddress() const;
	uint16 GetRemotePort() const;

private:
	void AsyncRecv();
	MessageBuffer& get_recv_buffer();

private:
	std::function<void()> accept_handler = nullptr;
	void OnClose() {};
	void OnRecv() {};

	bool schedule_send();

private:
	bool process_send_queue();
	void ReadHandler(boost::system::error_code error, size_t transferredBytes);
	void WriteHandlerWrapper(boost::system::error_code /*error*/, std::size_t /*transferedBytes*/);

private:
	// socket
	tcp::socket _socket;
	/*ring_buffer*/ MessageBuffer _recv_buffer; // ring_buffer 교체할것.
	std::queue<MessageBuffer> _send_queue;

	// addr
	boost::asio::ip::address _remoteAddress;
	uint16 _remotePort;

	// state
	std::atomic<bool> _closed;
	bool _is_writing; // worker 에서만 사용하므로, 공유자원 x
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