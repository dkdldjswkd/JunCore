#include <iostream>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <thread>
#include "network/EchoServer.h"
using namespace std;

int main()
{
	cout << "EchoServer" << endl;

	//// 서버 연결 정보 설정
	//std::string _ip   = "0.0.0.0";  // sConfigMgr->GetStringDefault("BindIP", "0.0.0.0");
	//uint16		_port = 8085;		//  uint16(sWorld->getIntConfig(CONFIG_PORT_WORLD));

	//// 워커 스레드 개수 설정
	//int32 networkThreads = 1; // sConfigMgr->GetIntDefault("Network.Threads", 1);

	//if (!sEchoSocketMgr.StartNetwork(_ip, _port, networkThreads))
	//{
	//	return 1;
	//}

	//while (true)
	//{
	//	std::string _input_str;
	//	std::cin >> _input_str;
	//	if (_input_str.compare("close") == 0)
	//		break;
	//}

	//// 에코 
	//std::shared_ptr<void> sWorldSocketMgrHandle(nullptr, [](void*) { sEchoSocketMgr.StopNetwork();});
	//return 0;
}