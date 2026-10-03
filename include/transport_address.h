/* TRANSPORT_ADDRESS.H: the network address (src/transport_address.cpp holds
   0x7aec0, 0x7af40 and 0x7af80; src/transport_address.cpp keeps its own copy of
   the structure, which it predates). An IPv4 address is a dword, an IPv6 one
   eight words; the length (4 or 16) sits at +0x12. */

#ifndef TRANSPORT_ADDRESS_H
#define TRANSPORT_ADDRESS_H

#include "cseries.h"

enum
{
	k_ipv4_address_length = 4,
	k_ipv6_address_length = 16
};

struct transport_address
{
	union
	{
		dword ipv4_address;
		word ipv6_address[8];
	};
	word port;
	short address_length;
};

bool function_07aec0(transport_address const *address, dword *ipv4_address);
bool transport_address_valid(transport_address const *address);
bool transport_address_equivalent(transport_address const *a, transport_address const *b, bool compare_ports);

#endif
