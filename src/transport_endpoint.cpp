// @flags /O2 /arch:SSE /Gr
/* TRANSPORT_ENDPOINT.CPP: the socket options and the closing of a transport
   endpoint (outside lane J's region; decompiled for src/network_link.cpp,
   which calls them). The endpoint itself is created by 0xb4d50
   (src/unknown_0b49a0.cpp). */

#include "cseries.h"
#include "globals.h"
#include "transport_address.h"
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

/* a socket address: sockaddr_in for IPv4 (0x10 bytes), sockaddr_in6 for
   IPv6 (0x1c bytes) */
struct s_socket_address
{
	short family;
	word port;
	dword ipv4_address;
	word ipv6_address[8];
	dword scope;
};

static inline word byte_swap_word(word value)
{
	return (word)((value >> 8) | (value << 8));
}

static inline dword byte_swap_long(dword value)
{
	return (((value & 0xff0000) | (value >> 16)) >> 8) | (((value & 0xff00) | (value << 16)) << 8);
}

// @retail 0xb5470
bool transport_address_to_socket_address(transport_address const *address, long *socket_address_length, s_socket_address *socket_address)
{
	*socket_address_length = 0;
	bool result = false;
	switch (address->address_length)
	{
	case k_ipv4_address_length:
		socket_address->family = AF_INET;
		socket_address->ipv4_address = byte_swap_long(address->ipv4_address);
		socket_address->port = byte_swap_word(address->port);
		*socket_address_length = 0x10;
		result = true;
		break;
	case k_ipv6_address_length:
		socket_address->family = 0x17;
		for (long i = 0; i < 8; i++)
			socket_address->ipv6_address[i] = byte_swap_word(address->ipv6_address[i]);
		socket_address->port = byte_swap_word(address->port);
		*socket_address_length = 0x1c;
		result = true;
		break;
	}
	return result;
}

// @retail 0xb5110
short transport_endpoint_write_to(s_transport_endpoint *endpoint, void const *buffer, short length, transport_address const *address)
{
	short result = -3;
	if (g_transport_globals.initialized && g_transport_globals.started)
	{
		s_socket_address socket_address;
		long socket_address_length;
		if (transport_address_to_socket_address(address, &socket_address_length, &socket_address))
		{
			result = (short)sendto(endpoint->socket, (char const *)buffer, length, 0, (sockaddr const *)&socket_address, socket_address_length);
			if (result == -1)
			{
				long error = WSAGetLastError();
				if (error == WSAEWOULDBLOCK)
					result = -2;
				else if (error == WSAEHOSTUNREACH)
					result = -1;
				else
					result = -3;
			}
		}
	}
	return result;
}
