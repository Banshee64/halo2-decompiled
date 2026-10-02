// @flags /O2 /Gr
/* UNKNOWN_07AD80.CPP: network random bytes and address helpers */

#include "cseries.h"
#include <xtl.h>
#include <stdlib.h>
#include <time.h>

byte g_4cf790;

// @retail 0x7ad80
void function_07ad80(long count, byte *buffer)
{
	if (g_4cf790)
	{
		XNetRandom(buffer, count);
		return;
	}

	time_t now = time(0);
	long r = rand();
	dword ticks = GetTickCount();
	dword seed = r ^ ticks ^ (dword)now;
	for (long i = 0; i < count; i++)
	{
		seed = seed * 0x19660d + 0x3c6ef35f;
		word r = (word)(seed >> 16);
		dword scaled = r * 256u;
		buffer[i] = (byte)(scaled >> 16);
	}
}

struct s_network_address
{
	word words[8];
	byte unknown10[2];
	short type;
};

#define ADDRESS_TYPE_IPV4 4
#define ADDRESS_TYPE_IPV6 0x10

// @retail 0x7aec0
bool function_07aec0(const s_network_address *address, dword *ip_out)
{
	bool result = false;
	if (address)
	{
		bool valid = false;
		switch (address->type)
		{
		case NONE:
		case ADDRESS_TYPE_IPV4:
			valid = *(const dword *)address != 0;
			break;
		case ADDRESS_TYPE_IPV6:
			for (long i = 0; i < 8; i++)
			{
				if (address->words[i])
				{
					valid = true;
					break;
				}
			}
			break;
		}

		if (valid && address->type == ADDRESS_TYPE_IPV4)
		{
			dword v = *(const dword *)address;
			dword ip = ((((v & 0xff0000) | (v >> 16)) >> 8) | (((v << 16) | (v & 0xff00)) << 8));
			*ip_out = ip;
			if ((byte)ip == 0)
			{
				result = true;
			}
		}
	}
	return result;
}
