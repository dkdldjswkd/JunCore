#ifndef _ECHO_SERVER_H_
#define _ECHO_SERVER_H_
#include <network/NetworkManager.h>
#include "../packet/packet.pb.h"
#include <iostream>

class EchoServer : public NetworkManager
{
public:
	static EchoServer& Instance();

public:
	virtual void OnAccept(SessionPtr session_ptr) override;

	virtual void InitPacketHandlers() override;
};

#define sEchoServer EchoServer::Instance()
#endif