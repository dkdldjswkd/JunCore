#include <iostream>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <thread>
#include "network/EchoClient.h"
using namespace std;

void main()
{
	cout << "EchoClient" << "오후 6:08 2024-11-05" << endl;

	// 서버 연결 정보 설정
	std::string		_ip		= "0.0.0.0"; // sConfigMgr->GetStringDefault("BindIP", "0.0.0.0");
	uint16			_port	= 8085;		 //  uint16(sWorld->getIntConfig(CONFIG_PORT_WORLD));
	tcp::endpoint	_endpoint(boost::asio::ip::address::from_string(_ip), _port);

	// 워커 스레드 개수 설정
	int32 networkThreads = 1; // sConfigMgr->GetIntDefault("Network.Threads", 1);

	sEchoClient.StartClient(networkThreads);
	sEchoClient.Connect(_endpoint);

	//if (!sEchoSocketMgr.StartNetwork(_ip, _port, networkThreads))
	//{
	//	return 1;
	//}

	while (true)
	{
		std::string _input_str;
		std::cin >> _input_str;
		if (_input_str.compare("close") == 0)
			break;
	}
}