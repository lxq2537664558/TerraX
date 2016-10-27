#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <atomic>
#include <cstdint>
#include <mutex>
#include <event2/bufferevent.h>
#include "PacketDispatcher.h"

#include "addressbook.pb.h"

namespace TerraX
{
	class EventLoop;
	using PacketFnCB = std::function<void(class packet*)>;
	class RpcChannel
	{
	public:
		using disconnect_cb = std::function<void(RpcChannel*, void* ptr)> ;

		explicit RpcChannel(struct event_base *base, int fd, PacketDispatcher& pd);
		explicit RpcChannel(EventLoop* loop, const std::string& host, int port);
		~RpcChannel();
		void setDisconnectCb(disconnect_cb cb, void* ptr);


		// test
		void SendMsg(std::string& msg);
		template<class T> void SendPacket(T& packet) {
			std::string msg;
			packet.SerializeToString(&msg);
			SendMsg(msg);
		}
		void OnMessage(tutorial::Person& p) {
			std::cout << p.name().c_str() << std::endl;
		}
	private:
		void onRead();

		void connectFailed();
		void connected();
		void disconnected();

		static void readCallback(struct bufferevent *bev, void *ptr);
		static void eventCallback(struct bufferevent *bev, short events, void *ptr);
	private:
		struct bufferevent* evConn_;
		bool connectFailed_;
		disconnect_cb disconnect_cb_;
		void* ptr_;

		std::atomic<int64_t> id_;
		std::mutex mutex_;
	public:
		PacketDispatcher& m_PacketDisPatcher;
	};


}