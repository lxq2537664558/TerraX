#include "RpcChannel.h"
#include "EventLoop.h"
#include <event2/buffer.h>
#include <event2/thread.h>
#include <cassert>
using namespace TerraX;

using std::string;

RpcChannel::RpcChannel(EventLoop* loop, const string& host, int port)
	: evConn_(bufferevent_socket_new(loop->eventBase(), -1, BEV_OPT_CLOSE_ON_FREE)),
	connectFailed_(false),
	disconnect_cb_(NULL),
	ptr_(NULL)
{
	bufferevent_setcb(evConn_, readCallback, NULL, eventCallback, this);
	bufferevent_socket_connect_hostname(evConn_, NULL, AF_INET, host.c_str(), port);
}

RpcChannel::RpcChannel(struct event_base* base, int fd, const std::map<std::string, PacketFnCB>& services)
	: evConn_(bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE)),
	connectFailed_(false),
	disconnect_cb_(NULL),
	ptr_(NULL),
	services_(services)
{
	bufferevent_setcb(evConn_, readCallback, NULL, eventCallback, this);
	bufferevent_enable(evConn_, EV_READ | EV_WRITE);
}

RpcChannel::~RpcChannel()
{
	bufferevent_free(evConn_);
	// printf("~RpcChannel()\n");
}

void RpcChannel::setDisconnectCb(disconnect_cb cb, void* ptr)
{
	disconnect_cb_ = cb;
	ptr_ = ptr;
}

void RpcChannel::connectFailed()
{
	connectFailed_ = true;
	disconnected();
}

void RpcChannel::connected()
{
	if (!connectFailed_)
	{
		bufferevent_enable(evConn_, EV_READ | EV_WRITE);
	}
}

void RpcChannel::disconnected()
{
	if (disconnect_cb_)
	{
		disconnect_cb_(this, ptr_);
	}
}

void RpcChannel::readCallback(struct bufferevent* bev, void* ptr)
{
	RpcChannel* self = static_cast<RpcChannel*>(ptr);
	assert(self->evConn_ == bev);
	//self->onRead();
}

void RpcChannel::eventCallback(struct bufferevent* bev, short events, void* ptr)
{
	RpcChannel* self = static_cast<RpcChannel*>(ptr);
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
