#include "WorldSocket.h"

WorldSocket::WorldSocket(tcp::socket&& socket) : Socket(std::move(socket)), _OverSpeedPings(0), _worldSession(nullptr), _authed(false), _sendBufferSize(4096)
{
	// todo
}

WorldSocket::~WorldSocket() = default;

void WorldSocket::Start()
{
	// todo
}

bool WorldSocket::Update()
{
	// todo
	return false;
}
