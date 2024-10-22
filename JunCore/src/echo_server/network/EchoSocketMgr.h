#ifndef __WORLDSOCKETMGR_H
#define __WORLDSOCKETMGR_H

#include <network/SocketMgr.h>
#include "EchoSocket.h"

class EchoSocketMgr : public SocketMgr<EchoSocket>
{
	typedef SocketMgr<EchoSocket> BaseSocketMgr;

public:
	static EchoSocketMgr& Instance();

	bool StartEchoNetwork(boost::asio::io_context& ioContext, std::string const& bindIp, uint16 port, int networkThreads);

	void StopNetwork() override;

	void OnSocketOpen(tcp::socket&& sock, uint32 threadIndex) override;

	std::size_t GetApplicationSendBufferSize() const { return _socketApplicationSendBufferSize; }

protected:
	EchoSocketMgr();

	NetworkThread<EchoSocket>* CreateThreads() const override;

private:
	int32 _socketSystemSendBufferSize;		// 의미 파악 필요
	int32 _socketApplicationSendBufferSize; // 의미 파악 필요
	bool _tcpNoDelay;
};

#define sEchoSocketMgr EchoSocketMgr::Instance()

#endif