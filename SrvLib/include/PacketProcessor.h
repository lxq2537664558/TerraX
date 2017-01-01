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
	class PacketProcessor
	{
		NOCOPY(PacketProcessor);

	protected:
		PacketProcessor(PeerType_t peer);

	public:
		virtual ~PacketProcessor() = default;

		void Connect(const std::string& host, int port);
		void Accept(int port, uint16_t max_connections);

		void Tick();

		NetChannelPtr GetChannel_FrontEnd(int32_t nChannelInfo);
		NetChannelPtr GetChannel_BackEnd() { return m_pBackEnd; }

		void SendPacket2Client(int channel_info, int dest_info, int owner_info, gpb::Message& msg);
	protected:
		void SendPacket2BackEnd(int dest_info, int owner_info, gpb::Message& msg);
		void SendPacket2FrontEnd(int channel_info, int dest_info, int owner_info, gpb::Message& msg);
	private:
		void SendPacketByChannel(NetChannelPtr& pChannel, int dest_info, int owner_info, gpb::Message& msg);

		MessageError_t ReadMessage(struct evbuffer* evbuf, PacketQueue& pktQueue);
		void ProcessMessage(evbuffer* evbuf, NetChannelPtr& pChannel, PacketQueue& pktQueue,
			std::function<void(NetChannelPtr&, Packet*)> fn);

		void OnMessage_FrontEnd(struct evbuffer* evbuf, NetChannelPtr& pChannel);
		void OnMessage_BackEnd(struct evbuffer* evbuf, NetChannelPtr& pChannel);

		virtual void ForwardPacket2FrontEnd(NetChannelPtr& pBackChannel, Packet* pkt);
		virtual void ForwardPacket2BackEnd(NetChannelPtr& pFrontChannel, Packet* pkt);

		void OnNetEvent_FrontEnd(NetChannelPtr& pChannel, NetEvent_t eEvent);
		void OnNetEvent_BackEnd(NetChannelPtr& pChannel, NetEvent_t eEvent);

	protected:
		virtual void DoBackEnd_Connected(NetChannelPtr& pChannel);
		virtual void DoBackEnd_Disconnected(NetChannelPtr& pChannel);
		virtual void DoBackEnd_ConnBreak(NetChannelPtr& pChannel);

		virtual void DoFrontEnd_Connected(NetChannelPtr& pChannel);
		virtual void DoFrontEnd_Disconnected(NetChannelPtr& pChannel);
		virtual void DoFrontEnd_ConnBreak(NetChannelPtr& pChannel);

	protected:
		const PeerType_t m_peer_type{ PeerType_t::undefine };
		EventLoop m_loop;
		NetChannelPtr m_pBackEnd;
		std::unique_ptr<NetServer> m_pFrontEnd;

		PacketQueue m_pktQueueFrontEnd;
		PacketQueue m_pktQueueBackEnd;
	};
}