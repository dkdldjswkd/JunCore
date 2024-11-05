#include "EchoClient.h"

EchoClient& EchoClient::Instance()
{
	static EchoClient _instance;
	return _instance;
}