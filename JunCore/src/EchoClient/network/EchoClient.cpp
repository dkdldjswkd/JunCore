#include "EchoClient.h"
#include "../packet/packet.pb.h"

EchoClient& EchoClient::Instance()
{
	static EchoClient _instance;
	return _instance;
}

void EchoClient::OnConnect(SessionPtr session_ptr)
{
	session_ptr_ = session_ptr;
}

int g_last_number = -1;

bool RandomStringChecker(std::string _str)
{
	// 문자열이 비었으면 안되고,
	if (_str.empty())
	{
		return false;
	}

	int _prev_num = g_last_number;

	for (int i = 0; i < _str.size(); ++i)
	{
		int _num = _str[i] - 0x30;

		if ((_prev_num + 1) % 10 != _num)
		{
			return false;
		}

		_prev_num = _num;
	}

	g_last_number = _prev_num;
	return true;
}

void EchoClient::InitPacketHandlers()
{
    RegisterPacketHandler<PacketLib::GU_ECHO_RES>(
        // packet id
        101 // PacketLib::PACKET_ID::PACKET_ID_GU_ECHO_RES

        // packet handler
        , [this](SessionPtr _session_ptr, const PacketLib::GU_ECHO_RES& _packet) 
        {
			// 채팅 클라이언트
            //std::cout << "??? : " << _packet.echo() << std::endl;

			if (false == RandomStringChecker(_packet.echo()))
			{
				std::cout << "error !!" << std::endl;
				std::cout << _packet.echo() << std::endl;
				exit(0);
			}
        }
    );
}