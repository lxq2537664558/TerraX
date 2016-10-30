#include "RpcServer.h"
#include "EventLoop.h"
#include "RpcChannel.h"
#include <thread>
#include <cassert>
#include <iostream>

using namespace TerraX;

struct sockaddr* getListenSock(int port)
{
	static struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	return (struct sockaddr*)&sin;
}

RpcServer::RpcServer(EventLoop* loop, int port)
	: evListener_(evconnlistener_new_bind(loop->eventBase(),
		newConnectionCallback, this, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1,
		getListenSock(port), sizeof(struct sockaddr_in))),
	currLoop_(0)
{
	loops_.push_back(loop->eventBase());
}

RpcServer::~RpcServer()
{
	// struct event_base* base = evconnlistener_get_base(evListener_);
	evconnlistener_free(evListener_);
	if (loops_.size() > 1)
	{
		for (size_t i = 0; i < loops_.size(); ++i)
		{
			event_base_free(loops_[i]);
		}
	}
}

void RpcServer::setThreadNum(int numThreads)
{
#ifndef _WIN32
	if (numThreads > 1)
	{
		loops_.clear();
		for (int i = 0; i < numThreads; ++i)
		{
			struct event_base* base = event_base_new();
			std::thread t(runLoop, base);
			t.detach();
			loops_.push_back(base);
		}
	}
#endif
}

static void cb_func(evutil_socket_t fd, short what, void *arg)
{
	std::cout << __FUNCTION__ << __LINE__ << std::endl;
}

void* RpcServer::runLoop(void* ptr)
{
#ifdef _WIN32

#else
	struct event_base* base = static_cast<struct event_base*>(ptr);
	int pipefd[2];
	int ret = pipe(pipefd);
	assert(ret == 0);
	struct event* ev = event_new(base, pipefd[0], EV_READ, cb_func, NULL);
	event_add(ev, NULL);

	printf("runLoop\n");
	event_base_loop(base, 0);
	printf("runLoop done\n");

	event_free(ev);
	close(pipefd[0]);
	close(pipefd[1]);
#endif
	return NULL;
}

void RpcServer::onConnect(evutil_socket_t fd)
{
	struct event_base* base = loops_[currLoop_];
	++currLoop_;
	if (static_cast<size_t>(currLoop_) >= loops_.size())
	{
		currLoop_ = 0;
	}

	RpcChannel* channel = new RpcChannel(base, static_cast<int>(fd));
	channel->setDisconnectCb(&RpcServer::disconnectCallback, this);

	std::lock_guard<std::mutex> lock(mutex_);
	channels_.insert(channel);
}

void RpcServer::onDisconnect(RpcChannel* channel)
{
	{
		std::lock_guard<std::mutex> lock(mutex_);
		int n = channels_.erase(channel);
		assert(n == 1);
	}
	delete channel;
}

void RpcServer::newConnectionCallback(struct evconnlistener* listener,
	evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx)
{
	//int flag = 1;
	//int ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&flag, sizeof(flag));
	//应该在此处设置tcp_nodelay， so_linger?
	printf("newConnectionCallback\n");
	RpcServer* self = static_cast<RpcServer*>(ctx);
	assert(self->evListener_ == listener);
	self->onConnect(fd);
}

void RpcServer::disconnectCallback(RpcChannel* channel, void* ctx)
{
	printf("disconnectCallback\n");
	RpcServer* self = static_cast<RpcServer*>(ctx);
	self->onDisconnect(channel);
}