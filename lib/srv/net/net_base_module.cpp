#include "net_base_module.h"
#include "packet_processor.h"

using namespace terra;

void NetBaseModule::ProcessServerMessage(TcpConnection* conn, evbuffer* evbuf)
{
	PacketProcessor::GetInstance().ProcessServerPacket(conn, evbuf);
}

void NetBaseModule::InitConnectInfo(const std::string& ip, int port)
{
	conn_ip_ = ip;
	conn_port_ = port;
}

void NetBaseModule::InitListenInfo(const std::string& ip, int port)
{
	listen_ip_ = ip;
	listen_port_ = port;
}
