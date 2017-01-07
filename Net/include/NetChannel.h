#pragma once
#include <event2/bufferevent.h>
#include <atomic>
#include <cstdint>
#include <iostream>
#include <map>
#include <mutex>
#include "ComDef.h"
#include "NetDefine.h"
#include "PacketDispatcher.h"

namespace TerraX
{
    class EventLoop;
    using OnMessage_CB = std::function<void(evbuffer*, NetChannelPtr&)>;
    // typedef std::shared_ptr<google::protobuf::Message> MessagePtr;
    class NetChannel final : public std::enable_shared_from_this<NetChannel>
    {
        DISABLE_COPY(NetChannel);

    public:
        explicit NetChannel(struct event_base* base, int fd);
        explicit NetChannel(EventLoop* loop, const std::string& host, int port);
        ~NetChannel();

        void SendMsg(const char* buf, int len);
        bool OnMessage(int32_t nMsgOwnerInfo, const std::string& strMsgType, const char* pBuffer,
                       const int nBufferSize);

        void SetPeerType(PeerType_t peer_type)
        {
            m_peer_info = (uint8_t(peer_type) << 24) + (m_peer_info & 0x00FFFFFF);
        }
        PeerType_t GetPeerType() const { return PeerType_t((m_peer_info & 0xFF000000) >> 24); }
        void SetPeerIndex(uint8_t peer_index)
        {
            m_peer_info = (peer_index << 16) + (m_peer_info & 0xFF00FFFF);
        }
        uint8_t GetPeerIndex() const { return (m_peer_info & 0x00FF0000) >> 16; }
        void SetChannelIndex(uint16_t idx) { m_peer_info = (m_peer_info & 0xFFFF0000) + idx; }
        uint16_t GetChannelIndex() const { return m_peer_info & 0x0000FFFF; }
        int32_t GetPeerInfo() const { return m_peer_info; }
        void SetPeerInfo(int32_t peer_info) { m_peer_info = peer_info; }

        ConnState_t GetConnState() const { return m_eState; }
        void ForceClose();

        void RegNetEvent_Callback(NetEvent_CB cb) { m_NetEventCB = cb; }
        void RegOnMessage_Callback(OnMessage_CB cb) { m_OnMessageCB = cb; }

    private:
        void OnRead();
        void OnWrite();

        void ConnectError();
        void Connected();
        void Disconnected();

        void SetConnState(ConnState_t eState) { m_eState = eState; }

        static void ReadCallback(struct bufferevent* bev, void* ptr);
        static void EventCallback(struct bufferevent* bev, short events, void* ptr);
        static void WriteCallback(struct bufferevent* bev, void* ptr);

    private:
        struct bufferevent* m_evConn{nullptr};
        bool m_connectFailed{false};

    private:
        NetEvent_CB m_NetEventCB;
        ConnState_t m_eState{ConnState_t::eDisconnected};
        int32_t m_peer_info{0};
        OnMessage_CB m_OnMessageCB;
    };
}