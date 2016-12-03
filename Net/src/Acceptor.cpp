#include "Acceptor.h"

using namespace TerraX;

Acceptor::Acceptor(EventLoop* pLoop, int port, uint16_t max_connections)
	: m_Acceptor(pLoop, port, max_connections) {
}

void Acceptor::ForceClose(NetChannelPtr& channel) {
	m_Acceptor.ForceClose(channel);
}

void Acceptor::SendPacket(NetChannelPtr& channel, google::protobuf::Message& packet) {
	//channel->SendMsg(1, packet);
}

//template<class Packet>
//void Acceptor::SendPacket(int32_t peer_info, Packet& packet) {
//channel->SendMsg(1, packet);
//}