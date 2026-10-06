// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_07A9A0.CPP: the transport layer's local address cache, address
   resolution against the XNet key registry, and the QoS handle pool */

#include "unknown_11c920.h"
#include "data_array.h"
#include "globals.h"
#include "bitstream.h"
#include "unknown_07aec0.h"
#include "unknown_1946f0.h"
#include "unknown_07f720.h"
#include <xtl.h>
#include <string.h>

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
s_type_99af70 g_4cf7b8;
byte g_4cf7cc[6];
s_xnet_registry_entry g_4cf7d4[8];

/* 0x46725c: points at the eight zero bytes of g_440070 (globals.h) */
const byte *g_46725c = g_440070;

void qos_release(long handle);
void function_07ad80(long count, byte *buffer);

/* 0x468758: the allocator the QoS pool and the online tasks
   (online_tasks.cpp) are built through. Retail's .data holds 0x479860, an
   allocator object not modelled yet, so it starts zeroed here like g_46875c. */
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
					g_4cf7b8.address_length = k_ipv4_address_length;
					g_4cf7b8.ipv4_address = byte_swap_long(xna.ina.s_addr);
					g_4cf7b8.port = 0;
				}
				return g_4cf791 = true;
			}
		}
	}
	return g_4cf791;
}

// @retail 0x7ab60
bool function_07ab60(const s_type_99af70 *address, bool local, long *index_out, XNKID *kid_out, XNKEY *key_out, XNADDR *xnaddr_out)
{
	bool result = false;
	long index = NONE;
	dword ina = 0;
	XNKID kid;
	XNKEY key;

	memset(&kid, 0, sizeof(kid));
	memset(&key, 0, sizeof(key));

	if (address->address_length == k_ipv4_address_length && address->ipv4_address)
	{
		if (local)
		{
			memset(xnaddr_out, 0, sizeof(*xnaddr_out));
			xnaddr_out->ina.s_addr = byte_swap_long(address->ipv4_address);
			result = true;
		}
		else if (function_07aec0(address, &ina))
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
bool function_07acc0(const s_type_99af70 *address)
{
	bool result = false;
	dword ina;

	if (function_07aec0(address, &ina))
	{
		IN_ADDR in_addr;

		in_addr.s_addr = ina;
		result = XNetConnect(in_addr) == 0;
	}

	return result;
}

// @retail 0x7acf0
long function_07acf0(const s_type_99af70 *address)
{
	dword ina;

	if (function_07aec0(address, &ina))
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

// @retail 0x7b3e0
void function_07b3e0(void)
{
	memset(&g_4cf8d4, 0, 8);
	g_4cf8d8 = data_new_inlined("transport qos attempts", 0x20, 8, 0, g_468758);
}

/* retail inlines the absolute-index walk of function_16bc00 here */
static __inline long next_absolute_index(s_record_pool *data, long index)
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

static __inline byte *iterator_next(s_record_pool_iterator *iterator)
{
	s_record_pool *data = iterator->data;
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
		s_record_pool_iterator iterator;

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
	byte unknown10c[0x130 - 0x10c];
};

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
void __stdcall function_07cc50(s_bitstream *stream, void *part)
{
	const s_session_packet *packet = (const s_session_packet *)part;
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
bool function_153750(s_player_appearance *appearance);

// @retail 0x7efa0
bool function_7efa0(s_bitstream *stream, s_player_appearance *appearance)
{
	memset(appearance, 0, sizeof(*appearance));
	appearance->colors[0] = (char)(function_1959c0(stream, 5) - 1);
	appearance->colors[1] = (char)(function_1959c0(stream, 5) - 1);
	appearance->colors[2] = (char)(function_1959c0(stream, 5) - 1);
	appearance->colors[3] = (char)(function_1959c0(stream, 5) - 1);
	appearance->unknown04[0] = (byte)(function_1959c0(stream, 3) - 1);
	appearance->unknown04[1] = (byte)function_1959c0(stream, 6);
	appearance->unknown04[2] = (byte)function_1959c0(stream, 6);
	appearance->unknown04[3] = (byte)function_1959c0(stream, 4);
	return function_153750(appearance);
}

struct s_session_message_values
{
	short unknown00;
	short type;
	long mode;
	long value08;
	long value0c;
	long value10;
	byte data14[8];
	byte data1c[16];
	byte data2c[36];
	long value50;
	long value54;
	long count58;
	long count5c;
	long count60;
	long count64;
	dword value68;
	dword value6c;
	long index70;
	long value74;
	long value78;
	long value7c;
	long value80;
	long value84;
	long count88;
	dword identities[16][3];
};

// @retail 0x7da60
bool function_7da60(const s_session_message_values *values)
{
	if (values && values->type >= 0 && values->type < 2 &&
		values->mode >= 0 && values->mode < 5 &&
		(values->value08 == NONE || (values->value08 > 0 && values->value08 <= 0xfffe)) &&
		(values->value0c == NONE || (values->value0c > 0 && values->value0c <= 0xfffe)) &&
		values->count58 >= 0 && values->count58 <= 16 &&
		values->count5c >= 0 && values->count5c <= 16 &&
		values->count60 >= 0 && values->count60 <= 16 &&
		values->count64 >= 0 && values->count64 <= 16 &&
		values->value68 < 0x10000 && values->value6c < 0x10000 &&
		values->index70 >= 0 && values->index70 < 16)
		return true;
	return false;
}

// @retail 0x7df00
bool function_7df00(s_bitstream *stream, s_session_message_values *values)
{
	values->unknown00 = (short)function_1959c0(stream, 8);
	values->type = (short)function_1959c0(stream, 2);
	values->mode = function_1959c0(stream, 3);
	values->value08 = function_1959c0(stream, 16) - 1;
	values->value0c = function_1959c0(stream, 16) - 1;
	values->value10 = function_1959c0(stream, 4);
	function_195820(stream, values->data14, 64);
	function_195820(stream, values->data1c, 128);
	function_195820(stream, values->data2c, 288);
	function_195820(stream, &values->value50, 32);
	function_195820(stream, &values->value54, 32);
	values->count58 = function_1959c0(stream, 5);
	values->count5c = function_1959c0(stream, 5);
	values->count60 = function_1959c0(stream, 5);
	values->count64 = function_1959c0(stream, 5);
	values->value68 = function_1959c0(stream, 16);
	values->value6c = function_1959c0(stream, 16);
	values->index70 = function_1959c0(stream, 4);
	function_195820(stream, &values->value74, 32);
	values->value78 = function_1959c0(stream, 7);
	values->value7c = function_1959c0(stream, 7);
	values->value80 = function_1959c0(stream, 7);
	values->value84 = function_1959c0(stream, 7);
	values->count88 = function_1959c0(stream, 4);
	for (long i = 0; i < 16; i++)
	{
		if (i < values->count88)
			function_195820(stream, values->identities[i], 96);
		else
			memset(values->identities[i], 0, sizeof(values->identities[i]));
	}
	if (!stream_overflowed(stream) && function_7da60(values))
		return true;
	return false;
}

struct s_message_identity
{
	dword words[3];
};

struct s_message_player_identity
{
	s_message_identity identity;
	byte unknown0c[0x13c - 0xc];
};

struct s_message_identities
{
	byte unknown00[0x10d0];
	dword player_mask;
	byte unknown10d4[0x11ec - 0x10d4];
	s_message_player_identity players[16];
};

// @retail 0x7ed20
bool function_7ed20(const s_message_identities *message, s_message_identity *common, bool *missing_out, bool *different_out)
{
	bool found = false;
	bool missing = false;
	bool different = false;
	s_message_identity first;
	dword mask = message->player_mask;
	for (long i = 0; i < 16; i++)
	{
		if (mask & (1 << i))
		{
			if (memcmp(&message->players[i].identity, g_440070, sizeof(first)) != 0)
			{
				if (found)
				{
					if (memcmp(&first, &message->players[i].identity, sizeof(first)) != 0)
						different = true;
				}
				else
				{
					first = message->players[i].identity;
					found = true;
				}
			}
			else
				missing = true;
		}
	}
	bool result = found && !missing && !different;
	if (missing_out)
		*missing_out = missing;
	if (different_out)
		*different_out = different;
	if (result && common)
		*common = first;
	return result;
}

// @retail 0x7ee10
void function_7ee10(s_bitstream *stream, const s_player_appearance *appearance)
{
	stream_write_checked(stream, appearance->colors[0] + 1, 5);
	stream_write_checked(stream, appearance->colors[1] + 1, 5);
	stream_write_checked(stream, appearance->colors[2] + 1, 5);
	stream_write_checked(stream, appearance->colors[3] + 1, 5);
	stream_write_checked(stream, (char)appearance->unknown04[0] + 1, 3);
	stream_write_checked(stream, appearance->unknown04[1], 6);
	stream_write_checked(stream, appearance->unknown04[2], 6);
	stream_write_checked(stream, appearance->unknown04[3], 4);
}

/* the title's address: the XNet address and the transport address, read
   again while the transport runs; true when the title has one */
// @retail 0x7aaa0
bool function_07aaa0(XNADDR *xnaddr, s_type_99af70 *address)
{
	if (g_transport_globals.initialized && g_transport_globals.started)
		function_07a9b0();
	if (xnaddr)
		*xnaddr = g_4cf793;
	if (address)
		*address = g_4cf7b8;
	return g_4cf792;
}

#include "unknown_0662e0.h"

struct s_member_quality_entry
{
	byte unknown00[0x90];
	long first;
	long second;
	long kind;
	dword mask;
	byte unknowna0[0x10c - 0xa0];
};
struct s_member_quality_collection
{
	byte unknown00[8];
	long count;
	s_member_quality_entry entries[16];
};

// @retail 0x7e100
void function_7e100(long current, const s_member_quality_collection *collection,
	long *selected, long *first, long *second, long *level)
{
	long best = NONE;
	long best_first = 0;
	long best_second = 0;
	if (current != NONE)
	{
		best = current;
		best_first = collection->entries[current].first;
		best_second = collection->entries[current].second;
	}
	long count = collection->count;
	for (long i = 0; i < count; i++)
	{
		const s_member_quality_entry *entry = &collection->entries[i];
		if ((!g_network_configuration.flag1ac || entry->kind != 3) &&
			entry->first >= g_network_configuration.valued0 &&
			entry->second >= g_network_configuration.valuecc &&
			entry->mask == (dword)((1 << count) - 1) &&
			entry->second >= best_second + g_network_configuration.valued4)
		{
			best = i;
			best_first = entry->first;
			best_second = entry->second;
		}
	}
	long quality = 0;
	for (long j = 16; j > 0; j--)
	{
		if (best_second >= g_network_configuration.value40[j])
		{
			quality = j;
			break;
		}
	}
	if (quality <= g_network_configuration.valuec8)
		quality = g_network_configuration.valuec8;
	if (selected)
		*selected = best;
	if (first)
		*first = best_first;
	if (second)
		*second = best_second;
	if (level)
		*level = quality;
}

struct s_session_description_payload
{
	short field0;
	short field2;
	long field4;
	long field8;
	long fieldc;
	long field10;
	byte field14[8];
	byte field1c[16];
	byte field2c[36];
	long field50;
	long field54;
	long field58;
	long field5c;
	long field60;
	long field64;
	long field68;
	long field6c;
	long field70;
	long field74;
	long field78;
	long field7c;
	long field80;
	long field84;
	long count;
	byte identities[16][12];
};

// @retail 0x7db10
void function_7db10(s_bitstream *stream, const s_session_description_payload *message)
{
	stream_write_checked(stream, message->field0, 8);
	stream_write_checked(stream, message->field2, 2);
	stream_write_checked(stream, message->field4, 3);
	stream_write_checked(stream, message->field8 + 1, 16);
	stream_write_checked(stream, message->fieldc + 1, 16);
	stream_write_checked(stream, message->field10, 4);
	function_1955d0(stream, message->field14, 64);
	function_1955d0(stream, message->field1c, 128);
	function_1955d0(stream, message->field2c, 288);
	function_1955d0(stream, &message->field50, 32);
	function_1955d0(stream, &message->field54, 32);
	stream_write_checked(stream, message->field58, 5);
	stream_write_checked(stream, message->field5c, 5);
	stream_write_checked(stream, message->field60, 5);
	stream_write_checked(stream, message->field64, 5);
	stream_write_checked(stream, message->field68, 16);
	stream_write_checked(stream, message->field6c, 16);
	stream_write_checked(stream, message->field70, 4);
	function_1955d0(stream, &message->field74, 32);
	stream_write_checked(stream, message->field78, 7);
	stream_write_checked(stream, message->field7c, 7);
	stream_write_checked(stream, message->field80, 7);
	stream_write_checked(stream, message->field84, 7);
	stream_write_checked(stream, message->count, 4);
	for (long i = 0; i < message->count; i++)
		function_1955d0(stream, message->identities[i], 96);
}

#include "unknown_19d220.h"
bool function_19d650(s_game_variant *variant);

#define SESSION_READ_RANGE(destination, type, maximum) \
	{ \
		long bits = 0; \
		do { bits++; } while (((1 << bits) - 1) < (maximum)); \
		(destination) = (type)function_1959c0(stream, bits); \
	}

// @retail 0x7d520
bool function_07d520(s_bitstream *stream, void *part)
{
	bool result = true;
	s_session_packet *packet = (s_session_packet *)part;
	packet->type = function_1959c0(stream, 4);
	if (packet->type)
	{
		packet->flag0 = (word)function_1959c0(stream, 1);
		function_194fa0(stream, packet->name, 32);
		packet->field3 = (char)(function_1959c0(stream, 7) - 1);
		packet->field48 = (dword)function_1959c0(stream, 15);
		SESSION_READ_RANGE(packet->field4c, dword, 6);
		packet->field50 = (dword)function_1959c0(stream, 16);
		packet->field54 = (dword)function_1959c0(stream, 16);
		SESSION_READ_RANGE(packet->field58, dword, 3);
		SESSION_READ_RANGE(packet->field74, dword, 0x10);
		SESSION_READ_RANGE(packet->field78, dword, 0x10);
		packet->field7c = (dword)function_1959c0(stream, 16);
		packet->field80 = (dword)function_1959c0(stream, 16);
		packet->field84 = (dword)function_1959c0(stream, 16);
		SESSION_READ_RANGE(packet->field88, dword, 2);
		SESSION_READ_RANGE(packet->fielda4, dword, 2);
		SESSION_READ_RANGE(packet->fielda8, dword, 2);
		packet->fieldac = (dword)function_1959c0(stream, 16);
		SESSION_READ_RANGE(packet->fieldb4, dword, 8);
		SESSION_READ_RANGE(packet->fieldcc[0], char, 3);
		SESSION_READ_RANGE(packet->fieldcc[1], char, 7);
		SESSION_READ_RANGE(packet->fieldcc[2], char, 7);
		SESSION_READ_RANGE(packet->fieldcc[3], char, 4);
		SESSION_READ_RANGE(packet->fieldcc[4], char, 4);
		SESSION_READ_RANGE(packet->fieldcc[5], char, 4);
		SESSION_READ_RANGE(packet->fieldcc[6], char, 6);
		SESSION_READ_RANGE(packet->fieldcc[7], char, 6);
		SESSION_READ_RANGE(packet->fieldcc[8], char, 0x13);
		SESSION_READ_RANGE(packet->fieldcc[9], char, 3);
		SESSION_READ_RANGE(packet->fieldcc[10], char, 0x14);
		SESSION_READ_RANGE(packet->fieldcc[11], char, 0x14);

		switch (packet->type)
		{
		case 9:
			packet->u.w[12] = (short)function_1959c0(stream, 16);
			packet->u.w[13] = (short)function_1959c0(stream, 16);
		case 1:
			packet->u.d[0] = (dword)function_1959c0(stream, 8);
			packet->u.d[1] = (dword)function_1959c0(stream, 16);
			SESSION_READ_RANGE(packet->u.d[2], dword, 2);
			SESSION_READ_RANGE(packet->u.d[3], dword, 1);
			SESSION_READ_RANGE(packet->u.d[4], dword, 3);
			SESSION_READ_RANGE(packet->u.d[5], dword, 2);
			break;
		case 2:
			packet->u.d[0] = (dword)function_1959c0(stream, 3);
			break;
		case 3:
			packet->u.d[0] = (dword)function_1959c0(stream, 3);
			SESSION_READ_RANGE(packet->u.w[2], short, 3);
			SESSION_READ_RANGE(packet->u.w[3], short, 1);
			SESSION_READ_RANGE(packet->u.w[4], short, 2);
			SESSION_READ_RANGE(packet->u.w[5], short, 3);
			break;
		case 4:
			packet->u.d[0] = (dword)function_1959c0(stream, 5);
			packet->u.w[2] = (short)function_1959c0(stream, 16);
			break;
		case 7:
			packet->u.d[0] = (dword)function_1959c0(stream, 7);
			SESSION_READ_RANGE(packet->u.w[2], short, 2);
			break;
		case 8:
			SESSION_READ_RANGE(packet->u.w[0], short, 8);
			packet->u.w[1] = (short)function_1959c0(stream, 16);
			packet->u.w[2] = (short)function_1959c0(stream, 16);
			break;
		default:
			result = false;
			break;
		}
		if (result)
		{
			s_game_variant checked;
			memcpy(&checked, packet, sizeof(checked));
			return function_19d650(&checked);
		}
	}
	else
		memset(packet, 0, sizeof(s_game_variant));
	return result;
}

#undef SESSION_READ_RANGE
