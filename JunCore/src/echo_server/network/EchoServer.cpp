#include "EchoServer.h"

EchoServer& EchoServer::Instance()
{
	static EchoServer _instance;
	return _instance;
}

void EchoServer::OnAccept(SessionPtr session_ptr)
{
    std::lock_guard<std::mutex> _lock(session_mutex_);
    session_set_.emplace(session_ptr);
}

void EchoServer::OnSessionClose(SessionPtr session_ptr)
{
    std::lock_guard<std::mutex> _lock(session_mutex_);
    session_set_.erase(session_ptr);
}

void EchoServer::InitPacketHandlers() 
{
    RegisterPacketHandler<PacketLib::UG_ECHO_REQ>(
        // packet id
        1 // PacketLib::PACKET_ID::PACKET_ID_UG_ECHO_REQ

        // packet handler
        , [this](SessionPtr _session_ptr, const PacketLib::UG_ECHO_REQ& _packet) {
            std::cout  << ">> " << _packet.echo() << std::endl;

			PacketLib::GU_ECHO_RES _res;
            _res.set_echo(_packet.echo());

             //핑퐁
			 _session_ptr->SendPacket(101 /*PacketLib::PACKET_ID::PACKET_ID_GU_ECHO_RES*/, _res);

    //        // 채팅서버 용
    //        {
				//std::lock_guard<std::mutex> _lock(session_mutex_);
    //            for (auto _el_session_ptr : session_set_)
    //            {
    //                if (_el_session_ptr == _session_ptr)
    //                    continue;

    //                _el_session_ptr->SendPacket(101 /*PacketLib::PACKET_ID::PACKET_ID_GU_ECHO_RES*/, _res);
    //            }
    //        }
        }
    );
}