// @flags /O2 /arch:SSE /Gr
/* TRANSPORT_ENDPOINT.CPP: the socket options and the closing of a transport
   endpoint (outside lane J's region; decompiled for src/network_link.cpp,
   which calls them). The endpoint itself is created by 0xb4d50
   (src/unknown_0b49a0.cpp). */

#include "cseries.h"
#include "globals.h"
#include <xtl.h>

struct s_transport_endpoint
{
	long socket;
	word flags;
	word type;
};

// @retail 0xb4da0
long transport_endpoint_option_name(short option)
{
	switch (option)
	{
	case 0:
		return SO_REUSEADDR;
	case 1:
		return SO_LINGER;
	case 2:
		return SO_BROADCAST;
	case 3:
		return SO_SNDBUF;
	case 4:
		return SO_RCVBUF;
	case 5:
		return 0x4001;
	default:
		return NONE;
	}
}

// @retail 0xb4e70
bool transport_endpoint_set_option(s_transport_endpoint *endpoint, short option, long value)
{
	bool result = false;
	if (g_transport_globals.initialized && g_transport_globals.started && endpoint->socket != NONE)
	{
		short name = (short)transport_endpoint_option_name(option);
		if (name != -1)
		{
			char const *option_value = option == 5 ? (char const *)value : (char const *)&value;
			if (setsockopt(endpoint->socket, SOL_SOCKET, name, option_value, sizeof(value)))
				WSAGetLastError();
			else
				result = true;
		}
	}
	return result;
}

// @retail 0xb4f50
void transport_endpoint_close(s_transport_endpoint *endpoint)
{
	if (endpoint->socket != NONE && g_transport_globals.initialized && g_transport_globals.started)
	{
		if (endpoint->flags & 1)
		{
			if (shutdown(endpoint->socket, 2))
				WSAGetLastError();
		}
		if (closesocket(endpoint->socket))
			WSAGetLastError();
	}
	endpoint->socket = NONE;
	endpoint->flags = 0;
}
