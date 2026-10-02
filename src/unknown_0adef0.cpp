/* UNKNOWN_0ADEF0.CPP: the encoder of the network message that
   unknown_0ae7f0.cpp (0xae7f0) decodes. Callbacks, hence stdcall */

#include "cseries.h"
#include "bitstream.h"
#include "unknown_1946f0.h"
#include "unknown_0ae7f0.h"

// @flags /O2 /Gr

/* writes a value that must fit in the given number of bits */
#define WRITE_BITS(stream, value, bits) \
	{ \
		dword written = (value); \
		if (written >= (1u << (bits))) \
		{ \
			char message[256]; \
			message[0] = 0; \
			csprintf_256(message, "%u exceeds max value of %u", written, 1u << (bits)); \
		} \
		function_195720(stream, written, bits); \
	}

/* function_194830, which retail inlines */
static __inline void stream_write_bit(s_bitstream *stream, bool value)
{
	if (stream->size_in_bytes * 8 - stream->bit_position >= 1 && value)
		stream->data[stream->bit_position / 8] |= (byte)(1 << (stream->bit_position % 8));
	stream->bit_position++;
}

/* every message starts with a 64 bit header */
struct s_message_header
{
	byte data[8];
};

/* one of the players in the 0xae7f0 message, 0x104 bytes */
struct s_message_player
{
	short index1;
	short index2;
	byte data[0x24];
	byte flag0;
	byte flag1;
	byte unknown2a[2];
	byte flag2;
	word words1[16];
	word words2[32];
	byte flag3;
	long value1;
	long value2;
	byte flag4;
	dword bits1;
	dword bits2;
	long value3;
	long value4;
	long value5;
	long value6;
	long value7;
	byte flag5;
	long values[4][4];
	byte flag6;
	long cell;
};

/* one of the entries in the 0xae7f0 message, 0x140 bytes */
struct s_message_entry
{
	short index1;
	short kind;
	byte point[0xc];
	short index2;
	short index3;
	byte flag;
	byte unknown15[3];
	long index4;
	byte unknown1c[0x90];
	byte unknownac[0x90];
	dword value;
};

struct s_message_0ae7f0
{
	s_message_header header;
	long value1;
	long value2;
	short player_count;
	short entry_count;
	s_message_player players[32];
	s_message_entry entries[32];
	byte flag;
	long value3;
};

// @retail 0xadef0
void __stdcall function_0adef0(s_bitstream *stream, long unused, s_message_0ae7f0 *message)
{
	long i;
	function_1955d0(stream, message, 0x40);
	function_195720(stream, message->value1, 0x20);
	if (message->value2 == NONE)
		stream_write_bit(stream, true);
	else
	{
		stream_write_bit(stream, false);
		function_195720(stream, message->value2, 0x20);
	}
	WRITE_BITS(stream, message->player_count, 5);
	WRITE_BITS(stream, message->entry_count, 5);
	for (i = 0; i < message->player_count; i++)
	{
		s_message_player *player = &message->players[i];
		if (player->index1 == NONE)
			stream_write_bit(stream, false);
		else
		{
			stream_write_bit(stream, true);
			WRITE_BITS(stream, player->index1, 4);
		}
		if (player->index2 == NONE)
			stream_write_bit(stream, false);
		else
		{
			stream_write_bit(stream, true);
			WRITE_BITS(stream, player->index2, 4);
		}
		function_1955d0(stream, player->data, 0x120);
		stream_write_bit(stream, TEST_FIELD_BIT(player->flag0));
		if (player->flag0)
		{
			stream_write_bit(stream, TEST_FIELD_BIT(player->flag1));
			stream_write_bit(stream, TEST_FIELD_BIT(player->flag2));
			if (player->flag2)
			{
				long j;
				word *words;
				for (j = 0, words = player->words1; j < 16; j++, words++)
				{
					word value = *words;
					function_195720(stream, value, 16);
					if (value == 0)
						break;
				}
				for (j = 0, words = player->words2; j < 32; j++, words++)
				{
					word value = *words;
					function_195720(stream, value, 16);
					if (value == 0)
						break;
				}
			}
			stream_write_bit(stream, TEST_FIELD_BIT(player->flag3));
			if (player->flag3)
			{
				WRITE_BITS(stream, player->value1, 3);
				WRITE_BITS(stream, player->value2, 7);
			}
			stream_write_bit(stream, TEST_FIELD_BIT(player->flag4));
			if (player->flag4)
			{
				function_1955d0(stream, &player->bits1, 0x20);
				function_1955d0(stream, &player->bits2, 0x20);
				WRITE_BITS(stream, player->value3, 2);
				WRITE_BITS(stream, player->value4, 16);
				WRITE_BITS(stream, player->value5, 11);
				WRITE_BITS(stream, player->value6, 11);
				WRITE_BITS(stream, player->value7, 11);
			}
			stream_write_bit(stream, TEST_FIELD_BIT(player->flag5));
			if (player->flag5)
			{
				long j;
				for (j = 0; j < 16; j++)
					WRITE_BITS(stream, player->values[0][j], 2);
			}
			stream_write_bit(stream, TEST_FIELD_BIT(player->flag6));
			if (player->flag6)
				WRITE_BITS(stream, player->cell, 4);
		}
	}
	for (i = 0; i < message->entry_count; i++)
	{
		s_message_entry *entry = &message->entries[i];
		WRITE_BITS(stream, entry->index1, 4);
		WRITE_BITS(stream, entry->kind, 2);
		if (entry->kind == 1)
		{
			function_1955d0(stream, entry->point, 0x60);
			WRITE_BITS(stream, entry->index2, 4);
			WRITE_BITS(stream, entry->index3, 2);
		}
		stream_write_bit(stream, TEST_FIELD_BIT(entry->flag));
		if (entry->flag)
		{
			WRITE_BITS(stream, entry->index4, 2);
			function_07c5a0(stream, entry->unknown1c);
			function_07c5a0(stream, entry->unknownac);
			function_195720(stream, entry->value, 0x20);
		}
	}
	stream_write_bit(stream, TEST_FIELD_BIT(message->flag));
	if (message->flag)
		WRITE_BITS(stream, message->value3, 4);
}
