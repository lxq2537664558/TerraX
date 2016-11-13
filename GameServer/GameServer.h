#pragma once
#include "ComDef.h"
#include "NetChannel.h"
#include "proto/server_packet.pb.h"
#include "NetDefine.h"

using namespace ServerPacket;
namespace TerraX
{
	class GameServer : public NetChannel
	{
		NOCOPY(GameServer);
	public:
		explicit GameServer(EventLoop* loop, const std::string& host, int port);
		template<class T>
		void SendPacket(T& packet) {
			SendMessage(0, packet);
		}
		void OnMessage_RegisterServerRet(NetChannel& channel, PktRegisterServer& pkt);

	protected:
		void ConnectFailed() override final;
		void Connected() override final;
		void Disconnected() override final;

	private:
		void RegisterServer();
		int32_t GetServerInfo() { return m_PeerInfo.serialize(); }

	private:
		PeerInfo m_PeerInfo{ PeerType_t::gameserver };
	};
}