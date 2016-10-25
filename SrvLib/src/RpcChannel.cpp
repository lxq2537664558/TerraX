#include "RpcChannel.h"
#include "EventLoop.h"
#include <event2/buffer.h>
#include <event2/thread.h>
#include <cassert>
#include <iostream>
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

void RpcChannel::onRead()
{
	struct evbuffer* input = bufferevent_get_input(evConn_);
	//int readable = evbuffer_get_length(input);
	//std::unique_ptr<char> pszMsg(new char[readable]);
	//evbuffer_remove(input, pszMsg.get(), readable);
	//std::cout << pszMsg.get() << std::endl;
	//echo
	//struct evbuffer* buf = evbuffer_new();
	//evbuffer_expand(buf, readable);
	//struct evbuffer_iovec vec;
	//evbuffer_reserve_space(buf, readable, &vec, 1);
	//uint8_t* start = static_cast<uint8_t*>(vec.iov_base);

	//int len_be = htonl(readable);
	//memcpy(start, &len_be, sizeof(len_be));
	//start += 4;

	//memcpy(start, pszMsg.get(), sizeof(char) * readable);
	//vec.iov_len = readable;
	//evbuffer_commit_space(buf, &vec, 1);
	bufferevent_write_buffer(evConn_, input);
	//evbuffer_free(input);

	//ParseErrorCode errorCode = read(input, this);
	//if (errorCode != kNoError)
	//{
		// FIXME:
	//}
}

void RpcChannel::readCallback(struct bufferevent* bev, void* ptr)
{
	RpcChannel* self = static_cast<RpcChannel*>(ptr);
	assert(self->evConn_ == bev);
	self->onRead();
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
