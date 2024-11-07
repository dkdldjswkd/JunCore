#ifndef _ECHO_SERVER_H_
#define _ECHO_SERVER_H_
#include <network/NetworkManager.h>
#include "../packet/packet.pb.h"
#include <iostream>

class EchoServer : public NetworkManager
{
public:
	static EchoServer& Instance();

public:
	virtual void OnAccept(SessionPtr session_ptr) override;

    virtual void InitPacketHandlers() override {

        // 에코 패킷 핸들러 등록
        RegisterPacketHandler<PacketLib::UG_ECHO_REQ>(
            // packet id
            PacketLib::PACKET_ID::PACKET_ID_UG_ECHO_REQ

            // packet handler
            , [this](SessionPtr _session_ptr, const PacketLib::UG_ECHO_REQ& _packet) {
                std::cout << "recv UG_ECHO_REQ!!" << std::endl;
                std::cout << _packet.echo() << std::endl;
            }
        );
    }
};

#define sEchoServer EchoServer::Instance()
#endif