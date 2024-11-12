#include "NetworkThread.h"

NetworkThread::NetworkThread() : connections_(0), stopped_(false), thread_(nullptr), io_context_(1), accept_socket_(io_context_), connect_socket_(io_context_), update_timer_(io_context_)
{
}

NetworkThread::~NetworkThread()
{
	Stop();
	if (thread_)
	{
		Wait();
		delete thread_;
	}
}

bool NetworkThread::Start()
{
	if (thread_)
		return false;

	thread_ = new std::thread(&NetworkThread::Run, this);
	return true;
}

void NetworkThread::Run()
{
	// TC_LOG_DEBUG("misc", "Network Thread Starting");
	update_timer_.expires_from_now(boost::posix_time::milliseconds(1));
	update_timer_.async_wait([this](boost::system::error_code const&) { Update(); });
	io_context_.run();

	// TC_LOG_DEBUG("misc", "Network Thread exits");
	new_session_vec_.clear();
	active_session_vec_.clear();
}

void NetworkThread::Update()
{
	if (stopped_)
		return;

	update_timer_.expires_from_now(boost::posix_time::milliseconds(1));
	update_timer_.async_wait([this](boost::system::error_code const&) { Update(); });

	////////////////////////
	// New Session 처리
	////////////////////////

	do
	{
		std::lock_guard<std::mutex> lock(new_session_lock);

		if (new_session_vec_.empty())
		{
			break;
		}

		for (auto _session : new_session_vec_)
		{
			if (!_session->IsOpen())
			{
				// callback 고려, SocketRemoved
				--connections_;
			}
			else
			{
				active_session_vec_.emplace_back(_session);
			}
		}

		new_session_vec_.clear();
	} 
	while (false);

	////////////////////////
	// Session Update
	////////////////////////

	active_session_vec_.erase(
		std::remove_if(active_session_vec_.begin(), active_session_vec_.end()
			, [this](SessionPtr network_session_ptr)
			{
				if (network_session_ptr->Update() == false)
				{
					if (network_session_ptr->IsOpen())
					{
						network_session_ptr->CloseSocket();
					}

					// callbck 고려 SocketRemoved

					--this->connections_;
					return true;
				}

				return false;
			}
		)
		, active_session_vec_.end()
	);
}

void NetworkThread::Stop()
{
	stopped_ = true;
	io_context_.stop();
}

void NetworkThread::Wait()
{
	//ASSERT(_thread);

	thread_->join();
	delete thread_;
	thread_ = nullptr;
}

int32 NetworkThread::GetConnectionCount() const
{
	return connections_;
}

void NetworkThread::AddNewSession(SessionPtr _new_network_session_ptr)
{
	std::lock_guard<std::mutex> lock(new_session_lock);

	++connections_;
	new_session_vec_.emplace_back(_new_network_session_ptr);
	// callback 고려
}

tcp::socket* NetworkThread::GetSocketForAccept() 
{ 
	return &accept_socket_; 
}

tcp::socket* NetworkThread::GetSocketForConnect() 
{ 
	return &connect_socket_; 
}