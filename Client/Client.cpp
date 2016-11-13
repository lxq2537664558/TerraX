#include "Client.h"

using namespace TerraX;

Client::Client(EventLoop* loop, const std::string& host, int port) :
	NetChannel(loop, host, port)
{
	//you can use marco to wrapper it if you want;
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&Client::OnMessage_RegisterServerRet, this, std::placeholders::_1, std::placeholders::_2)));
}

void Client::ConnectFailed()
{
	NetChannel::ConnectFailed();
}

void Client::Connected()
{
	NetChannel::Connected();
	RegisterServer();
}

void Client::Disconnected()
{
	NetChannel::Disconnected();
}

void Client::RegisterServer()
{
	PktRegisterServer pkt;
	pkt.set_server_info(GetServerInfo());
	SendPacket(pkt);
}

void Client::OnMessage_RegisterServerRet(NetChannel& channel, PktRegisterServer& pkt) 
{
	int32_t server_info = pkt.server_info();
	PeerInfo pi;
	pi.parse(server_info);
	std::cout << "Server: " << int32_t(pi.peer_type) << "\tIndex: " << int32_t(pi.peer_index) <<
		"\t ChannelIndex: " << int32_t(pi.channel_index) << std::endl;
	if (pi.peer_index <= 0) {
		std::cout << "Register failed!" << std::endl;
		channel.ForceClose();
	}
}