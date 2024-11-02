#include "Session.h"

// NetworkCore::accept()에서 callback
void Session::Start()
{
	AsyncRecv();
};

void Session::AsyncRecv()
{
	if (!IsOpen())
		return;

	_recv_buffer.Normalize();
	_recv_buffer.EnsureFreeSpace();
	_socket.async_read_some(boost::asio::buffer(_recv_buffer.GetWritePointer(), _recv_buffer.GetRemainingSpace()), std::bind(&Session::ReadHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void Session::ReadHandler(boost::system::error_code error, size_t transferredBytes)
{
	if (error)
	{
		CloseSocket();
		return;
	}

	_recv_buffer.WriteCompleted(transferredBytes);

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

// NetworkThread::Update()에서 callback
bool Session::Update()
{
	if (_closed)
		return false;

	if (_is_writing || _send_queue.empty())
		return true;

	while (process_send_queue());
	return true;
}

bool Session::process_send_queue()
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
			return schedule_send();
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
		return schedule_send();
	}

	_send_queue.pop();
	return !_send_queue.empty();
}

void Session::WriteHandlerWrapper(boost::system::error_code /*error*/, std::size_t /*transferedBytes*/)
{
	_is_writing = false;
	process_send_queue();
}

bool Session::schedule_send()
{
	if (_is_writing)
		return false;

	_is_writing = true;
	_socket.async_write_some(boost::asio::null_buffers(), std::bind(&Session::WriteHandlerWrapper, this, std::placeholders::_1, std::placeholders::_2));
	return false;
}

boost::asio::ip::address Session::GetRemoteIpAddress() const
{
	return _remoteAddress;
}

uint16 Session::GetRemotePort() const
{
	return _remotePort;
}

// MessageBuffer에 user에게 데이터를 체워서 넘기게하자
// + header 공간은 core단에서 먼저 확보해두고, 우리가 header를 채우는 방식으로
// + MessageBuffer->PacketBuffer 치환
void Session::SendPacket(MessageBuffer&& buffer)
{
	// todo : server core header 세팅
	_send_queue.push(std::move(buffer));
}

bool Session::IsOpen() const { return !_closed; }

void Session::CloseSocket()
{
	if (_closed.exchange(true))
		return;

	boost::system::error_code shutdownError;
	_socket.shutdown(boost::asio::socket_base::shutdown_send, shutdownError);

	// MCHECK_RETURN(!shutdownError, "Session::close_socket: {} errored when shutting down socket: {} ({})", GetRemoteIpAddress().to_string(), shutdownError.value(), shutdownError.message());
	OnClose();
}

MessageBuffer& Session::get_recv_buffer() { return _recv_buffer; }
