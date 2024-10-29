#ifndef __WORLDSOCKETMGR_H
#define __WORLDSOCKETMGR_H

#include <network/NetworkCore.h>
#include "EchoSocket.h"

class EchoSocketMgr : public NetworkCore<EchoSocket>
{
	typedef NetworkCore<EchoSocket> BaseSocketMgr;

public:
	static EchoSocketMgr& Instance();

};

#define sEchoSocketMgr EchoSocketMgr::Instance()

#endif