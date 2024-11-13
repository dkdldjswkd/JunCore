#pragma once
#include <boost/asio/ip/tcp.hpp>
#include <common/types.h>
#include <atomic>
#include <functional>

using boost::asio::ip::tcp;

class AsyncAcceptor
{
public:
	AsyncAcceptor(boost::asio::io_context& _io_context, std::string const& _bind_ip, uint16 _port);
	~AsyncAcceptor();

public:
	bool Listen();
	void AsyncAccept(tcp::socket& _socket, std::function<void(boost::system::error_code)> _accept_handler);
	void Close();
	bool IsClosed() const;

private:
	tcp::acceptor _acceptor;
	tcp::endpoint _endpoint;
	std::atomic<bool> _closed;
};