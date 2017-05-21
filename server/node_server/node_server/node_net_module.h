#pragma once

#include "base/types.h"
#include "base/module_interface.h"
#include "srv/net/net_base_module.h"
#include "srv/net/server_conn_service.h"
namespace terra
{
	class ServerConnService;
	class NodeNetModule final : public NetBaseModule
	{
		DISABLE_COPY(NodeNetModule);
		MAKE_INSTANCE(NodeNetModule);
	private:
		std::unique_ptr<ServerConnService> conn_service_;
	public:
		NodeNetModule() = default;
		~NodeNetModule() = default;

		bool Init();
		bool AfterInit();
		bool Execute();
		bool BeforeShut();
		bool Shut();
	private:

		//void AcceptClient() {};
		//void OnClientSocketEvent(TcpConnection* conn, ConnState_t conn_state) {};
		//void OnClientMessage(TcpConnection* conn, evbuffer* evbuf) {};




	};
}