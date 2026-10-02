/* UNKNOWN_0B0900.CPP: a network message decoder. It reads the message
   fields through the bit stream module (unknown_195720.cpp) and returns
   whether the message it read is valid. It is a callback, so it is stdcall */

#include "cseries.h"
#include "bitstream.h"
#include "unknown_1946f0.h"
#include "unknown_0ae7f0.h"
#include "unknown_0b0900.h"
#include <string.h>

// @flags /O2 /Gr

/* function_1946f0, which retail inlines */
static __inline bool stream_overflowed(s_bitstream *stream)
{
	bool result = stream->bit_position > (stream->size_in_bytes << 3);
	if (stream->error)
		result = true;
	return result;
}

/* every message starts with a 64 bit header */
struct s_message_header
{
	byte data[8];
};

/* one of the sixteen entries at 0x624, 0xe4 bytes */
struct s_message_entry
{
	bool flag0;
	bool flag1;
	word value1;
	dword value2;
	byte address[6];
	byte unknown0e[12];
	byte unknown1a[2];
	byte unknown1c[0xc8];
};

struct s_message_0b0900
{
	s_message_header header;
	long value1;
	long value2;
	bool flag10;
	bool flag11;
	byte unknown12[2];
	long value14;
	long value18;
	bool flag1c;
	byte unknown1d[3];
	long value20;
	bool flag24;
	byte unknown25[3];
	long value28;
	bool flag2c;
	byte unknown2d[3];
	long value30;
	long value34;
	bool flag38;
	bool flag39;
	byte data3a[8];
	bool flag42;
	byte unknown43;
	long value44;
	bool flag48;
	bool flag49;
	bool flag4a;
	byte unknown4b;
	long value4c;
	bool flag50;
	bool flag51;
	bool flag52;
	byte unknown53;
	long value54;
	long value58;
	long value5c;
	long value60;
	long value64;
	long value68;
	long value6c;
	bool flag70;
	byte unknown71[3];
	long value74;
	bool flag78;
	bool flag79;
	byte unknown7a[6];
	byte data80[8];
	bool flag88;
	bool flag89;
	byte unknown8a[2];
	byte data8c[0x308];
	bool flag394;
	byte unknown395[3];
	long value398;
	long value39c;
	byte string[0x80];
	bool flag420;
	byte unknown421[7];
	byte data428[8];
	bool flag430;
	byte unknown431[3];
	long value434;
	bool flag438;
	byte unknown439[3];
	long value43c;
	bool flag440;
	byte unknown441[3];
	byte data444[0x130];
	bool flag574;
	byte unknown575;
	word words[32];
	bool flag5b6;
	bool flag5b7;
	dword mask;
	byte addresses[16][6];
	byte unknown61c[8];
	s_message_entry entries[16];
	bool flag1464;
	bool flag1465;
	bool flag1466;
	byte unknown1467;
	short value1468;
	bool flag146a;
	bool flag146b;
	long value146c;
	long value1470;
	long value1474;
	byte data1478[12];
	bool flag1484;
	bool flag1485;
	byte unknown1486[2];
	byte data1488[0x44];
	bool flag14cc;
	byte unknown14cd[3];
	long value14d0;
};

// @retail 0xb0900
bool __stdcall function_0b0900(s_bitstream *stream, long unused, s_message_0b0900 *message)
{
	long i;
	bool valid = true;
	function_195820(stream, message, 0x40);
	message->value1 = function_1959c0(stream, 0x20);
	if (function_1957d0(stream))
		message->value2 = NONE;
	else
		message->value2 = function_1959c0(stream, 0x20);
	message->flag10 = function_1957d0(stream);
	if (message->flag10)
	{
		message->flag11 = function_1957d0(stream);
		message->value18 = function_1959c0(stream, 0x20);
		message->value14 = function_1959c0(stream, 5);
	}
	message->flag1c = function_1957d0(stream);
	if (message->flag1c)
		message->value20 = function_1959c0(stream, 4);
	message->flag24 = function_1957d0(stream);
	if (message->flag24)
		message->value28 = function_1959c0(stream, 2);
	message->flag38 = function_1957d0(stream);
	if (message->flag38)
	{
		message->flag39 = function_1957d0(stream);
		if (message->flag39)
			function_195820(stream, message->data3a, 0x40);
	}
	message->flag2c = function_1957d0(stream);
	if (message->flag2c)
	{
		message->value30 = function_1959c0(stream, 4) + 1;
		message->value34 = function_1959c0(stream, 4) + 1;
	}
	message->flag42 = function_1957d0(stream);
	if (message->flag42)
		message->value44 = function_1959c0(stream, 2);
	message->flag48 = function_1957d0(stream);
	if (message->flag48)
		message->flag49 = function_1957d0(stream);
	message->flag4a = function_1957d0(stream);
	if (message->flag4a)
		message->value4c = function_1959c0(stream, 5) - 1;
	message->flag50 = function_1957d0(stream);
	if (message->flag50)
		message->flag51 = function_1957d0(stream);
	message->flag52 = function_1957d0(stream);
	if (message->flag52)
	{
		message->value54 = function_1959c0(stream, 3);
		if (message->value54 == 1)
		{
			message->value58 = function_1959c0(stream, 4);
			message->value5c = function_1959c0(stream, 4);
			message->value60 = function_1959c0(stream, 6);
			message->value64 = function_1959c0(stream, 10);
		}
		else if (message->value54 == 2)
			message->value68 = function_1959c0(stream, 10);
		else if (message->value54 == 3)
			message->value6c = function_1959c0(stream, 10);
	}
	message->flag70 = function_1957d0(stream);
	if (message->flag70)
		message->value74 = function_1959c0(stream, 5);
	message->flag88 = function_1957d0(stream);
	if (message->flag88)
	{
		message->flag89 = function_1957d0(stream);
		if (message->flag89)
			valid = function_063980(stream, message->data8c);
	}
	message->flag78 = function_1957d0(stream);
	if (message->flag78)
	{
		message->flag79 = function_1957d0(stream);
		if (message->flag79)
			function_195820(stream, message->data80, 0x40);
	}
	message->flag394 = function_1957d0(stream);
	if (message->flag394)
	{
		message->value398 = function_1959c0(stream, 0x20);
		message->value39c = function_1959c0(stream, 0x20);
		i = 0;
		do
		{
			message->string[i] = (byte)function_1959c0(stream, 8);
			if (message->string[i] == 0)
				break;
			i++;
		} while (i < 0x80);
		if (i >= 0x80)
		{
			message->string[0x7f] = 0;
			stream->error = true;
		}
	}
	message->flag420 = function_1957d0(stream);
	if (message->flag420)
		function_195820(stream, message->data428, 0x40);
	message->flag430 = function_1957d0(stream);
	if (message->flag430)
		message->value434 = function_1959c0(stream, 0x20);
	message->flag438 = function_1957d0(stream);
	if (message->flag438)
		message->value43c = function_1959c0(stream, 2);
	message->flag440 = function_1957d0(stream);
	if (message->flag440)
		valid = valid && function_07d520(stream, message->data444);
	message->flag574 = function_1957d0(stream);
	if (message->flag574)
		function_194fa0(stream, message->words, 0x20);
	message->flag5b6 = function_1957d0(stream);
	if (message->flag5b6)
	{
		message->flag5b7 = function_1957d0(stream);
		if (message->flag5b7)
		{
			message->mask = function_1959c0(stream, 0x10);
			for (i = 0; i < 16; i++)
			{
				if (message->mask & (1 << i))
				{
					long j;
					function_195820(stream, message->addresses[i], 0x30);
					for (j = 0; j < i; j++)
					{
						if ((message->mask & (1 << j)) && memcmp(message->addresses[i], message->addresses[j], 6) == 0)
							valid = false;
					}
				}
			}
			for (i = 0; i < 16; i++)
			{
				message->entries[i].flag0 = stream_read_bit(stream);
				if (message->entries[i].flag0)
				{
					message->entries[i].flag1 = stream_read_bit(stream);
					if (message->entries[i].flag1)
					{
						message->entries[i].value1 = message->entries[i].value2 = NONE;
					}
					else
					{
						function_195820(stream, message->entries[i].address, 0x30);
						message->entries[i].value1 = (word)function_1959c0(stream, 2);
						message->entries[i].value2 = function_1959c0(stream, 2);
					}
					function_195820(stream, message->entries[i].unknown0e, 0x60);
					bool entry_valid = function_07ca70(stream, message->entries[i].unknown1c);
					if (valid && entry_valid)
					{
						long j;
						valid = true;
						if (!message->entries[i].flag1)
						{
							long found = NONE;
							for (j = 0; j < 16; j++)
							{
								if ((message->mask & (1 << j)) && memcmp(message->addresses[j], message->entries[i].address, 6) == 0)
									found = j;
							}
							valid = found != NONE;
						}
						for (j = 0; j < i; j++)
						{
							s_message_entry *other = &message->entries[j];
							if (other->flag0)
							{
								valid = valid && memcmp(message->entries[i].unknown0e, other->unknown0e, 12) != 0;
								if (!message->entries[i].flag1)
								{
									if (memcmp(message->entries[i].address, other->address, 6) == 0)
										valid = valid && message->entries[i].value1 != other->value1 && message->entries[i].value2 != other->value2;
								}
							}
						}
					}
					else
						valid = false;
				}
			}
		}
	}
	message->flag1464 = stream_read_bit(stream);
	if (message->flag1464)
		message->flag1465 = stream_read_bit(stream);
	message->flag1466 = stream_read_bit(stream);
	if (message->flag1466)
	{
		message->value1468 = (short)(function_1959c0(stream, 3) - 1);
		if (valid && (message->value1468 == NONE || (message->value1468 >= 0 && message->value1468 < 4)))
			valid = true;
		else
			valid = false;
	}
	message->flag146a = stream_read_bit(stream);
	if (message->flag146a)
	{
		message->flag146b = stream_read_bit(stream);
		message->value146c = function_1959c0(stream, 12);
		message->value1470 = function_1959c0(stream, 2);
		if (message->value1470 == 1 || message->value1470 == 2)
			message->value1474 = function_1959c0(stream, 12);
		else
			message->value1474 = NONE;
		if (message->value1470 == 1)
			function_195820(stream, message->data1478, 0x60);
	}
	message->flag1484 = stream_read_bit(stream);
	if (message->flag1484)
	{
		message->flag1485 = stream_read_bit(stream);
		if (message->flag1485)
			valid = valid && function_0b23d0(stream, message->data1488);
	}
	message->flag14cc = stream_read_bit(stream);
	if (message->flag14cc)
		message->value14d0 = function_1959c0(stream, 5) - 1;
	valid = valid && !stream_overflowed(stream) && (message->value2 == NONE || message->value2 < message->value1);
	if (message->flag10)
		valid = valid && message->value14 > 0 && message->value14 < 19 && message->value18 >= 0;
	if (message->flag1c)
		valid = valid && message->value20 >= 0 && message->value20 < 9;
	if (message->flag24)
		valid = valid && message->value28 >= 0 && message->value28 < 3;
	if (message->flag2c)
		valid = valid && message->value30 > 0 && message->value30 <= 16 && message->value34 > 0 && message->value34 <= 16;
	if (message->flag42)
		valid = valid && message->value44 >= 0 && message->value44 < 3;
	if (message->flag4a)
		valid = valid && (message->value4c == NONE || (message->value4c >= 0 && message->value4c < 16));
	if (message->flag52)
	{
		valid = valid && message->value54 >= 0 && message->value54 < 5;
		if (message->value54 == 1)
			valid = valid && message->value58 >= 0 && message->value58 <= 15 && message->value5c >= 0 && message->value5c <= 15 && message->value60 >= 0 && message->value60 <= 63 && message->value64 >= 0 && message->value64 <= 1023;
	}
	if (message->flag70)
		valid = valid && message->value74 >= 0 && message->value74 < 20;
	if (message->flag438)
		valid = valid && message->value43c >= 0 && message->value43c < 3;
	if (message->flag146a)
	{
		valid = valid && message->value146c >= 0 && message->value1470 >= 0 && message->value1470 < 3;
		if (message->value1470 == 1 || message->value1470 == 2)
			valid = valid && message->value1474 >= 0;
		else
			valid = valid && message->value1474 == NONE;
	}
	if (message->flag14cc)
	{
		long v = message->value14d0;
		valid = valid && ((v < 0 ? 0 : (v > 15 ? 15 : v)) == v || v == NONE);
	}
	return valid;
}
