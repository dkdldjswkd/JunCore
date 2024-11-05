#ifndef _ECHO_SERVER_H_
#define _ECHO_SERVER_H_

#include <network/NetworkManager.h>

class EchoServer : public NetworkManager
{
public:
	static EchoServer& Instance();

public:
	virtual void OnAccept(SessionPtr session_ptr) override;
};

#define sEchoServer EchoServer::Instance()
#endif