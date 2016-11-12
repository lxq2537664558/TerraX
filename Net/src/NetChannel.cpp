#include "NetChannel.h"
#include "EventLoop.h"
#include <event2/buffer.h>
#include <event2/thread.h>
#include "CodecLite-inl.h"
#include <cassert>
#include <iostream>
using namespace TerraX;

using std::string;

NetChannel::NetChannel(EventLoop* loop, const string& host, int port)
	: m_evConn(bufferevent_socket_new(loop->eventBase(), -1, BEV_OPT_CLOSE_ON_FREE))
{

	bufferevent_setcb(m_evConn, ReadCallback, nullptr, EventCallback, this);
	bufferevent_socket_connect_hostname(m_evConn, nullptr, AF_INET, host.c_str(), port);
	//setsockopt tpc_nodelay?
	//evutil_socket_t fd = bufferevent_getfd(evConn_);
	//assert(fd >= 0);
	//int flag = 1;
	//int ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&flag, sizeof(flag));
}

NetChannel::NetChannel(struct event_base* base, int fd)
	: m_evConn(bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE))
{
	bufferevent_setcb(m_evConn, ReadCallback, nullptr, EventCallback, this);
	bufferevent_enable(m_evConn, EV_READ | EV_WRITE);
}

NetChannel::~NetChannel()
{
	bufferevent_free(m_evConn);
	// printf("~NetChannel()\n");
}

void NetChannel::SetDisconnectCb(disconnect_cb cb, void* ptr)
{
	m_disconnect_cb = cb;
	m_ptr = ptr;
}

void NetChannel::ConnectFailed()
{
	m_connectFailed = true;
	Disconnected();
}

void NetChannel::Connected()
{
	if (!m_connectFailed)
	{
		bufferevent_enable(m_evConn, EV_READ | EV_WRITE);
	}
}

void NetChannel::Disconnected()
{
	if (m_disconnect_cb)
	{
		m_disconnect_cb(this, m_ptr);
	}
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
	ParseErrorCode errorCode = read(input, this);
	if (errorCode != kNoError)
	{
	//FIXME:
	}
}

void NetChannel::ReadCallback(struct bufferevent* bev, void* ptr)
{
	NetChannel* self = static_cast<NetChannel*>(ptr);
	assert(self->m_evConn == bev);
	self->OnRead();
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
		self->Disconnected();
	}
	else if (events & BEV_EVENT_ERROR)
	{
		printf("connect error\n");
		self->ConnectFailed();
	}
}

void NetChannel::SendMessage(int flag, google::protobuf::Message& msg)
{
	//bufferevent_write(evConn_, msg.c_str(), msg.size());
	send(m_evConn, flag, msg);
}

bool NetChannel::OnMessage(const std::string& strMsgType, const char* pBuffer, const int nBufferSize)
{
	return PacketDispatcher::GetInstance().DeliverPacket(*this, strMsgType, pBuffer, nBufferSize);
}