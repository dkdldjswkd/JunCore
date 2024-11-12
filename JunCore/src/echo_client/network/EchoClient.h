#ifndef _ECHO_CLIENT_H_
#define _ECHO_CLIENT_H_

#include <network/NetworkManager.h>

class EchoClient : public NetworkManager
{
public:
	static EchoClient& Instance();

public:
	virtual void OnConnect(SessionPtr session_ptr) override;

	virtual void InitPacketHandlers() override;

public:
	SessionPtr session_ptr_ = nullptr;
};

#define sEchoClient EchoClient::Instance()
#endif