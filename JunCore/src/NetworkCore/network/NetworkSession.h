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

class NetworkSession
{
public:
	explicit NetworkSession(tcp::socket&& socket)
		:	/*socket*/	_socket(std::move(socket))
		/*addr*/, _remoteAddress(_socket.remote_endpoint().address()), _remotePort(_socket.remote_endpoint().port())
		/*state*/, _closed(false), _is_writing(false)
	{
	}

	virtual ~NetworkSession()
	{
		_closed = true;
		boost::system::error_code error;
		_socket.close(error);
	}

public:
	void Start(); // async_accept 시 callback
	void async_recv();
	bool Update(); // Update Thread 에서 모든 NetworkSession들을 순회하며 호출

public:
	// MessageBuffer에 user에게 데이터를 체워서 넘기게하자
	// + header 공간은 core단에서 먼저 확보해두고, 우리가 header를 채우는 방식으로
	// + MessageBuffer->PacketBuffer 치환
	void send_packet(MessageBuffer&& buffer)
	{
		// todo : server core header 세팅
		_send_queue.push(std::move(buffer));
	}

	bool is_open() const { return !_closed; }

	void close_socket()
	{
		if (_closed.exchange(true))
			return;

		boost::system::error_code shutdownError;
		_socket.shutdown(boost::asio::socket_base::shutdown_send, shutdownError);

		// MCHECK_RETURN(!shutdownError, "NetworkSession::close_socket: {} errored when shutting down socket: {} ({})", GetRemoteIpAddress().to_string(), shutdownError.value(), shutdownError.message());
		OnClose();
	}

	MessageBuffer& get_recv_buffer() { return _recv_buffer; }

	// address
	boost::asio::ip::address GetRemoteIpAddress() const;
	uint16 GetRemotePort() const;

protected:
	// todo NetworkCore 멤버 함수로 이동
	virtual void OnStart() {};
	virtual void OnClose() {};
	virtual void OnRecv() {};

	bool async_send_inner();

private:
	// _send_queue를 모두 처리함 (empty가 될때까지 send)
	bool process_send_queue();

	void ReadHandlerInternal(boost::system::error_code error, size_t transferredBytes)
	{
		if (error)
		{
			close_socket();
			return;
		}

		_recv_buffer.WriteCompleted(transferredBytes);
		on_recv_core();
	}

	void on_recv_core() {
		//if (!is_open())
		//	return;

		//MessageBuffer& packet = get_recv_buffer();

		//// 패킷 조립
		//for (;;) {
		//	int recvLen = p_session->recvBuf.GetUseSize();
		//	if (recvLen <= NET_HEADER_SIZE)
		//		break;

		//	// 헤더 카피
		//	char encryptPacket[NET_HEADER_SIZE + MAX_PAYLOAD_LEN];
		//	p_session->recvBuf.Peek(encryptPacket, NET_HEADER_SIZE);

		//	// code 검사
		//	BYTE code = ((PacketHeader*)encryptPacket)->code;
		//	if (code != protocolCode) {
		//		LOG("NetServer", LOG_LEVEL_WARN, "Recv Packet is wrong code!!", WSAGetLastError());
		//		DisconnectSession(p_session);
		//		break;
		//	}

		//	// 페이로드 데이터 부족
		//	WORD payloadLen = ((PacketHeader*)encryptPacket)->len;
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
		//	async_recv(p_session);
		//}

		//=======================================================

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
		//			close_socket();
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
		//			close_socket();

		//		return;
		//	}
		//}

		// async_recv();

	}

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

using NetworkSessionPtr		= std::shared_ptr<NetworkSession>;
using NetworkSessionPtrVec	= std::vector<NetworkSessionPtr>;
#endif

//void SetNoDelay(bool enable)
//{
//	boost::system::error_code err;
//	_socket.set_option(tcp::no_delay(enable), err);

//       // todo : error handling
//	//if (err)
//	//{
//	//	// TC_LOG_DEBUG("network", "NetworkSession::SetNoDelay: failed to set_option(boost::asio::ip::tcp::no_delay) for {} - {} ({})",
//	//}

//	GetRemoteIpAddress().to_string(), err.value(), err.message());
//}