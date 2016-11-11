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
	: evConn_(bufferevent_socket_new(loop->eventBase(), -1, BEV_OPT_CLOSE_ON_FREE)),
	connectFailed_(false),
	disconnect_cb_(nullptr),
	ptr_(nullptr)
{

	bufferevent_setcb(evConn_, readCallback, nullptr, eventCallback, this);
	bufferevent_socket_connect_hostname(evConn_, nullptr, AF_INET, host.c_str(), port);
	//setsockopt tpc_nodelay?
	//evutil_socket_t fd = bufferevent_getfd(evConn_);
	//assert(fd >= 0);
	//int flag = 1;
	//int ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&flag, sizeof(flag));
}

NetChannel::NetChannel(struct event_base* base, int fd)
	: evConn_(bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE)),
	connectFailed_(false),
	disconnect_cb_(nullptr),
	ptr_(nullptr)
{
	bufferevent_setcb(evConn_, readCallback, nullptr, eventCallback, this);
	bufferevent_enable(evConn_, EV_READ | EV_WRITE);
}

NetChannel::~NetChannel()
{
	bufferevent_free(evConn_);
	// printf("~NetChannel()\n");
}

void NetChannel::setDisconnectCb(disconnect_cb cb, void* ptr)
{
	disconnect_cb_ = cb;
	ptr_ = ptr;
}

void NetChannel::connectFailed()
{
	connectFailed_ = true;
	disconnected();
}

void NetChannel::connected()
{
	if (!connectFailed_)
	{
		bufferevent_enable(evConn_, EV_READ | EV_WRITE);
	}
}

void NetChannel::disconnected()
{
	if (disconnect_cb_)
	{
		disconnect_cb_(this, ptr_);
	}
}

void NetChannel::onRead()
{
	//struct evbuffer* input = bufferevent_get_input(evConn_);

	//int readable = evbuffer_get_length(input);
	//std::unique_ptr<char> pszMsg(new char[readable + 1]);
	//evbuffer_copyout(input, pszMsg.get(), readable);
	//std::cout << __FUNCTION__ << '\t' << pszMsg.get() << std::endl;
	//m_PacketDisPatcher.DeliverPacket("Person", pszMsg.get(), readable);
	//bufferevent_write_buffer(evConn_, input);

	struct evbuffer* input = bufferevent_get_input(evConn_);
	ParseErrorCode errorCode = read(input, this);
	if (errorCode != kNoError)
	{
	//FIXME:
	}
}

void NetChannel::readCallback(struct bufferevent* bev, void* ptr)
{
	NetChannel* self = static_cast<NetChannel*>(ptr);
	assert(self->evConn_ == bev);
	self->onRead();
}

void NetChannel::eventCallback(struct bufferevent* bev, short events, void* ptr)
{
	NetChannel* self = static_cast<NetChannel*>(ptr);
	if (events & BEV_EVENT_CONNECTED)
	{
		printf("connected\n");
		self->connected();
	}
	else if (events & BEV_EVENT_EOF)
	{
		printf("disconnected\n");
		self->disconnected();
	}
	else if (events & BEV_EVENT_ERROR)
	{
		printf("connect error\n");
		self->connectFailed();
	}
}

void NetChannel::sendMessage(int flag, google::protobuf::Message& msg)
{
	//bufferevent_write(evConn_, msg.c_str(), msg.size());
	send(evConn_, flag, msg);
}

bool NetChannel::onMessage(const std::string& strMsgType, const char* pBuffer, const int nBufferSize)
{
	return PacketDispatcher::GetInstance().DeliverPacket(*this, strMsgType, pBuffer, nBufferSize);
}