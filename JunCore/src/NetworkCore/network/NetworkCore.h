#ifndef SocketMgr_h__
#define SocketMgr_h__

#include "AsyncAcceptor.h"
#include "Errors.h"
#include "NetworkThread.h"
#include "NetworkSession.h"
#include <boost/asio/ip/tcp.hpp>
#include <memory>

using boost::asio::ip::tcp;

template<class SocketType>
class NetworkCore
{
public:
	NetworkCore() = default;
	virtual ~NetworkCore() = default;

	bool StartNetwork(std::string const& _bind_ip, uint16 _port, int _worker_cnt)
	{
		_io_context = new boost::asio::io_context;
		_acceptor = new AsyncAcceptor(*_io_context, _bind_ip, _port);

		// CHECK_RETURN(threadCount > 0, false);

		if (!_acceptor->Bind())
		{
			// LOG_ERROR
			delete _acceptor;
			return false;
		}

		_network_threads.reserve(_worker_cnt);

		for (int i = 0; i < _worker_cnt; ++i)
		{
			auto _worker = new NetworkThread<SocketType>();
			_network_threads.emplace_back(_worker);

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
		_acceptor = nullptr;

		// 4. acceptor ioconext 정리
		delete _io_context;
		_io_context = nullptr;

		// 5. workers ioconext stop
		for (const auto& _worker : _network_threads)
		{
			_worker->Stop();
		}

		// 6. workers join wait
		for (const auto& _worker : _network_threads)
		{
			_worker->Wait();
		}
	}

	void async_accept()
	{
		uint32 _min_worker_index = 0;

		for (int _worker_index = 0; _worker_index < _network_threads.size(); ++_worker_index)
		{
			if (_network_threads[_worker_index]->GetConnectionCount() < _network_threads[_min_worker_index]->GetConnectionCount())
				_min_worker_index = _worker_index;
		}

		tcp::socket* _accept_sock = _network_threads[_min_worker_index]->GetSocketForAccept();

		_acceptor->async_accept(*_accept_sock
			// accept handler
			, [this, _accept_sock, _min_worker_index](boost::system::error_code error)
			{
				if (!error)
				{
					// 비동기 소켓으로 생성
					_accept_sock->non_blocking(true);

					std::shared_ptr<SocketType> _new_sock = std::make_shared<SocketType>(std::move(*_accept_sock));
					_network_threads[_min_worker_index]->AddNewSession(_new_sock); // todo
					_new_sock->Start(); // ex. EchoSocket::Start()
				}

				if (!_acceptor->is_closed())
				{
					this->async_accept();
				}
			}
		);
	}

private:
	boost::asio::io_context* _io_context = nullptr;
	AsyncAcceptor* _acceptor = nullptr;
	std::vector<NetworkThread*> _network_threads;

};

#endif // SocketMgr_h__