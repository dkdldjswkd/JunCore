﻿#include <iostream>
using namespace std;

int main()
{
	//// io_context 생성
	//std::shared_ptr<boost::asio::io_context> _io_context = std::make_shared<boost::asio::io_context>();

	//// 스레드 개수 설정
	//int _num_threads = 1 /*sConfigMgr->GetIntDefault("ThreadPool", 1)*/;
	//_num_threads = _num_threads < 1 ? 1 : _num_threads;

	//std::shared_ptr<boost::asio::thread_pool> _thread_pool = std::make_shared<boost::asio::thread_pool>(std::thread::hardware_concurrency());

	//// 스레드 생성
	//for (int i = 0; i < _num_threads; ++i)
	//{
	//	boost::asio::post(*_thread_pool, [_io_context]() { _io_context->run(); });
	//}

	//// io_context 종료 핸들러 설정
	//std::shared_ptr<void> _io_context_stop_handler(nullptr, [_io_context](void*) { _io_context->stop(); });

	//// 서버 연결 정보 설정
	//std::string _ip = "0.0.0.0";  // sConfigMgr->GetStringDefault("BindIP", "0.0.0.0");
	//uint16		_port = 8085;		//  uint16(sWorld->getIntConfig(CONFIG_PORT_WORLD));

	//// 워커 스레드 개수 설정
	//int32 networkThreads = 1;               // sConfigMgr->GetIntDefault("Network.Threads", 1);

	//if (!sEchoSocketMgr.StartEchoNetwork(*_io_context, _ip, _port, networkThreads))
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
	//std::shared_ptr<void> sWorldSocketMgrHandle(nullptr, [](void*) { sEchoSocketMgr.StopNetwork(); });

	//_io_context_stop_handler.reset();
	//_thread_pool.reset();
	//return 0;
}