// @flags /O2 /arch:SSE /Gr
/* TRANSPORT_ADDRESS.CPP: network addresses

The functions follow transport_address.obj in Bungie's May 2003 debug builds
(halo-symbol-atlas). Retail keeps only these three; the string, loopback,
listen and broadcast functions are gone. */

#include "cseries.h"
#include <string.h>

/* Bungie's macro, as in Halo CE's cseries.h */
#ifndef MIN
#define MIN(a,b) ((a)>(b)?(b):(a))
#endif

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

bool transport_address_valid(transport_address const *address);

/* an IPv4 address in host byte order whose last octet is zero */
// @retail 0x7aec0
bool function_07aec0(transport_address const *address, dword *ipv4_address)
{
	bool result = false;

	if (transport_address_valid(address) && address->address_length == k_ipv4_address_length)
	{
		dword value = address->ipv4_address;

		value = (((value & 0xff0000) | (value >> 16)) >> 8) | (((value << 16) | (value & 0xff00)) << 8);
		*ipv4_address = value;
		if (!(byte)value)
			result = true;
	}
	return result;
}

// @retail 0x7af40
bool transport_address_valid(transport_address const *address)
{
	bool result = false;

	if (address)
	{
		switch (address->address_length)
		{
		case NONE:
		case k_ipv4_address_length:
			result = address->ipv4_address != 0;
			break;
		case k_ipv6_address_length:
			for (long i = 0; i < 8; i++)
			{
				if (address->ipv6_address[i])
					return true;
			}
			break;
		}
	}
	return result;
}

// @retail 0x7af80
bool transport_address_equivalent(transport_address const *a, transport_address const *b, bool compare_ports)
{
	short length = MIN(a->address_length, b->address_length);

	if (a->address_length > 0 && a->address_length == b->address_length && memcmp(a, b, length) == 0 &&
		(!compare_ports || a->port == b->port))
		return true;
	return false;
}
