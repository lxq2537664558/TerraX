#include "NetChannel.h"
#include "EventLoop.h"
#include <event2/buffer.h>
#include <event2/thread.h>
#include "SocketOpt-inl.h"
#include <cassert>
#include <iostream>
using namespace TerraX;

using std::string;

NetChannel::NetChannel(EventLoop* loop, const string& host, int port)
	: m_evConn(bufferevent_socket_new(loop->eventBase(), -1, BEV_OPT_CLOSE_ON_FREE)),
	m_eState(ConnState_t::eConnecting)
{
	SetTcpNodelay(bufferevent_getfd(m_evConn));
	bufferevent_setcb(m_evConn, ReadCallback, WriteCallback, EventCallback, this);
	bufferevent_socket_connect_hostname(m_evConn, nullptr, AF_INET, host.c_str(), port);
}

NetChannel::NetChannel(struct event_base* base, int fd)
	: m_evConn(bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE))
{
	SetConnState(ConnState_t::eConnected);
	bufferevent_setcb(m_evConn, ReadCallback, WriteCallback, EventCallback, this);
	bufferevent_enable(m_evConn, EV_READ | EV_WRITE);
}

NetChannel::~NetChannel()
{
	bufferevent_free(m_evConn);
	printf("~NetChannel()\n");
}

void NetChannel::ConnectError()
{
	m_connectFailed = true;
	SetConnState(ConnState_t::eDisconnected);
	if (m_NetEventCB) {
		auto pchannel = shared_from_this();
		m_NetEventCB(std::ref(pchannel), NetEvent_t::eConnectFailed);
	}
}

void NetChannel::Connected()
{
	if (!m_connectFailed)
	{
		SetConnState(ConnState_t::eConnected);
		bufferevent_enable(m_evConn, EV_READ | EV_WRITE);

		if (m_NetEventCB) {
			auto pchannel = shared_from_this();
			m_NetEventCB(std::ref(pchannel), NetEvent_t::eConnected);
		}
	}
}

void NetChannel::Disconnected()
{
	SetConnState(ConnState_t::eDisconnected);
	if (m_NetEventCB) {
		auto pchannel = shared_from_this();
		m_NetEventCB(std::ref(pchannel), NetEvent_t::eDisconnected);
	}
}

void NetChannel::ForceClose()
{
	bufferevent_disable(m_evConn, EV_READ);
	SetConnState(ConnState_t::eDisconnecting);
}

void NetChannel::OnRead()
{
	//struct evbuffer* input = bufferevent_get_input(evConn_);

	//int readable = evbuffer_get_length(input);
	//std::unique_ptr<char> pszMsg(new char[readable + 1]);
	//evbuffer_copyout(input, pszMsg.get(), readable);
	//std::cout << __FUNCTION__ << '\t' << pszMsg.get() << std::endl;
	//m_PacketDisPatcher.DeliverPacket("Person", pszMsg.get(), readable);
	//bufferevent_write_buffer(evConn_, input);

	struct evbuffer* input = bufferevent_get_input(m_evConn);
	auto pchannel = shared_from_this();
	m_OnMessageCB(input, std::ref(pchannel));
	//ParseErrorCode errorCode = read(input, this);
	//if (errorCode != kNoError){ /*FIXME:*/ }
}

void NetChannel::OnWrite()
{
	if (m_eState == ConnState_t::eDisconnecting) {
		struct evbuffer* input = bufferevent_get_input(m_evConn);
		std::size_t readable = evbuffer_get_length(input);
		std::cout << "Writing: " << readable << std::endl;
		if (readable <= 0) {
			Disconnected();
		}
	}
}

void NetChannel::ReadCallback(struct bufferevent* bev, void* ptr)
{
	NetChannel* self = static_cast<NetChannel*>(ptr);
	assert(self->m_evConn == bev);
	self->OnRead();
}

void NetChannel::WriteCallback(struct bufferevent *bev, void *ptr)
{
	NetChannel* self = static_cast<NetChannel*>(ptr);
	assert(self->m_evConn == bev);
	self->OnWrite();
}

void NetChannel::EventCallback(struct bufferevent* bev, short events, void* ptr)
{
	NetChannel* self = static_cast<NetChannel*>(ptr);
	if (events & BEV_EVENT_CONNECTED)
	{
		printf("connected\n");
		self->Connected();
	}
	else if (events & BEV_EVENT_EOF)
	{
		printf("disconnected\n");
		self->Disconnected(); //disconnect代表是正常断开
	}
	else if (events & BEV_EVENT_ERROR)
	{
		printf("connect error\n");
		self->ConnectError(); //突然断开连接
	}
}

bool NetChannel::OnMessage(int32_t nMsgOwnerInfo, const std::string& strMsgType, const char* pBuffer, const int nBufferSize)
{
	return PacketDispatcher::GetInstance().DeliverPacket(m_peer_info, nMsgOwnerInfo, strMsgType, pBuffer, nBufferSize);
}

void NetChannel::SendMsg(const char* buf, int len)
{
	if (m_eState == ConnState_t::eConnected) {
		bufferevent_write(m_evConn, buf, len);
	}
}
