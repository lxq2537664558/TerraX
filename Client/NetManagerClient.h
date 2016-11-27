#pragma once
#include "ComDef.h"
#include "NetDefine.h"
#include "Connector.h"
#include "EventLoop.h"

namespace TerraX
{
	class NetManagerClient
	{
		NOCOPY(NetManagerClient);
		MAKEINSTANCE(NetManagerClient);
	public:
		NetManagerClient() = default;
		~NetManagerClient() = default;

		void Connect(const std::string& host, int port, bool bGate);
		void SendPacket2Server(int32_t peer_info, google::protobuf::Message& packet);
		void Tick() { m_loop.loop();}

		void OnGateServer_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent);
		void OnLoginServer_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent);
	private:
		EventLoop m_loop;
		std::unique_ptr<Connector> m_pConnector;
	};

}