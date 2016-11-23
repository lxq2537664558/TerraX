#include "ConnectionManager.h"
#include "PacketDispatcher.h"
#include "CenterServer.h"
using namespace TerraX;

ConnectionManager::ConnectionManager(CenterServer& cs)
	: server(cs) {
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&ConnectionManager::OnMessage_Register, this, std::placeholders::_1, std::placeholders::_2)));
}

void ConnectionManager::OnMessage_Register(NetChannel& channel, PktRegisterServer& pkt) {
	int32_t server_info = pkt.server_info();
	PeerInfo pi;
	pi.parse(server_info);
	assert(channel.GetChannelIndex() != 0 && pi.channel_index == 0);
	assert(pi.peer_type > (uint16_t)PeerType_t::client && pi.peer_type < (uint16_t)PeerType_t::peer_count);
	channel.SetPeerType(pi.peer_type);

	std::cout << "Server: " << pi.server_name() <<
		"\t ChannelIndex: " << int32_t(channel.GetChannelIndex()) << std::endl;
}
