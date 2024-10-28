#ifndef __ASYNCACCEPT_H_
#define __ASYNCACCEPT_H_

#include <boost/asio/ip/tcp.hpp>
#include <Define.h>
#include <atomic>
#include <functional>

using boost::asio::ip::tcp;

class AsyncAcceptor
{
public:
	AsyncAcceptor(boost::asio::io_context& _io_context, std::string const& _bind_ip, uint16 _port);
	~AsyncAcceptor();

public:
	bool Bind();
	inline void async_accept(tcp::socket& _socket, std::function<void(boost::system::error_code)> _accept_handler);
	void close();
	bool is_closed() const;

private:
	tcp::acceptor _acceptor;
	tcp::endpoint _endpoint;
	std::atomic<bool> _closed;
};

inline void AsyncAcceptor::async_accept(tcp::socket& _socket, std::function<void(boost::system::error_code)> _accept_handler)
{
	_acceptor.async_accept(_socket, _accept_handler);
}
#endif