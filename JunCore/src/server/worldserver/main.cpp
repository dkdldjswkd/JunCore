#include <iostream>

// common.lib build test
#include <Common.h>

// shared.lib build test
#include <Networking/AsyncAcceptor.h>
#include <Networking/NetworkThread.h>
#include <Networking/Socket.h>
#include <Networking/SocketMgr.h>

// game.lib build test
#include <Server/WorldPacket.h>
#include <Server/WorldSession.h>
#include <Server/WorldSocket.h>
#include <Server/WorldSocketMgr.h>

using namespace std;

/*
SocketMgr 구현 해야함.
*/

int main() 
{
	f_Common_h();
	return 0;
}