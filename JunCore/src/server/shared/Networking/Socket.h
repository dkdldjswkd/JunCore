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
#ifdef BOOST_ASIO_HAS_IOCP
#define TC_SOCKET_USE_IOCP
#endif

template<class T>
class Socket : public std::enable_shared_from_this<T>
{
public:
	explicit Socket(tcp::socket&& socket)
		: _socket(std::move(socket)), _remoteAddress(_socket.remote_endpoint().address()), _remotePort(_socket.remote_endpoint().port()), _readBuffer(), _closed(false), _closing(false), _isWritingAsync(false)
	{
		_readBuffer.Resize(READ_BLOCK_SIZE);
	}

	virtual void Start() = 0;

	virtual bool Update()
	{
		if (_closed)
			return false;

#ifndef TC_SOCKET_USE_IOCP
		if (_isWritingAsync || (_writeQueue.empty() && !_closing))
			return true;

		for (; HandleQueue();)
			;
#endif

		return true;
	}


private:
	tcp::socket _socket;

	boost::asio::ip::address _remoteAddress;
	uint16 _remotePort;

	MessageBuffer _readBuffer;
	std::queue<MessageBuffer> _writeQueue;

	std::atomic<bool> _closed;
	std::atomic<bool> _closing;

	bool _isWritingAsync;
};

#endif