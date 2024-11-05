#include <iostream>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <thread>
#include "network/EchoServer.h"
#include "packet/packet.pb.h"
using namespace std;

void main()
{
	cout << "EchoServer" << "오후 6:08 2024-11-05" << endl;

	PacketLib::UG_ECHO_REQ a;
	a.set_echo("PacketLib::UG_ECHO_REQ");
	cout << a.echo() << endl;

	// 서버 연결 정보 설정
	std::string _ip   = "0.0.0.0";  // sConfigMgr->GetStringDefault("BindIP", "0.0.0.0");
	uint16		_port = 8085;		//  uint16(sWorld->getIntConfig(CONFIG_PORT_WORLD));

	// 워커 스레드 개수 설정
	int32 networkThreads = 1; // sConfigMgr->GetIntDefault("Network.Threads", 1);

	if (!sEchoServer.StartServer(_ip, _port, networkThreads))
	{
		// LOG_ERROR
		return;
	}

	// 임시
	while (true)
	{
		std::string _input_str;
		std::cin >> _input_str;
		if (_input_str.compare("close") == 0)
			break;
	}

	sEchoServer.StopServer();
}