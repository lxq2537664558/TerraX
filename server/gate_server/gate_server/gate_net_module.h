#pragma once

#include "base/types.h"
#include "srv/net/net_base_module.h"
#include "srv/net/server_conn_service.h"
namespace terra
{
	class ServerConnService;
	class GateNetModule final : public NetBaseModule
	{
		DISABLE_COPY(GateNetModule);
		MAKE_INSTANCE(GateNetModule);
	private:
		std::unique_ptr<ServerConnService> conn_service_;
	public:
		GateNetModule() = default;
		~GateNetModule() = default;

		bool Init();
		bool AfterInit();
		bool Execute();
		bool BeforeShut();
		bool Shut();
	private:
		void InitGateNetInfo();
		void StartConnectWorldServer();
		//void AcceptClient() {};
		//void OnClientSocketEvent(TcpConnection* conn, ConnState_t conn_state) {};
		//void OnClientMessage(TcpConnection* conn, evbuffer* evbuf) {};




	};
}