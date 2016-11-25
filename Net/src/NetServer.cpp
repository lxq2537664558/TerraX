#include "NetServer.h"
#include "EventLoop.h"
#include "NetChannel.h"
#include <thread>
#include <cassert>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#endif
#include "SocketOpt-inl.h"
using namespace TerraX;

struct sockaddr* getListenSock(int port)
{
	static struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(static_cast<uint16_t>(port));
	return (struct sockaddr*)&sin;
}

NetServer::NetServer(EventLoop* loop, int port, uint16_t max_conns)
	: m_maxconnections(max_conns), 
	m_evListener(evconnlistener_new_bind(loop->eventBase(),
		NewConnectionCallback, this, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1,
		getListenSock(port), sizeof(struct sockaddr_in)))
{
	m_loops.push_back(loop->eventBase());

	for (int i = 1; i < max_conns; ++i) {
		m_freeindexes.push(i);
	}
	m_vecChannels.resize(max_conns);

}

NetServer::~NetServer()
{
	// struct event_base* base = evconnlistener_get_base(evListener_);
	evconnlistener_free(m_evListener);
	if (m_loops.size() > 1)
	{
		for (size_t i = 0; i < m_loops.size(); ++i)
		{
			event_base_free(m_loops[i]);
		}
	}
}

void NetServer::SetThreadNum(int numThreads)
{
#ifdef __GNUC__
	if (numThreads > 1)
	{
		m_loops.clear();
		for (int i = 0; i < numThreads; ++i)
		{
			struct event_base* base = event_base_new();
			std::thread t(RunLoop, base);
			t.detach();
			m_loops.push_back(base);
		}
	}
#endif
}

void NetServer::ForceClose(NetChannelPtr& channel)
{
	channel->ForceClose();
}

void NetServer::ForceCloseAll()
{
	for (auto channel : m_vecChannels) {
		channel->ForceClose();
	}
}

static void cb_func(evutil_socket_t fd, short what, void *arg)
{
	std::cout << __FUNCTION__ << __LINE__ << std::endl;
}

void* NetServer::RunLoop(void* ptr)
{
#ifdef __GNUC__
	struct event_base* base = static_cast<struct event_base*>(ptr);
	int pipefd[2];
	int ret = pipe(pipefd);
	assert(ret == 0);
	struct event* ev = event_new(base, pipefd[0], EV_READ, cb_func, nullptr);
	event_add(ev, nullptr);

	std::cout << "Run Thread Loop: " << std::this_thread::get_id() << std::endl;
	event_base_loop(base, 0);
	printf("runLoop done\n");

	event_free(ev);
	close(pipefd[0]);
	close(pipefd[1]);
#endif
	return nullptr;
}

void NetServer::OnConnect(evutil_socket_t fd)
{
	struct event_base* base = m_loops[m_currLoop];
	++m_currLoop;
	if (static_cast<size_t>(m_currLoop) >= m_loops.size())
	{
		m_currLoop = 0;
	}

	NetChannelPtr pChannel(new NetChannel(base, static_cast<int>(fd)));
	pChannel->SetDisconnectCb(&NetServer::DisconnectCallback, this);
	
	if (m_freeindexes.empty())
	{
		printf("No Enough Connection Index! \n");
		pChannel.reset();
	}
	else
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		int nIndex = m_freeindexes.front();
		m_freeindexes.pop();
		m_vecChannels[nIndex] = pChannel;
		pChannel->SetChannelIndex(nIndex);
	}
}

void NetServer::OnDisconnect(NetChannelPtr& pChannel)
{
	assert(pChannel);
	if (m_DisconnectedCB) {
		m_DisconnectedCB(pChannel);
	}
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_freeindexes.push(pChannel->GetChannelIndex());
		m_vecChannels[pChannel->GetChannelIndex()] = nullptr;
	}
	pChannel.reset();
}

void NetServer::NewConnectionCallback(struct evconnlistener* listener,
	evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx)
{
	SetTcpNodelay(fd);
	printf("newConnectionCallback\n");
	NetServer* self = static_cast<NetServer*>(ctx);
	assert(self->m_evListener == listener);
	self->OnConnect(fd);
}

void NetServer::DisconnectCallback(NetChannelPtr& channel, void* ctx)
{
	printf("disconnectCallback\n");
	NetServer* self = static_cast<NetServer*>(ctx);
	self->OnDisconnect(channel);
}