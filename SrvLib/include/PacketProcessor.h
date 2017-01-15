#pragma once

#include "EventLoop.h"
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
		DISABLE_COPY(PacketProcessor);

	protected:
		PacketProcessor(PeerType_t peer);

	public:
		virtual ~PacketProcessor() = default;

		void Connect(const std::string& host, int port);
		void Accept(int port, uint16_t max_connections);

		void Tick();

		NetChannelPtr GetChannel_FrontEnd(int16_t channel_index);
		NetChannelPtr GetChannel_BackEnd() { return m_pBackEnd; }

		PeerType_t GetPeerType() const { return m_peer_type; }
	protected:
		void SendPacket2BackEnd(int dest_info, int owner_info, gpb::Message& msg);
		void SendPacket2FrontEnd(uint16_t channel_index, int dest_info, int owner_info, gpb::Message& msg);
	private:
		void SendPacketByChannel(NetChannelPtr& pChannel, int dest_info, int owner_info, gpb::Message& msg);

		MessageError_t ReadMessage(struct evbuffer* evbuf, bool bFromClient);
		void ProcessMessage(evbuffer* evbuf, NetChannelPtr& pChannel, bool bFromClient,
			std::function<void(NetChannelPtr&, PacketBase*)> fn);

		void OnMessage_FrontEnd(struct evbuffer* evbuf, NetChannelPtr& pChannel);
		void OnMessage_BackEnd(struct evbuffer* evbuf, NetChannelPtr& pChannel);

		virtual void ForwardPacketOnBackEnd(NetChannelPtr& pBackChannel, PacketBase* pkt);
		virtual void ForwardPacketOnFrontEnd(NetChannelPtr& pFrontChannel, PacketBase* pkt);

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
		PacketQueue m_queueRead;
	};
}