#include "AsyncAcceptor.h"

AsyncAcceptor::AsyncAcceptor(boost::asio::io_context& _io_context, std::string const& _bind_ip, uint16 _port)
	: _acceptor(_io_context), _endpoint(boost::asio::ip::make_address(_bind_ip), _port), _closed(false)
{
}

AsyncAcceptor::~AsyncAcceptor()
{
}

bool AsyncAcceptor::Bind()
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

void AsyncAcceptor::close()
{
	if (_closed.exchange(true))
		return;

	boost::system::error_code err;
	_acceptor.close(err);
}

bool AsyncAcceptor::is_closed() const
{
	return _closed.load();
}
