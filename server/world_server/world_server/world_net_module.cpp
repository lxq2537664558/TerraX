#include "world_net_module.h"

#include "srv/net/server_login_ack_service.h"
#include "comm/config/server_config.h"
using namespace terra;

void WorldNetModule::InitWorldNetInfo()
{
	ServerConfig::GetInstance().LoadConfigFromJson("world_server.json");
	std::string ip;
	int port;
	ServerConfig::GetInstance().GetJsonObjectValue("net", "listen_ip", ip);
	ServerConfig::GetInstance().GetJsonObjectValue("net", "listen_port", port);
	InitListenInfo(ip, port);
}

bool WorldNetModule::Init()
{
	InitWorldNetInfo();
	std::unique_ptr<ServerAcceptService> accept_service(new ServerAcceptService(*this));
	std::unique_ptr<ServerLoginAckService> login_ack_service(new ServerLoginAckService(*accept_service, PeerType_t::WORLDSERVER, 64));
	accept_service->InitLoginAckService(login_ack_service);
	accept_service_ = std::move(accept_service);
	return true;
}
bool WorldNetModule::AfterInit()
{
	accept_service_->AcceptConnection(get_listen_port(), 64);
	return true;
}
bool WorldNetModule::Execute()
{
	get_event_loop()->loop();;
	return true;
}
bool WorldNetModule::BeforeShut()
{
	return true;
}
bool WorldNetModule::Shut()
{
	return true;
}
