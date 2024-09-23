#ifndef __WORLDSOCKETMGR_H
#define __WORLDSOCKETMGR_H

#include <Networking/SocketMgr.h>

class WorldSocket;

class WorldSocketMgr : public SocketMgr<WorldSocket>
{
	using BaseSocketMgr = SocketMgr<WorldSocket>;

public:
};

#define sWorldSocketMgr WorldSocketMgr::Instance()

#endif