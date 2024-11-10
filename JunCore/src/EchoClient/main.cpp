#include <iostream>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <thread>
#include "network/EchoClient.h"
#include "packet/packet.pb.h"
using namespace std;

class RandomStringGenerator {
private:
	std::string numbers = "01234567890123456789012345678901234567890123456789012345678901234567890123456789";
	size_t current_pos = 0;
	std::random_device rd;
	std::mt19937 gen;

public:
	RandomStringGenerator() : gen(rd()) {}

	void get_random_str(std::string& _str) {
		// 1부터 9까지의 랜덤한 길이 생성 (최소 1, 최대 20)
		std::uniform_int_distribution<> len_dis(1, 20);
		int length = len_dis(gen);

		_str.clear();
		// 요청된 길이만큼 문자열 생성
		for (int i = 0; i < length; i++) {
			// 현재 위치가 끝에 도달하면 처음으로 돌아감
			if (current_pos >= numbers.length()) {
				current_pos = 0;
			}
			_str += numbers[current_pos++];
		}
	}
};

void main()
{
	// 서버 연결 정보 설정
	std::string		_ip = "127.0.0.1";	// "10.10.1.213"; // "127.0.0.1"; // sConfigMgr->GetStringDefault("BindIP", "0.0.0.0");
	uint16			_port	= 8085;		//  uint16(sWorld->getIntConfig(CONFIG_PORT_WORLD));
	tcp::endpoint	_endpoint(boost::asio::ip::address::from_string(_ip), _port);

	// 워커 스레드 개수 설정
	int32 networkThreads = 1; // sConfigMgr->GetIntDefault("Network.Threads", 1);

	sEchoClient.StartClient(networkThreads);
	sEchoClient.Connect(_endpoint);

	// 반복 문자열 생성기
	RandomStringGenerator generator;

	while (true)
	{
		std::string result;
		generator.get_random_str(OUT result);

		PacketLib::UG_ECHO_REQ _echo_req;
		_echo_req.set_echo(result);
		sEchoClient.session_ptr_->SendPacket(1, _echo_req);
	}

	// sEchoClient.StopClient();
}