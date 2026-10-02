// @flags /O2 /Gr
/* UNKNOWN_0AF5F0.CPP: the last network message codec of the membership
   family and the function that registers the family's eight message types in
   the message type table */

#include "cseries.h"
#include "bitstream.h"
#include "unknown_1946f0.h"
#include "unknown_0af5f0.h"

static __inline bool stream_overflowed(s_bitstream *stream)
{
	bool result = stream->bit_position > (stream->size_in_bytes << 3);
	if (stream->error)
		result = true;
	return result;
}

struct s_message_header
{
	byte data[8];
};

/* 0xaf540, 0xaf5f0 */
struct s_message_0af540
{
	s_message_header header;
	long index;
	long index2;
	byte unknown10[0x90];
	dword value;
};

// @retail 0xaf5f0
bool __stdcall function_0af5f0(s_bitstream *stream, long unused, s_message_0af540 *message)
{
	function_195820(stream, message, 0x40);
	message->index = function_1959c0(stream, 2);
	message->index2 = function_1959c0(stream, 2);
	bool valid = function_07ca70(stream, message->unknown10);
	message->value = function_1959c0(stream, 0x20);
	if (valid && !stream_overflowed(stream) && message->index >= 0 && message->index < 4 && message->index2 >= 0 && message->index2 < 4)
		return true;
	return false;
}

// @retail 0xaf680
void message_types_register_membership(s_message_type *types)
{
	message_type_define(&types[25], "membership-update", 0x489c, (t_message_encode)function_0adef0, (t_message_decode)function_0ae7f0);
	message_type_define(&types[26], "peer-properties", 0xd0, (t_message_encode)function_0aedb0, (t_message_decode)function_0af050);
	message_type_define(&types[27], "delegate-leader", 0x2c, (t_message_encode)function_0af1f0, (t_message_decode)function_0af220);
	message_type_define(&types[28], "boot-machine", 0x2c, (t_message_encode)function_0af1f0, (t_message_decode)function_0af220);
	message_type_define(&types[29], "player-add", 0xb0, (t_message_encode)function_0af270, (t_message_decode)function_0af320);
	message_type_define(&types[30], "player-refuse", 0x18, (t_message_encode)function_0af3c0, (t_message_decode)function_0af430);
	message_type_define(&types[31], "player-remove", 0xc, (t_message_encode)function_0af490, (t_message_decode)function_0af4f0);
	message_type_define(&types[32], "player-properties", 0xa4, (t_message_encode)function_0af540, (t_message_decode)function_0af5f0);
}
