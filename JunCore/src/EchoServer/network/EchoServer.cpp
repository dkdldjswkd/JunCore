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

void EchoServer::InitPacketHandlers() 
{
    RegisterPacketHandler<PacketLib::UG_ECHO_REQ>(
        // packet id
        1 // PacketLib::PACKET_ID::PACKET_ID_UG_ECHO_REQ

        // packet handler
        , [this](SessionPtr _session_ptr, const PacketLib::UG_ECHO_REQ& _packet) {
            std::cout << "recv UG_ECHO_REQ!!" << std::endl;
            std::cout << _packet.echo() << std::endl;

			PacketLib::GU_ECHO_RES _res;
            _res.set_echo(_packet.echo());
			_session_ptr->SendPacket(101 /*PacketLib::PACKET_ID::PACKET_ID_GU_ECHO_RES*/, _res);
        }
    );
}