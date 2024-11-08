#include <iostream>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <thread>
#include "network/EchoClient.h"
#include "packet/packet.pb.h"
using namespace std;

std::string ConvertToUTF8(const std::string& str);

void main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	// 서버 연결 정보 설정
	std::string		_ip = "10.10.1.213"; // "127.0.0.1"; // sConfigMgr->GetStringDefault("BindIP", "0.0.0.0");
	uint16			_port	= 8085;		 //  uint16(sWorld->getIntConfig(CONFIG_PORT_WORLD));
	tcp::endpoint	_endpoint(boost::asio::ip::address::from_string(_ip), _port);

	// 워커 스레드 개수 설정
	int32 networkThreads = 1; // sConfigMgr->GetIntDefault("Network.Threads", 1);

	sEchoClient.StartClient(networkThreads);
	sEchoClient.Connect(_endpoint);

	while (true)
	{
		std::cout << ">> ";

#ifdef _WIN32
		// Windows의 경우 getline 사용
		std::string _input_str;
		std::getline(std::cin, _input_str);
#else
		// Linux/Unix 시스템
		std::string _input_str;
		std::getline(std::cin, _input_str);
#endif

		if (_input_str.compare("close") == 0)
		{
			break;
		}
		else
		{
			PacketLib::UG_ECHO_REQ _echo_req;
			_echo_req.set_echo(std::move(ConvertToUTF8(_input_str)));
			sEchoClient.session_ptr_->SendPacket(1, _echo_req);
		}
	}
}

std::string ConvertToUTF8(const std::string& str) {
	// 현재 시스템 코드 페이지의 문자열을 UTF-16으로 변환
	int wlen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	if (wlen <= 0) return str;

	std::wstring wstr(wlen, 0);
	if (!MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], wlen)) {
		return str;
	}

	// UTF-16에서 UTF-8로 변환
	int utf8len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (utf8len <= 0) return str;

	std::string utf8str(utf8len, 0);
	if (!WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8str[0], utf8len, nullptr, nullptr)) {
		return str;
	}

	// 끝의 널 문자 제거
	if (!utf8str.empty() && utf8str.back() == '\0') {
		utf8str.pop_back();
	}

	return utf8str;
}