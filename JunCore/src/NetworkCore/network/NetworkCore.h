#ifndef SocketMgr_h__
#define SocketMgr_h__

#include "AsyncAcceptor.h"
#include "Errors.h"
#include "NetworkThread.h"
#include "Session.h"
#include <boost/asio/ip/tcp.hpp>
#include <memory>

using boost::asio::ip::tcp;

class NetworkCore
{
public:
	NetworkCore();
	virtual ~NetworkCore();

public:
	bool StartNetwork(std::string const& _bind_ip, uint16 _port, int _worker_cnt);
	void StopNetwork();

private:
	void accept();

private:
	boost::asio::io_context* _io_context = nullptr;
	AsyncAcceptor* _acceptor = nullptr;
	std::vector<NetworkThread*> _network_threads;
};

#endif // SocketMgr_h__