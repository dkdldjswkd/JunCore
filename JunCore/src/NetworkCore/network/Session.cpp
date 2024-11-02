#include "Session.h"

Session::Session(tcp::socket&& socket)
	:	/*socket*/	_socket(std::move(socket)),
		/*addr*/	_remoteAddress(_socket.remote_endpoint().address()), _remotePort(_socket.remote_endpoint().port()),
		/*state*/	_closed(false), _is_writing(false)
{
}

Session::~Session()
{
	_closed = true;
	boost::system::error_code error;
	_socket.close(error);
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

	if (!IsOpen())
		return;

	// ... 패킷 처리

	AsyncRecv();
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

// ret true : send queue 추가 처리가 가능한 경우, ret false : send queue가 비었거나, 추가 처리가 불가능한 경우 (ex. would_block)
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
			schedule_send();
			return false;
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
		schedule_send();
		return false;
	}

	_send_queue.pop();
	return !_send_queue.empty();
}

void Session::WriteHandlerWrapper(boost::system::error_code /*error*/, std::size_t /*transferedBytes*/)
{
	_is_writing = false;
	process_send_queue();
}

void Session::schedule_send()
{
	if (_is_writing)
		return;

	_is_writing = true;
	_socket.async_write_some(boost::asio::null_buffers(), std::bind(&Session::WriteHandlerWrapper, this, std::placeholders::_1, std::placeholders::_2));
	return;
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

bool Session::IsOpen() const 
{ 
	return !_closed; 
}

void Session::CloseSocket()
{
	if (_closed.exchange(true))
		return;

	boost::system::error_code shutdownError;
	_socket.shutdown(boost::asio::socket_base::shutdown_send, shutdownError); // todo

	// MCHECK_RETURN(!shutdownError, "Session::close_socket: {} errored when shutting down socket: {} ({})", GetRemoteIpAddress().to_string(), shutdownError.value(), shutdownError.message());
	close_handler_();
}

MessageBuffer& Session::get_recv_buffer() 
{ 
	return _recv_buffer; 
}