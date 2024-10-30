#include "NetworkCore.h"

NetworkCore::NetworkCore()
{
}

NetworkCore::~NetworkCore() 
{
}

bool NetworkCore::StartNetwork(std::string const& _bind_ip, uint16 _port, int _worker_cnt)
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
		auto _worker = new NetworkThread();
		_network_threads.emplace_back(_worker);

		_worker->Start();
	}

	this->accept();
	return true;
}
void NetworkCore::StopNetwork()
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

void NetworkCore::accept()
{
	uint32 _min_network_thread_index = 0;

	for (int _worker_index = 0; _worker_index < _network_threads.size(); ++_worker_index)
	{
		if (_network_threads[_worker_index]->GetConnectionCount() < _network_threads[_min_network_thread_index]->GetConnectionCount())
			_min_network_thread_index = _worker_index;
	}

	tcp::socket* _accept_sock = _network_threads[_min_network_thread_index]->GetSocketForAccept();

	_acceptor->accept(*_accept_sock
		// accept handler
		, [this, _accept_sock, _min_network_thread_index](boost::system::error_code error)
		{
			if (!error)
			{
				// 비동기 소켓으로 생성
				_accept_sock->non_blocking(true);

				NetworkSessionPtr _new_network_session = std::make_shared<Session>(std::move(*_accept_sock));
				_network_threads[_min_network_thread_index]->AddNewSession(_new_network_session);
				_new_network_session->Start();
			}

			if (!_acceptor->is_closed())
			{
				this->accept();
			}
		}
	);
}