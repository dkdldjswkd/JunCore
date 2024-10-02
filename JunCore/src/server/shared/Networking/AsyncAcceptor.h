#ifndef __ASYNCACCEPT_H_
#define __ASYNCACCEPT_H_

#include <boost/asio/ip/tcp.hpp>
#include <Define.h>
#include <atomic>
#include <functional>

using boost::asio::ip::tcp;

#define JUNCORE_MAX_LISTEN_CONNECTIONS boost::asio::socket_base::max_listen_connections

class AsyncAcceptor
{
public:
	using AcceptCallback = void(*)(tcp::socket&& newSocket, uint32 threadIndex);

public:
	AsyncAcceptor(boost::asio::io_context& ioContext, std::string const& bindIp, uint16 port) :
		_acceptor(ioContext)
		, _endpoint(boost::asio::ip::make_address(bindIp), port)
		, _socket(ioContext)
		, _closed(false)
		, _socketFactory(std::bind(&AsyncAcceptor::DefeaultSocketFactory, this))
	{
	}

public:
	bool Bind()
	{
		boost::system::error_code errorCode;
		_acceptor.open(_endpoint.protocol(), errorCode);
		if (errorCode)
		{
			// TC_LOG_INFO("network", "Failed to open acceptor {}", errorCode.message());
			return false;
		}

#if TRINITY_PLATFORM != TRINITY_PLATFORM_WINDOWS // todo
		_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), errorCode);
		if (errorCode)
		{
			// TC_LOG_INFO("network", "Failed to set reuse_address option on acceptor {}", errorCode.message());
			return false;
		}
#endif

		_acceptor.bind(_endpoint, errorCode);
		if (errorCode)
		{
			// TC_LOG_INFO("network", "Could not bind to {}:{} {}", _endpoint.address().to_string(), _endpoint.port(), errorCode.message());
			return false;
		}

		_acceptor.listen(JUNCORE_MAX_LISTEN_CONNECTIONS, errorCode);
		if (errorCode)
		{
			// TC_LOG_INFO("network", "Failed to start listening on {}:{} {}", _endpoint.address().to_string(), _endpoint.port(), errorCode.message());
			return false;
		}

		return true;
	}

	template<class T>
	void AsyncAccept()
	{
		_acceptor.async_accept(_socket
			, [this](boost::system::error_code error) {
				if (!error)
				{
					try
					{
						std::make_shared<T>(std::move(this->_socket))->Start();
					}
					catch (boost::system::system_error const& err)
					{
						// TC_LOG_INFO("network", "Failed to retrieve client's remote address {}", err.what());
					}
				}

				if (!_closed)
					this->AsyncAccept<T>();
			}
		);
	}

	void Close()
	{
		if (_closed.exchange(true))
			return;

		boost::system::error_code err;
		_acceptor.close(err);
	}

	void SetSocketFactory(std::function<std::pair<tcp::socket*, uint32>()> func) { _socketFactory = func; }

	template<AcceptCallback acceptCallback>
	void AsyncAcceptWithCallback()
	{
		tcp::socket* socket;
		uint32 threadIndex;
		std::tie(socket, threadIndex) = _socketFactory();

		_acceptor.async_accept(*socket
			, [this, socket, threadIndex](boost::system::error_code error)
			{
				if (!error)
				{
					try
					{
						socket->non_blocking(true);

						acceptCallback(std::move(*socket), threadIndex);
					}
					catch (boost::system::system_error const& err)
					{
						// TC_LOG_INFO("network", "Failed to initialize client's socket {}", err.what());
					}
				}

				if (!_closed)
					this->AsyncAcceptWithCallback<acceptCallback>();
			}
		);
	}

private:
	std::pair<tcp::socket*, uint32> DefeaultSocketFactory() { return std::make_pair(&_socket, 0); }

private:
	tcp::acceptor _acceptor;
	tcp::endpoint _endpoint;
	tcp::socket _socket;
	std::atomic<bool> _closed;
	std::function<std::pair<tcp::socket*, uint32>()> _socketFactory;
};

#endif