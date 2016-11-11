#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <atomic>
#include <cstdint>
#include <mutex>
#include <event2/bufferevent.h>
#include "PacketDispatcher.h"
#include "ComDef.h"

namespace TerraX
{
	class EventLoop;
	using PacketFnCB = std::function<void(class packet*)>;
	class NetChannel
	{
		NOCOPY(NetChannel);
	public:
		using disconnect_cb = std::function<void(NetChannel*, void* ptr)> ;

		explicit NetChannel(struct event_base *base, int fd);
		explicit NetChannel(EventLoop* loop, const std::string& host, int port);
		~NetChannel();
		void setDisconnectCb(disconnect_cb cb, void* ptr);
		
		void sendMessage(int flag, google::protobuf::Message& msg);
		bool onMessage(const std::string& strMsgType, const char* pBuffer, const int nBufferSize);
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
	};


}