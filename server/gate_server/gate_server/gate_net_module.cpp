#include "gate_net_module.h"

#include "srv/net/server_login_req_service.h"
#include "comm/config/server_config.h"

using namespace terra;


void GateNetModule::InitGateNetInfo()
{
	ServerConfig::GetInstance().LoadConfigFromJson("gate_server.json");

	std::string conn_ip;
	int conn_port;
	ServerConfig::GetInstance().GetJsonObjectValue("net", "world_server_ip", conn_ip);
	ServerConfig::GetInstance().GetJsonObjectValue("net", "world_server_port", conn_port);
	InitConnectInfo(conn_ip, conn_port);

	std::string listen_ip;
	int listen_port;
	ServerConfig::GetInstance().GetJsonObjectValue("net", "listen_ip", listen_ip);
	ServerConfig::GetInstance().GetJsonObjectValue("net", "listen_port", listen_port);
	InitListenInfo(listen_ip, listen_port);
}

void GateNetModule::StartConnectWorldServer()
{

}

bool GateNetModule::Init()
{
	InitGateNetInfo();
	std::unique_ptr<ServerConnService> conn_service(new ServerConnService(*this));
	std::unique_ptr<ServerLoginReqService> login_req_service(new ServerLoginReqService(*conn_service, PeerType_t::GATESERVER));
	conn_service->InitLoginReqService(login_req_service);
	conn_service_ = std::move(conn_service);
    return true;
}
bool GateNetModule::AfterInit()
{
    conn_service_->Connect2World();
    return true;
}
bool GateNetModule::Execute()
{
    get_event_loop()->loop();
    return true;
}
bool GateNetModule::BeforeShut() { return true; }
bool GateNetModule::Shut() { return true; }
