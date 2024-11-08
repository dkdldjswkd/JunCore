#include "EchoClient.h"
#include "../packet/packet.pb.h"

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

void EchoClient::InitPacketHandlers()
{
    RegisterPacketHandler<PacketLib::GU_ECHO_RES>(
        // packet id
        101 // PacketLib::PACKET_ID::PACKET_ID_GU_ECHO_RES

        // packet handler
        , [this](SessionPtr _session_ptr, const PacketLib::GU_ECHO_RES& _packet) 
        {
            std::cout << "recv : " << _packet.echo() << std::endl;
        }
    );
}