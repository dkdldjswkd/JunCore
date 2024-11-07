#include "Session.h"
#include "../network/NetworkManager.h"

Session::Session(tcp::socket&& _socket, NetworkManager* _network_manager)
	:	/*socket*/	socket_(std::move(_socket)),
		/*addr*/	remote_address_(socket_.remote_endpoint().address()), remote_port_(socket_.remote_endpoint().port()),
		/*state*/	closed_(false), is_writing_(false),
		/*network_manager*/ network_manager_(_network_manager)
{
}

Session::~Session()
{
	closed_ = true;
	boost::system::error_code error;
	socket_.close(error);
}

// NetworkCore::accept()에서 callback
void Session::Start()
{
	AsyncRecv();
};

void Session::AsyncRecv()
{
	if (!IsOpen())
		return;

	recv_buffer_.Normalize();
	recv_buffer_.EnsureFreeSpace();
	socket_.async_read_some(boost::asio::buffer(recv_buffer_.GetWritePointer(), recv_buffer_.GetRemainingSpace()), std::bind(&Session::ReadHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void Session::ReadHandler(boost::system::error_code error, size_t transferredBytes)
{
	if (error || !(0 < transferredBytes))
	{
		CloseSocket();
		return;
	}

	recv_buffer_.WriteCompleted(transferredBytes);

	if (!IsOpen())
		return;

	/////////////////////
	// 패킷 처리
	/////////////////////

	for (;;)
	{
		const int32 _recv_size = recv_buffer_.GetActiveSize();

		// 1. 헤더 사이즈 체크
		if (_recv_size < HEADER_SIZE)
		{
			recv_buffer_.Normalize();
			break;
		}

		PacketHeader* _p_packet_header = (PacketHeader*)recv_buffer_.GetReadPointer();

		// 2. code 검사
		// todo

		// 3. 페이로드 Size 체크
		if (_recv_size < (HEADER_SIZE + _p_packet_header->len))
		{
			recv_buffer_.Normalize();
			break;
		}

		// 4. serialized packet 추출 및 read pos 전진
		std::vector<char> _serialized_packet(_p_packet_header->len);
		std::memcpy(_serialized_packet.data(), recv_buffer_.GetReadPointer(), _p_packet_header->len);
		recv_buffer_.ReadCompleted(HEADER_SIZE + _p_packet_header->len);

		// NetworkManager의 packet handler 호출
		network_manager_->HandlePacket(shared_from_this(), _p_packet_header->pid, _serialized_packet);
	}

	// Recv 예약
	AsyncRecv();
}

MessageBuffer& Session::GetRecvBuffer()
{
	return recv_buffer_;
}

// NetworkThread::Update()에서 callback
bool Session::Update()
{
	if (closed_)
		return false;

	if (is_writing_ || send_queue_.empty())
		return true;

	while (ProcessSendQueue());
	return true;
}

// ret true : send queue 추가 처리가 가능한 경우, ret false : send queue가 비었거나, 추가 처리가 불가능한 경우 (ex. would_block)
bool Session::ProcessSendQueue()
{
	if (send_queue_.empty())
		return false;

	PacketBufferPtr _send_msg_ptr = send_queue_.front();
	std::size_t _send_msg_size = _send_msg_ptr->GetPacketSize();

	boost::system::error_code error;
	std::size_t _complete_send_msg_size = socket_.write_some(boost::asio::buffer(_send_msg_ptr->GetPacketPos(), _send_msg_size), error);

	if (error)
	{
		if (error == boost::asio::error::would_block || error == boost::asio::error::try_again)
		{
			ScheduleSend();
			return false;
		}
		else
		{
			send_queue_.pop();
			return false;
		}
	}
	else if (_complete_send_msg_size == 0)
	{
		send_queue_.pop();
		return false;
	}

	// 패킷을 부분적으로 송신한 경우 (but 결과적으로 write_some()을 통해 send 하므로, 해당 if에 걸릴 수 없음.)
	if (_complete_send_msg_size < _send_msg_size)
	{
		// LOG_ERROR("invalid case");
		// read pos 전진
		ScheduleSend();
		return false;
	}

	send_queue_.pop();
	return !send_queue_.empty();
}

void Session::ScheduleSend()
{
	if (is_writing_)
		return;

	is_writing_ = true;
	socket_.async_write_some(boost::asio::null_buffers(), std::bind(&Session::OnSendReady, this, std::placeholders::_1, std::placeholders::_2));
	return;
}

void Session::OnSendReady(boost::system::error_code /*error*/, std::size_t /*transferedBytes*/)
{
	is_writing_ = false;
	ProcessSendQueue();
}

boost::asio::ip::address Session::GetRemoteIpAddress() const
{
	return remote_address_;
}

uint16 Session::GetRemotePort() const
{
	return remote_port_;
}

bool Session::IsOpen() const 
{ 
	return !closed_; 
}

void Session::CloseSocket()
{
	if (closed_.exchange(true))
		return;

	boost::system::error_code shutdownError;
	socket_.shutdown(boost::asio::socket_base::shutdown_send, shutdownError); // todo

	// MCHECK_RETURN(!shutdownError, "Session::close_socket: {} errored when shutting down socket: {} ({})", GetRemoteIpAddress().to_string(), shutdownError.value(), shutdownError.message());
	close_handler_();
}