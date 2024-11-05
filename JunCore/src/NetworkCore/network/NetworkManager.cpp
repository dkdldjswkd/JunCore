#include "NetworkManager.h"
#include "Session.h"

NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager() 
{
}

bool NetworkManager::StartServer(std::string const& bind_ip, uint16 port, int worker_cnt)
{
	io_context_ = new boost::asio::io_context;
	acceptor_	= new AsyncAcceptor(*io_context_, bind_ip, port);

	// CHECK_RETURN(threadCount > 0, false);

	if (!acceptor_->Bind())
	{
		// LOG_ERROR
		delete acceptor_;
		return false;
	}

	network_threads_.reserve(worker_cnt);

	for (int i = 0; i < worker_cnt; ++i)
	{
		auto worker = new NetworkThread();
		network_threads_.emplace_back(worker);

		worker->Start();
	}

	this->Accept();
	return true;
}

void NetworkManager::StopServer()
{
	// 1. acceptor 종료
	acceptor_->close();

	// 2. acceptor ioconext stop
	io_context_->stop();

	// 3. acceptor 정리
	delete acceptor_;
	acceptor_ = nullptr;

	// 4. acceptor ioconext 정리
	delete io_context_;
	io_context_ = nullptr;

	// 5. workers ioconext stop
	for (const auto& worker : network_threads_)
	{
		worker->Stop();
	}

	// 6. workers join wait
	for (const auto& worker : network_threads_)
	{
		worker->Wait();
	}
}

void NetworkManager::Accept()
{
	uint32 min_network_thread_index = 0;

	for (int worker_index = 0; worker_index < network_threads_.size(); ++worker_index)
	{
		if (network_threads_[worker_index]->GetConnectionCount() < network_threads_[min_network_thread_index]->GetConnectionCount())
			min_network_thread_index = worker_index;
	}

	tcp::socket* accept_sock = network_threads_[min_network_thread_index]->GetSocketForAccept();

	acceptor_->accept(*accept_sock
		// accept handler
		, [this, accept_sock, min_network_thread_index](boost::system::error_code error)
		{
			if (!error)
			{
				// 비동기 소켓으로 생성
				accept_sock->non_blocking(true);

				SessionPtr new_network_session = std::make_shared<Session>(std::move(*accept_sock));
				network_threads_[min_network_thread_index]->AddNewSession(new_network_session);
				new_network_session->Start();

				// 사용자 재정의 callback
				OnAccept(new_network_session);
			}

			if (!acceptor_->is_closed())
			{
				this->Accept();
			}
		}
	);
}

void NetworkManager::OnAccept(SessionPtr session_ptr)
{
}

bool NetworkManager::StartClient(int worker_cnt)
{
	// CHECK_RETURN(threadCount > 0, false);

	network_threads_.reserve(worker_cnt);

	for (int i = 0; i < worker_cnt; ++i)
	{
		auto worker = new NetworkThread();
		network_threads_.emplace_back(worker);

		worker->Start();
	}

	return true;
}

void NetworkManager::Connect(const tcp::endpoint& endpoint)
{
	// server session의 경우 server_session_flag = true 등 처리 필요?
	//		ㄴ server session인걸 알아야하는 경우가 있나?

	// server session의 경우 client session과 worker thread 분리 필요?
	//		ㄴ client session의 메시지가 server session의 메시지 처리에 영향을 주지 않게 하기위해서 분리 필요
 
	////////////////////////////////////////////////////////////////////////

	uint32 min_network_thread_index = 0;

	for (int worker_index = 0; worker_index < network_threads_.size(); ++worker_index)
	{
		if (network_threads_[worker_index]->GetConnectionCount() < network_threads_[min_network_thread_index]->GetConnectionCount())
			min_network_thread_index = worker_index;
	}

	tcp::socket* connect_sock = network_threads_[min_network_thread_index]->GetSocketForConnect();

	connect_sock->async_connect(endpoint,
		[this, &connect_sock, min_network_thread_index](boost::system::error_code ec) {
			if (!ec)
			{
				// 비동기 소켓으로 설정
				// connect_sock->non_blocking(true);

				// todo 고려
				SessionPtr new_network_session = std::make_shared<Session>(std::move(*connect_sock));
				network_threads_[min_network_thread_index]->AddNewSession(new_network_session);
				new_network_session->Start();

				// 사용자 제정의 callback
				OnConnect(new_network_session);
			}
			else
			{
				// Timer에 reconnect 시도 등록
				// ...
			}
		}
	);
}

void NetworkManager::OnConnect(SessionPtr session_ptr)
{
}