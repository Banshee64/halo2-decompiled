// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_07A9A0.CPP: the transport layer's local address cache, address
   resolution against the XNet key registry, and the QoS handle pool */

#include "cseries.h"
#include "data_array.h"
#include "globals.h"
#include "bitstream.h"
#include "unknown_1946f0.h"
#include <xtl.h>
#include <string.h>

/* the transport address, as 0x7ad80's file also sees it: an IPv4 address is a
   dword, an IPv6 one eight words; the type sits at +0x12 */
struct transport_address
{
	union
	{
		dword ip;
		word words[8];
	};
	word port;
	short type;
};

struct s_xnet_registry_entry
{
	bool valid;
	byte unknown1[7];
	XNKID kid;
	XNKEY key;
};

/* 0x4cf791: the cached title address and the flag for its state */
bool g_4cf791;
bool g_4cf792;
XNADDR g_4cf793;
transport_address g_4cf7b8;
byte g_4cf7cc[6];
s_xnet_registry_entry g_4cf7d4[8];

/* 0x440070: eight zero bytes */
const byte g_440070[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
const byte *g_46725c = g_440070;

/* src/unknown_07ad80.cpp names the same structure s_network_address, and its
   function_07aec0 is mangled with that name */
struct s_network_address
{
	word words[8];
	byte unknown10[2];
	short type;
};

void qos_release(long handle);
bool function_07aec0(const s_network_address *address, dword *ip_out);
void function_07ad80(long count, byte *buffer);

/* 0x468758: the allocator the QoS pool is built through */
c_data_allocator *g_468758;

static __inline dword byte_swap_long(dword v)
{
	return (((v & 0xff0000) | (v >> 16)) >> 8) | (((v << 16) | (v & 0xff00)) << 8);
}

// @retail 0x7a9a0
void __stdcall function_07a9a0(long unused)
{
	g_4cf791 = false;
	g_4cf792 = false;
}

// @retail 0x7a9b0
bool function_07a9b0(void)
{
	if (!g_4cf791)
	{
		XNADDR xna;
		dword flags;

		memset(&g_4cf793, 0, sizeof(g_4cf793));
		memset(&g_4cf7b8, 0, sizeof(g_4cf7b8));

		flags = XNetGetTitleXnAddr(&xna);
		if (flags != XNET_GET_XNADDR_PENDING)
		{
			if (flags == XNET_GET_XNADDR_NONE)
			{
				g_4cf792 = false;
			}
			else
			{
				g_4cf792 = true;
				g_4cf793 = xna;
				if (flags & (XNET_GET_XNADDR_STATIC | XNET_GET_XNADDR_DHCP | XNET_GET_XNADDR_PPPOE))
				{
					g_4cf7b8.type = 4;
					g_4cf7b8.ip = byte_swap_long(xna.ina.s_addr);
					g_4cf7b8.port = 0;
				}
				return g_4cf791 = true;
			}
		}
	}
	return g_4cf791;
}

// @retail 0x7ab60
bool function_07ab60(const transport_address *address, bool local, long *index_out, XNKID *kid_out, XNKEY *key_out, XNADDR *xnaddr_out)
{
	bool result = false;
	long index = NONE;
	dword ina = 0;
	XNKID kid;
	XNKEY key;

	memset(&kid, 0, sizeof(kid));
	memset(&key, 0, sizeof(key));

	if (address->type == 4 && address->ip)
	{
		if (local)
		{
			memset(xnaddr_out, 0, sizeof(*xnaddr_out));
			xnaddr_out->ina.s_addr = byte_swap_long(address->ip);
			result = true;
		}
		else if (function_07aec0((const s_network_address *)address, &ina))
		{
			IN_ADDR in_addr;

			in_addr.s_addr = ina;
			if (XNetInAddrToXnAddr(in_addr, xnaddr_out, &kid) == 0)
			{
				long i;
				s_xnet_registry_entry *entry;

				result = true;
				for (i = 0; i < 8; i++)
				{
					entry = &g_4cf7d4[i];
					if (entry->valid && memcmp(&entry->kid, &kid, sizeof(kid)) == 0)
					{
						index = i;
						key = entry->key;
						break;
					}
				}
			}
		}
	}
	if (index_out)
	{
		*index_out = index;
	}
	if (kid_out)
	{
		*kid_out = kid;
	}
	if (key_out)
	{
		*key_out = key;
	}

	return result;
}

// @retail 0x7acc0
bool function_07acc0(const transport_address *address)
{
	bool result = false;
	dword ina;

	if (function_07aec0((const s_network_address *)address, &ina))
	{
		IN_ADDR in_addr;

		in_addr.s_addr = ina;
		result = XNetConnect(in_addr) == 0;
	}

	return result;
}

// @retail 0x7acf0
long function_07acf0(const transport_address *address)
{
	dword ina;

	if (function_07aec0((const s_network_address *)address, &ina))
	{
		IN_ADDR in_addr;

		in_addr.s_addr = ina;
		switch (XNetGetConnectStatus(in_addr))
		{
		case 0:
			return 0;
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 3;
		}
	}

	return 4;
}

// @retail 0x7ad50
void function_07ad50(byte *buffer)
{
	do
	{
		function_07ad80(8, buffer);
	}
	while (memcmp(g_46725c, buffer, 8) == 0);
}

// @retail 0x7ae70
void function_07ae70(void)
{
	XNADDR xna;

	memset(&xna, 0, sizeof(xna));
	XNetGetTitleXnAddr(&xna);
	memcpy(g_4cf7cc, xna.abEnet, 6);
}

// @retail 0x7af40
bool transport_address_valid(const transport_address *address)
{
	bool result = false;

	if (address)
	{
		switch (address->type)
		{
		case NONE:
		case 4:
			result = address->ip != 0;
			break;
		case 0x10:
			for (long i = 0; i < 8; i++)
			{
				if (address->words[i])
				{
					result = true;
					break;
				}
			}
			break;
		}
	}

	return result;
}

// @retail 0x7af80
bool function_07af80(const transport_address *a, const transport_address *b, bool compare_port)
{
	short size = b->type <= a->type ? b->type : a->type;

	if (b->type > 0 && b->type == a->type)
	{
		if (memcmp(a, b, size) == 0 && (!compare_port || a->port == b->port))
		{
			return true;
		}
	}

	return false;
}

// @retail 0x7b3e0
void function_07b3e0(void)
{
	memset(&g_4cf8d4, 0, 8);
	g_4cf8d8 = data_new_inlined("transport qos attempts", 0x20, 8, 0, g_468758);
}

/* retail inlines the absolute-index walk of data_next_absolute_index here */
static __inline long next_absolute_index(s_data_array *data, long index)
{
	long result = NONE;

	if (index >= 0)
	{
		for (; index < data->high_water_index; index++)
		{
			if (data->bitmap[index >> 5] & (1 << (index & 0x1f)))
			{
				result = index;
				break;
			}
		}
	}

	return result;
}

static __inline byte *iterator_next(s_data_iterator *iterator)
{
	s_data_array *data = iterator->data;
	long index = next_absolute_index(data, iterator->index + 1);
	byte *result;

	if (index != NONE)
	{
		result = data->data + data->size * index;
		iterator->index = index;
		iterator->datum_index = (*(short *)result << 16) | index;
	}
	else
	{
		iterator->index = data->maximum_count;
		iterator->datum_index = NONE;
		result = 0;
	}

	return result;
}

// @retail 0x7b430
void function_07b430(void)
{
	if (g_4cf8d4)
	{
		s_data_iterator iterator;

		iterator.data = g_4cf8d8;
		iterator.datum_index = NONE;
		iterator.index = NONE;
		while (iterator_next(&iterator))
		{
			qos_release(iterator.datum_index);
		}
		g_4cf8d8->valid = 0;
		g_4cf8d4 = false;
	}
}


/* the session description as the network codecs pack it into a bit stream */
struct s_session_packet
{
	word flag0;
	byte unknown2;
	char field3;
	word name[32];
	long type;
	dword field48;
	dword field4c;
	dword field50;
	dword field54;
	dword field58;
	byte unknown5c[0x18];
	dword field74;
	dword field78;
	dword field7c;
	dword field80;
	dword field84;
	dword field88;
	byte unknown8c[0x18];
	dword fielda4;
	dword fielda8;
	dword fieldac;
	byte unknownb0[4];
	dword fieldb4;
	byte unknownb8[0x14];
	char fieldcc[12];
	byte unknownd8[0x18];
	union
	{
		dword d[6];
		short w[14];
	} u;
};

/* 1947e0 with a constant bit count, expanded inline as retail does */
static __inline void stream_write_checked(s_bitstream *stream, dword value, long bits)
{
	if (bits < 32 && value >= (dword)(1 << bits))
	{
		char message[256];
		message[0] = 0;
		csprintf_256(message, "%u exceeds max value of %u", value, 1 << bits);
	}
	function_195720(stream, value, bits);
}

/* a value in 0..maximum, written in the fewest bits that hold the maximum */
#define STREAM_WRITE_RANGE(stream, value, maximum) \
	{ \
		long bits = 0; \
		do \
		{ \
			bits++; \
		} \
		while (((1 << bits) - 1) < (maximum)); \
		function_1947e0((stream), (value), bits); \
	}

// @retail 0x7cc50
void function_07cc50(s_bitstream *stream, const s_session_packet *packet)
{
	long i;

	stream_write_checked(stream, packet->type, 4);
	if (packet->type)
	{
		stream_write_checked(stream, packet->flag0, 1);
		const word *character = packet->name;
		for (i = 0; i < 32; i++)
		{
			word c = *character;
			function_195720(stream, c, 16);
			if (c == 0)
			{
				break;
			}
			character++;
		}
		stream_write_checked(stream, packet->field3 + 1, 7);
		stream_write_checked(stream, packet->field48, 15);
		STREAM_WRITE_RANGE(stream, packet->field4c, 6);
		stream_write_checked(stream, packet->field50, 16);
		stream_write_checked(stream, packet->field54, 16);
		STREAM_WRITE_RANGE(stream, packet->field58, 3);
		STREAM_WRITE_RANGE(stream, packet->field74, 0x10);
		STREAM_WRITE_RANGE(stream, packet->field78, 0x10);
		stream_write_checked(stream, packet->field7c, 16);
		stream_write_checked(stream, packet->field80, 16);
		stream_write_checked(stream, packet->field84, 16);
		STREAM_WRITE_RANGE(stream, packet->field88, 2);
		STREAM_WRITE_RANGE(stream, packet->fielda4, 2);
		STREAM_WRITE_RANGE(stream, packet->fielda8, 2);
		stream_write_checked(stream, packet->fieldac, 16);
		STREAM_WRITE_RANGE(stream, packet->fieldb4, 8);
		STREAM_WRITE_RANGE(stream, packet->fieldcc[0], 3);
		STREAM_WRITE_RANGE(stream, packet->fieldcc[1], 7);
		STREAM_WRITE_RANGE(stream, packet->fieldcc[2], 7);
		STREAM_WRITE_RANGE(stream, packet->fieldcc[3], 4);
		STREAM_WRITE_RANGE(stream, packet->fieldcc[4], 4);
		STREAM_WRITE_RANGE(stream, packet->fieldcc[5], 4);
		STREAM_WRITE_RANGE(stream, packet->fieldcc[6], 6);
		STREAM_WRITE_RANGE(stream, packet->fieldcc[7], 6);
		STREAM_WRITE_RANGE(stream, packet->fieldcc[8], 0x13);
		STREAM_WRITE_RANGE(stream, packet->fieldcc[9], 3);
		STREAM_WRITE_RANGE(stream, packet->fieldcc[10], 0x14);
		STREAM_WRITE_RANGE(stream, packet->fieldcc[11], 0x14);

		switch (packet->type)
		{
		case 5:
			stream_write_checked(stream, packet->u.w[12], 16);
			stream_write_checked(stream, packet->u.w[13], 16);
		case 1:
			stream_write_checked(stream, packet->u.d[0], 8);
			stream_write_checked(stream, packet->u.d[1], 16);
			STREAM_WRITE_RANGE(stream, packet->u.d[2], 2);
			STREAM_WRITE_RANGE(stream, packet->u.d[3], 1);
			STREAM_WRITE_RANGE(stream, packet->u.d[4], 3);
			STREAM_WRITE_RANGE(stream, packet->u.d[5], 2);
			break;
		case 2:
			stream_write_checked(stream, packet->u.d[0], 3);
			break;
		case 3:
			stream_write_checked(stream, packet->u.d[0], 3);
			STREAM_WRITE_RANGE(stream, packet->u.w[2], 3);
			STREAM_WRITE_RANGE(stream, packet->u.w[3], 1);
			STREAM_WRITE_RANGE(stream, packet->u.w[4], 2);
			STREAM_WRITE_RANGE(stream, packet->u.w[5], 3);
			break;
		case 4:
			stream_write_checked(stream, packet->u.d[0], 5);
			stream_write_checked(stream, packet->u.w[2], 16);
			break;
		case 7:
			stream_write_checked(stream, packet->u.d[0], 7);
			STREAM_WRITE_RANGE(stream, packet->u.w[2], 2);
			break;
		case 8:
			STREAM_WRITE_RANGE(stream, packet->u.w[0], 8);
			stream_write_checked(stream, packet->u.w[1], 16);
			stream_write_checked(stream, packet->u.w[2], 16);
			break;
		default:
			__assume(0);
		}
	}
}