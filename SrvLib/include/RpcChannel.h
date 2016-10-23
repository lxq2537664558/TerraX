#pragma once
#include <functional>
#include <map>
#include <atomic>
#include <cstdint>
#include <mutex>
#include <event2/bufferevent.h>

namespace TerraX
{
	class EventLoop;
	using PacketFnCB = std::function<void(class packet*)>;
	class RpcChannel
	{
	public:
		using disconnect_cb = std::function<void(RpcChannel*, void* ptr)> ;

		explicit RpcChannel(struct event_base *base, int fd, const std::map<std::string, PacketFnCB>&);
		explicit RpcChannel(EventLoop* loop, const std::string& host, int port);
		~RpcChannel();
		void setDisconnectCb(disconnect_cb cb, void* ptr);

	private:
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

		std::map<std::string, PacketFnCB> services_;
	};


}