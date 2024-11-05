#ifndef _ECHO_CLIENT_H_
#define _ECHO_CLIENT_H_

#include <network/NetworkManager.h>

class EchoClient : public NetworkManager
{
public:
	static EchoClient& Instance();
};

#define sEchoClient EchoClient::Instance()
#endif