#pragma once

#include <event2/util.h>
#ifdef __GNUC__
#include <netinet/tcp.h>
#endif
namespace terra
{
	inline void SetTcpNodelay(evutil_socket_t fd)
	{
		int opt_val = 1;
#ifdef _WIN32
		::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
			(const char *)&opt_val, sizeof(opt_val));
#else
		::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
			(void *)&opt_val, sizeof(opt_val));
#endif
	}
}