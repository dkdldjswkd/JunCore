#ifndef SocketMgr_h__
#define SocketMgr_h__

//#include "AsyncAcceptor.h"
#include "Errors.h"
#include "NetworkThread.h"
#include <boost/asio/ip/tcp.hpp>
#include <memory>

using boost::asio::ip::tcp;

class AsyncAcceptor
{
public:
	AsyncAcceptor(boost::asio::io_context& _io_context, std::string const& _bind_ip, uint16 _port) : 
		_acceptor(_io_context), _endpoint(boost::asio::ip::make_address(_bind_ip), _port) , _closed(false)
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

private:
	tcp::acceptor _acceptor;
	tcp::endpoint _endpoint;
	std::atomic<bool> _closed;
};

template<class SocketType>
class SocketMgr
{
public:
	SocketMgr() = default;
	virtual ~SocketMgr() = default;

	virtual bool StartNetwork(std::string const& _bind_ip, uint16 _port, int _worker_cnt)
	{
		_io_context = new boost::asio::io_context;
		_acceptor   = new AsyncAcceptor(*_io_context, _bind_ip, _port);

        // CHECK_RETURN(threadCount > 0, false);

		if (!_acceptor->Bind())
		{
			// LOG_ERROR
			delete acceptor;
			return false;
		}

		_workers.reserve(_worker_cnt);

		for (int i = 0; i < _worker_cnt; ++i)
		{
			auto _worker = new NetworkThread<SocketType>();
			_workers.emplace_back(_worker);

			_worker->Start();
		}

		this->async_accept();
		return true;
	}

	virtual void StopNetwork()
	{
		// 1. acceptor 종료
		_acceptor->close();

		// 2. acceptor ioconext stop
		_io_context->stop();

		// 3. acceptor 정리
		delete _acceptor;
		_acceptor = nullptr

		// 4. acceptor ioconext 정리
		delete _io_context;
		_io_context = nullptr;

		// 5. workers ioconext stop
		for (const auto& _worker : _workers)
		{
			_worker->stop();
		}

		// 6. workers join wait
		for (const auto& _worker : _workers)
		{
			_worker->Wait();
		}
	}

	void async_accept()
	{
		uint32 _min_worker_index = 0;

		for (int _worker_index= 0 ; _worker_index < _workers.size(); ++_worker_index)
		{
			if (_workers[_worker_index].GetConnectionCount() < _workers[_min_worker_index].GetConnectionCount())
				_min_worker_index = _worker_index;
		}

		tcp::socket* _accept_sock = _workers[_min_worker_index].GetSocketForAccept();

		_acceptor.async_accept(*_accept_sock
			// accept handler
			, [this, _accept_sock, _min_worker_index, &_workers](boost::system::error_code error)
			{
				if (!error)
				{
					// 비동기 소켓으로 생성
					_accept_sock->non_blocking(true);

					std::shared_ptr<SocketType> _new_sock(_accept_sock);
					_workers[_min_worker_index]->AddSocket(_new_sock);
					_new_sock->Start(); // ex. EchoSocket::Start()
				}

				if (!_closed)
				{
					this->async_accept();
				}
			}
		);
	}

private:
	boost::asio::io_context* _io_context = nullptr;
	AsyncAcceptor* _acceptor = nullptr;
    std::vector<NetworkThread<SocketType>*> _workers;
};

#endif // SocketMgr_h__
