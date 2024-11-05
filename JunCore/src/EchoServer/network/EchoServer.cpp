#include "EchoServer.h"

EchoServer& EchoServer::Instance()
{
	static EchoServer _instance;
	return _instance;
}

void EchoServer::OnAccept(SessionPtr session_ptr)
{
	// ...
}