#ifndef __ASYNCACCEPT_H_
#define __ASYNCACCEPT_H_

#include <boost/asio/ip/tcp.hpp>
#include <Define.h>
#include <atomic>
#include <functional>

using boost::asio::ip::tcp;

class AsyncAcceptor
{
public:
	AsyncAcceptor(boost::asio::io_context& _io_context, std::string const& _bind_ip, uint16 _port) :
		_acceptor(_io_context), _endpoint(boost::asio::ip::make_address(_bind_ip), _port), _closed(false)
	{
	}
	~AsyncAcceptor() = default;

public:
	bool Bind()
	{
		boost::system::error_code _error_code;
		_acceptor.open(_endpoint.protocol(), _error_code);
		if (_error_code)
		{
			// LOG_ERROR("network", "Failed to open acceptor {}", errorCode.message());
			return false;
		}

		_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), _error_code);
		if (_error_code)
		{
			// LOG_ERROR("network", "Failed to set reuse_address option on acceptor {}", errorCode.message());
			return false;
		}

		_acceptor.bind(_endpoint, _error_code);
		if (_error_code)
		{
			// LOG_ERROR("network", "Could not bind to {}:{} {}", _endpoint.address().to_string(), _endpoint.port(), errorCode.message());
			return false;
		}

		_acceptor.listen(boost::asio::socket_base::max_listen_connections, _error_code);
		if (_error_code)
		{
			// LOG_ERROR("network", "Failed to start listening on {}:{} {}", _endpoint.address().to_string(), _endpoint.port(), errorCode.message());
			return false;
		}

		return true;
	}

	inline void async_accept(tcp::socket& _socket, std::function<void(boost::system::error_code)> _accept_handler)
	{
		_acceptor.async_accept(_socket, _accept_handler);
	}

	void close()
	{
		if (_closed.exchange(true))
			return;

		boost::system::error_code err;
		_acceptor.close(err);
	}

	bool is_closed() const
	{
		return _closed.load();
	}

private:
	tcp::acceptor _acceptor;
	tcp::endpoint _endpoint;
	std::atomic<bool> _closed;
};
#endif