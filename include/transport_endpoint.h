/* TRANSPORT_ENDPOINT.H: a transport endpoint, a socket of the transport
   layer (src/transport_endpoint.cpp; src/unknown_0b49a0.cpp creates them) */

#ifndef TRANSPORT_ENDPOINT_H
#define TRANSPORT_ENDPOINT_H

#include "cseries.h"
#include "globals.h"
#include "transport_address.h"
#include <xtl.h>

struct s_transport_endpoint
{
	long socket;
	union
	{
		word flags;
		struct
		{
			word connected : 1;
			word unknown1 : 3;
			word blocking : 1;
		};
	};
	short type;
};

bool transport_endpoint_bind(s_transport_endpoint *endpoint, transport_address const *address);
bool transport_endpoint_set_option(s_transport_endpoint *endpoint, short option, long value);
void transport_endpoint_close(s_transport_endpoint *endpoint);
short transport_endpoint_read_from(s_transport_endpoint *endpoint, void *buffer, short length, transport_address *address);
short transport_endpoint_write_to(s_transport_endpoint *endpoint, void const *buffer, short length, transport_address const *address);

/* makes an endpoint's socket non-blocking; retail has it expanded in
   network_link_open_endpoint (0x92ae0) and transport_endpoint_connect
   (0xb51b0) */
__forceinline bool transport_endpoint_set_nonblocking(s_transport_endpoint *endpoint)
{
	bool result = true;
	if (g_transport_globals.initialized && g_transport_globals.started)
	{
		if (endpoint->socket == NONE)
			result = false;
		else if ((bool)(((dword)(short)endpoint->flags >> 4) & 1))
		{
			dword argument = 1;
			if (ioctlsocket(endpoint->socket, FIONBIO, &argument) == 0)
				endpoint->blocking = false;
			else
			{
				WSAGetLastError();
				result = false;
			}
		}
	}
	return result;
}

#endif
