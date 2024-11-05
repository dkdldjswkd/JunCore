#ifndef SocketMgr_h__
#define SocketMgr_h__

#include "AsyncAcceptor.h"
#include "Errors.h"
#include "NetworkThread.h"
#include "Session.h"
#include <boost/asio/ip/tcp.hpp>
#include <memory>

using boost::asio::ip::tcp;

class NetworkManager
{
public:
	NetworkManager();
	virtual ~NetworkManager();

public:
	bool StartServer(std::string const& _bind_ip, uint16 _port, int _worker_cnt);
	void StopServer();

	// todo 수정 반드시 필요
	bool StartClient(int _worker_cnt);

private:
	void Accept();
	virtual void OnAccept(SessionPtr session_ptr);

public:
	void Connect(const tcp::endpoint& endpoint);
	virtual void OnConnect(SessionPtr session_ptr);

private:
	boost::asio::io_context* io_context_ = nullptr;
	AsyncAcceptor* acceptor_ = nullptr;
	std::vector<NetworkThread*> network_threads_;
};

#endif // SocketMgr_h__