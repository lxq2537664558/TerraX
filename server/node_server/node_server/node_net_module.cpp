#include "node_net_module.h"

#include "srv/net/server_login_req_service.h"

using namespace terra;

bool NodeNetModule::Init()
{
	std::unique_ptr<ServerConnService> conn_service(new ServerConnService(*this));
	std::unique_ptr<ServerLoginReqService> login_req_service(new ServerLoginReqService(*conn_service, PeerType_t::NODESERVER));
	conn_service->InitLoginReqService(login_req_service);
    return true;
}
bool NodeNetModule::AfterInit()
{
    conn_service_->Connect2World();
    return true;
}
bool NodeNetModule::Execute()
{
    loop_.loop();
    return true;
}
bool NodeNetModule::BeforeShut() { return true; }
bool NodeNetModule::Shut() { return true; }
