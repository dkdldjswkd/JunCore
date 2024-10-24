#ifndef __WORLDSOCKETMGR_H
#define __WORLDSOCKETMGR_H

#include <network/SocketMgr.h>
#include "EchoSocket.h"

class EchoSocketMgr : public SocketMgr<EchoSocket>
{
	typedef SocketMgr<EchoSocket> BaseSocketMgr;

public:
	static EchoSocketMgr& Instance();

};

#define sEchoSocketMgr EchoSocketMgr::Instance()

#endif