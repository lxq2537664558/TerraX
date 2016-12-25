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

NetChannelPtr NetServer::GetChannel(uint16_t nChannelIndex)
{
	auto it = m_mapChannels.find(nChannelIndex);
	if (it != m_mapChannels.end()) {
		return it->second;
	}
	return nullptr;
}

NetChannelPtr NetServer::GetChannel(PeerType_t peer_type, uint8_t peer_index)
{
	for (auto& var : m_mapChannels) {
		auto& channel = var.second;
		if (channel->GetPeerType() == peer_type && channel->GetPeerIndex() == peer_index) {
			return channel;
		}
	}
	return nullptr;
}

void NetServer::ForceClose(NetChannelPtr& channel)
{
	channel->ForceClose();
}

void NetServer::ForceCloseAll()
{
	for (auto& var : m_mapChannels) {
		(var.second)->ForceClose();
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
	struct event* ev = event_new(base, pipefd[0], EV_READ, cb_func, NULL);
	event_add(ev, NULL);

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

	NetChannelPtr pChannel = std::make_shared<NetChannel>(base, static_cast<int>(fd));
	pChannel->SetDisconnectCb(std::bind(&NetServer::DisconnectCallback, this, std::placeholders::_1));
	pChannel->RegOnMessage_Callback(m_OnMessageCB);
	
	if (m_freeindexes.empty())
	{
		printf("No Enough Connection Index! \n");
		pChannel.reset();
	}
	else
	{
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			auto nIndex = m_freeindexes.front();
			m_freeindexes.pop();
			m_mapChannels[nIndex] = pChannel;
			pChannel->SetChannelIndex(nIndex);
		}
		if (m_NetEventCB) {
			m_NetEventCB(pChannel, NetEvent_t::eConnected);
		}
	}
}

void NetServer::OnDisconnect(NetChannelPtr& pChannel)
{
	assert(pChannel);
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_freeindexes.push(pChannel->GetChannelIndex());
		m_mapChannels.erase(pChannel->GetChannelIndex());
	}
	if (m_NetEventCB) {
		m_NetEventCB(pChannel, NetEvent_t::eDisconnected);
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

void NetServer::DisconnectCallback(NetChannelPtr& channel)
{
	printf("disconnectCallback\n");
	this->OnDisconnect(channel);
}