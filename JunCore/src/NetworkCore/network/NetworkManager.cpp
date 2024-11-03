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

void NetworkManager::Connect(const tcp::endpoint& endpoint)
{
	// NetworkThread에서 Session 가져오기
	boost::asio::io_context io_context;
	tcp::socket socket(io_context);
	auto session_ptr = std::make_shared<Session>(std::move(socket));

	// session에 server session flag 설정
	// ...

	session_ptr->socket_.async_connect(endpoint,
		[this, session_ptr](boost::system::error_code ec) {
			if (!ec)
			{
				// NetworkThread에 AddNewSession
				// session_ptr->StartClient()

				// 사용자 제정의 callback
				OnConnect(session_ptr);
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