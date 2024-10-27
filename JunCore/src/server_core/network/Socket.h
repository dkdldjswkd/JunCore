#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <Define.h>
#include <vector>
#include <queue>
#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include "Utilities/MessageBuffer.h"

using boost::asio::ip::tcp;

#define READ_BLOCK_SIZE 4096

template<class T>
class Socket : public std::enable_shared_from_this<T>
{
public:
	explicit Socket(tcp::socket&& socket) 
		: _socket(std::move(socket)), _remoteAddress(_socket.remote_endpoint().address()), _remotePort(_socket.remote_endpoint().port()), _recv_buffer(), _closed(false), _is_writing(false)
	{
		_recv_buffer.Resize(READ_BLOCK_SIZE);
	}

	virtual ~Socket()
	{
		_closed = true;
		boost::system::error_code error;
		_socket.close(error);
	}

public:
	void Start(); // async_accept 시 callback
	void async_recv();
	bool Update(); // Update Thread 에서 모든 Socket들을 순회하며 호출

public:
	void SendPacket(MessageBuffer&& buffer)
	{
		_send_queue.push(std::move(buffer));
	}

	bool IsOpen() const { return !_closed; }

	void CloseSocket()
	{
		if (_closed.exchange(true))
			return;

		boost::system::error_code shutdownError;
		_socket.shutdown(boost::asio::socket_base::shutdown_send, shutdownError);

		// MCHECK_RETURN(!shutdownError, "Socket::CloseSocket: {} errored when shutting down socket: {} ({})", GetRemoteIpAddress().to_string(), shutdownError.value(), shutdownError.message());

		OnClose();
	}

	MessageBuffer& GetReadBuffer() { return _recv_buffer; }

	// address
	boost::asio::ip::address GetRemoteIpAddress() const;
	uint16 GetRemotePort() const;

protected:
	virtual void OnStart() = 0;
	virtual void OnClose() { }
	virtual void OnRecv() = 0;

	bool async_send_inner()
	{
		if (_is_writing)
			return false;

		_is_writing = true;
		_socket.async_write_some(boost::asio::null_buffers(), std::bind(&Socket<T>::WriteHandlerWrapper, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
		return false;
	}

private:
	// _send_queue를 모두 처리함 (empty가 될때까지 send)
	bool process_send_queue();

	void ReadHandlerInternal(boost::system::error_code error, size_t transferredBytes)
	{
		if (error)
		{
			CloseSocket();
			return;
		}

		_recv_buffer.WriteCompleted(transferredBytes);
		on_recv_core();
	}

	void on_recv_core() {
		if (!IsOpen())
			return;

		MessageBuffer& packet = GetReadBuffer();

		//while (packet.GetActiveSize() > 0)
		//{
		//	if (_headerBuffer.GetRemainingSpace() > 0)
		//	{
		//		// need to receive the header
		//		std::size_t readHeaderSize = std::min(packet.GetActiveSize(), _headerBuffer.GetRemainingSpace());
		//		_headerBuffer.Write(packet.GetReadPointer(), readHeaderSize);
		//		packet.ReadCompleted(readHeaderSize);

		//		if (_headerBuffer.GetRemainingSpace() > 0)
		//		{
		//			// Couldn't receive the whole header this time.
		//			// ASSERT(packet.GetActiveSize() == 0);
		//			break;
		//		}

		//		// We just received nice new header
		//		if (!ReadHeaderHandler())
		//		{
		//			CloseSocket();
		//			return;
		//		}
		//	}

		//	// We have full read header, now check the data payload
		//	if (_packetBuffer.GetRemainingSpace() > 0)
		//	{
		//		// need more data in the payload
		//		std::size_t readDataSize = std::min(packet.GetActiveSize(), _packetBuffer.GetRemainingSpace());
		//		_packetBuffer.Write(packet.GetReadPointer(), readDataSize);
		//		packet.ReadCompleted(readDataSize);

		//		if (_packetBuffer.GetRemainingSpace() > 0)
		//		{
		//			// Couldn't receive the whole data this time.
		//			// ASSERT(packet.GetActiveSize() == 0);
		//			break;
		//		}
		//	}

		//	// just received fresh new payload
		//	ReadDataHandlerResult result = ReadDataHandler();
		//	_headerBuffer.Reset();
		//	if (result != ReadDataHandlerResult::Ok)
		//	{
		//		if (result != ReadDataHandlerResult::WaitingForQuery)
		//			CloseSocket();

		//		return;
		//	}
		//}

		async_recv();

		//=======================================================

		//	// 패킷 조립
		//for (;;) {
		//	int recvLen = p_session->recvBuf.GetUseSize();
		//	if (recvLen <= NET_HEADER_SIZE)
		//		break;

		//	// 헤더 카피
		//	char encryptPacket[NET_HEADER_SIZE + MAX_PAYLOAD_LEN];
		//	p_session->recvBuf.Peek(encryptPacket, NET_HEADER_SIZE);

		//	// code 검사
		//	BYTE code = ((NetHeader*)encryptPacket)->code;
		//	if (code != protocolCode) {
		//		LOG("NetServer", LOG_LEVEL_WARN, "Recv Packet is wrong code!!", WSAGetLastError());
		//		DisconnectSession(p_session);
		//		break;
		//	}

		//	// 페이로드 데이터 부족
		//	WORD payloadLen = ((NetHeader*)encryptPacket)->len;
		//	if (recvLen < (NET_HEADER_SIZE + payloadLen)) {
		//		break;
		//	}

		//	// 암호패킷 복사
		//	p_session->recvBuf.MoveFront(NET_HEADER_SIZE);
		//	p_session->recvBuf.Dequeue(encryptPacket + NET_HEADER_SIZE, payloadLen);

		//	// 패킷 복호화
		//	PacketBuffer* decrypt_packet = PacketBuffer::Alloc();
		//	if (!decrypt_packet->DecryptPacket(encryptPacket, privateKey)) {
		//		PacketBuffer::Free(decrypt_packet);
		//		LOG("NetServer", LOG_LEVEL_WARN, "Recv Packet is wrong checksum!!", WSAGetLastError());
		//		DisconnectSession(p_session);
		//		break;
		//	}

		//	// 사용자 패킷 처리
		//	OnRecv(p_session->sessionId, decrypt_packet);
		//	InterlockedIncrement(&recvMsgCount);

		//	// 암호패킷, 복호화 패킷 Free
		//	PacketBuffer::Free(decrypt_packet);
		//}

		////------------------------------
		//// Post Recv
		////------------------------------
		//if (!p_session->disconnectFlag) {
		//	AsyncRecv(p_session);
		//}

	}

	void WriteHandlerWrapper(boost::system::error_code /*error*/, std::size_t /*transferedBytes*/)
	{
		_is_writing = false;
		process_send_queue();
	}

private:
	// socket
	tcp::socket _socket;
	MessageBuffer _recv_buffer;
	std::queue<MessageBuffer> _send_queue;

	// addr
	boost::asio::ip::address _remoteAddress;
	uint16 _remotePort;

	// state
	std::atomic<bool> _closed;
	bool _is_writing; // update thread 에서만 사용하므로, 공유자원 x
};


template<class T>
boost::asio::ip::address Socket<T>::GetRemoteIpAddress() const
{
	return _remoteAddress;
}

template<class T>
uint16 Socket<T>::GetRemotePort() const
{
	return _remotePort;
}

template<class T>
void Socket<T>::Start()
{
	async_recv();
	OnStart();
};

template<class T>
void Socket<T>::async_recv()
{
	if (!IsOpen())
		return;

	_recv_buffer.Normalize();
	_recv_buffer.EnsureFreeSpace();
	_socket.async_read_some(boost::asio::buffer(_recv_buffer.GetWritePointer(), _recv_buffer.GetRemainingSpace()), std::bind(&Socket<T>::ReadHandlerInternal, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

template<class T>
bool Socket<T>::process_send_queue()
{
	if (_send_queue.empty())
		return false;

	MessageBuffer& _send_msg = _send_queue.front();
	std::size_t _send_msg_size = _send_msg.GetActiveSize();

	boost::system::error_code error;
	std::size_t _complete_send_msg_size = _socket.write_some(boost::asio::buffer(_send_msg.GetReadPointer(), _send_msg_size), error);

	if (error)
	{
		if (error == boost::asio::error::would_block || error == boost::asio::error::try_again)
		{
			return async_send_inner();
		}
		else
		{
			_send_queue.pop();
			return false;
		}
	}
	else if (_complete_send_msg_size == 0)
	{
		_send_queue.pop();
		return false;
	}

	// 패킷을 부분적으로 송신한 경우 (but 결과적으로 write_some()을 통해 send 하므로, 해당 if에 걸릴 수 없음.)
	if (_complete_send_msg_size < _send_msg_size)
	{
		// LOG_ERROR("invalid case");
		_send_msg.ReadCompleted(_complete_send_msg_size);
		return async_send_inner();
	}

	_send_queue.pop();
	return !_send_queue.empty();
}

template<class T>
bool Socket<T>::Update()
{
	if (_closed)
		return false;

	if (_is_writing || _send_queue.empty())
		return true;

	while (process_send_queue());
	return true;
}
#endif

//void SetNoDelay(bool enable)
//{
//	boost::system::error_code err;
//	_socket.set_option(tcp::no_delay(enable), err);

//       // todo : error handling
//	//if (err)
//	//{
//	//	// TC_LOG_DEBUG("network", "Socket::SetNoDelay: failed to set_option(boost::asio::ip::tcp::no_delay) for {} - {} ({})",
//	//}

//	GetRemoteIpAddress().to_string(), err.value(), err.message());
//}