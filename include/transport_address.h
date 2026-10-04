/* TRANSPORT_ADDRESS.H: the network address (src/s_type_99af70.cpp holds
   0x7aec0, 0x7af40 and 0x7af80; src/s_type_99af70.cpp keeps its own copy of
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

struct s_type_99af70
{
	union
	{
		dword ipv4_address;
		word ipv6_address[8];
	};
	word port;
	short address_length;
};

bool function_07aec0(s_type_99af70 const *address, dword *ipv4_address);
bool function_7af40(s_type_99af70 const *address);
bool function_7af80(s_type_99af70 const *a, s_type_99af70 const *b, bool compare_ports);

#endif
