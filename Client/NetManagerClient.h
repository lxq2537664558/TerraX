#pragma once

#include "ComDef.h"
#include "EventLoop.h"
#include "NetChannel.h"
#include "NetDefine.h"
#include "NetServer.h"
#include "Packet.h"

namespace TerraX
{
	enum class MessageError_t {
		eNoError,
		eInvalidLength,
	};

	namespace gpb = google::protobuf;
	class NetManagerClient
	{
		NOCOPY(NetManagerClient);
		MAKEINSTANCE(NetManagerClient);

	protected:
		NetManagerClient();

	public:
		virtual ~NetManagerClient() = default;

		void Connect(const std::string& host, int port);
		void Tick();
		NetChannelPtr GetChannel_BackEnd() { return m_pBackEnd; }
		void SendPacket(PeerType_t peer_type, gpb::Message& msg);
	private:
		MessageError_t ReadMessage(struct evbuffer* evbuf, PacketQueue& pktQueue);
		void ProcessMessage(evbuffer* evbuf, NetChannelPtr& pChannel, PacketQueue& pktQueue,
			std::function<void(NetChannelPtr&, Packet*)> fn);
		void OnMessage_BackEnd(struct evbuffer* evbuf, NetChannelPtr& pChannel);

		virtual void ForwardPacket2BackEnd(NetChannelPtr& pFrontChannel, Packet* pkt);

		void OnNetEvent_BackEnd(NetChannelPtr& pChannel, NetEvent_t eEvent);

	protected:
		virtual void DoBackEnd_Connected(NetChannelPtr& pChannel);
		virtual void DoBackEnd_Disconnected(NetChannelPtr& pChannel);
		virtual void DoBackEnd_ConnBreak(NetChannelPtr& pChannel);

	protected:
		const PeerType_t m_peer_type{ PeerType_t::undefine };
		EventLoop m_loop;
		NetChannelPtr m_pBackEnd;
		PacketQueue m_pktQueueBackEnd;
	};
}