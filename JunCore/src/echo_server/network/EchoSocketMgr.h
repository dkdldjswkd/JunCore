#ifndef __WORLDSOCKETMGR_H
#define __WORLDSOCKETMGR_H

#include <network/SocketMgr.h>
#include "EchoSocket.h"

class EchoSocketMgr : public SocketMgr<EchoSocket>
{
	typedef SocketMgr<EchoSocket> BaseSocketMgr;

public:
	static EchoSocketMgr& Instance();

	bool StartEchoNetwork(std::string const& bindIp, uint16 port, int networkThreads);

	void StopNetwork() override;

	std::size_t GetApplicationSendBufferSize() const { return _socketApplicationSendBufferSize; }

protected:
	EchoSocketMgr();

private:
	int32 _socketApplicationSendBufferSize;
};

#define sEchoSocketMgr EchoSocketMgr::Instance()

#endif