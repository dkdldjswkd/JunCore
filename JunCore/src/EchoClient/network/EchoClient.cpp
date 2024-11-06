#include "EchoClient.h"

EchoClient& EchoClient::Instance()
{
	static EchoClient _instance;
	return _instance;
}

void EchoClient::OnConnect(SessionPtr session_ptr)
{
	std::cout << "EchoClient::OnConnect()" << std::endl;
	session_ptr_ = session_ptr;
}