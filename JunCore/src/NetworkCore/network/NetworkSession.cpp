#include "NetworkSession.h"

boost::asio::ip::address NetworkSession::GetRemoteIpAddress() const
{
	return _remoteAddress;
}

uint16 NetworkSession::GetRemotePort() const
{
	return _remotePort;
}

void NetworkSession::Start()
{
	async_recv();
	OnStart();
};

bool NetworkSession::Update()
{
	if (_closed)
		return false;

	if (_is_writing || _send_queue.empty())
		return true;

	while (process_send_queue());
	return true;
}

void NetworkSession::WriteHandlerWrapper(boost::system::error_code /*error*/, std::size_t /*transferedBytes*/)
{
	_is_writing = false;
	process_send_queue();
}

bool NetworkSession::async_send_inner()
{
	if (_is_writing)
		return false;

	_is_writing = true;
	_socket.async_write_some(boost::asio::null_buffers(), std::bind(&NetworkSession::WriteHandlerWrapper, this, std::placeholders::_1, std::placeholders::_2));
	return false;
}


bool NetworkSession::process_send_queue()
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

	// ��Ŷ�� �κ������� �۽��� ��� (but ��������� write_some()�� ���� send �ϹǷ�, �ش� if�� �ɸ� �� ����.)
	if (_complete_send_msg_size < _send_msg_size)
	{
		// LOG_ERROR("invalid case");
		_send_msg.ReadCompleted(_complete_send_msg_size);
		return async_send_inner();
	}

	_send_queue.pop();
	return !_send_queue.empty();
}

void NetworkSession::async_recv()
{
	if (!is_open())
		return;

	_recv_buffer.Normalize();
	_recv_buffer.EnsureFreeSpace();
	_socket.async_read_some(boost::asio::buffer(_recv_buffer.GetWritePointer(), _recv_buffer.GetRemainingSpace()), std::bind(&NetworkSession::ReadHandlerInternal, this, std::placeholders::_1, std::placeholders::_2));
}
