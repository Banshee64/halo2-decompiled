/* UNKNOWN_0AC490.H: the network message codecs for the discovery (ping, pong,
   broadcast), connection (connect-*) and join message families, and the
   message type table entry their registration functions fill in */

#ifndef UNKNOWN_0AC490_H
#define UNKNOWN_0AC490_H

#include "cseries.h"
#include "bitstream.h"
#include "unknown_1946f0.h"

typedef void (__stdcall *t_message_encode)(s_bitstream *stream, long size, void *message);
typedef bool (__stdcall *t_message_decode)(s_bitstream *stream, long size, void *message);

/* one entry of the message type table; entries are 0x20 bytes apart */
struct s_message_type
{
	bool initialized;
	byte unknown01[3];
	char const *name;
	long unknown08;
	long minimum_size;
	long maximum_size;
	t_message_encode encode;
	t_message_decode decode;
	void *unknown1c;
};

inline void message_type_define(s_message_type *type, char const *name, long size, t_message_encode encode, t_message_decode decode)
{
	type->name = name;
	type->unknown08 = 0;
	type->minimum_size = size;
	type->maximum_size = size;
	type->encode = encode;
	type->decode = decode;
	type->unknown1c = NULL;
	type->initialized = true;
}

/* writes one bit; the codecs inline it */
inline void stream_write_bit(s_bitstream *stream, bool value)
{
	if ((stream->size_in_bytes << 3) - stream->bit_position >= 1 && value)
		stream->data[stream->bit_position / 8] |= (byte)(1 << (stream->bit_position % 8));
	stream->bit_position++;
}

/* the checked write and the overflow test the codecs inline (1947e0, 1946f0) */
inline void stream_write_checked(s_bitstream *stream, dword value, long bits)
{
	if (bits < 32 && value >= (dword)(1 << bits))
	{
		char message[256];
		message[0] = 0;
		csprintf_256(message, "%u exceeds max value of %u", value, 1 << bits);
	}
	function_195720(stream, value, bits);
}

inline bool stream_overflowed(s_bitstream *stream)
{
	bool result = stream->bit_position > (stream->size_in_bytes << 3);
	if (stream->error)
		result = true;
	return result;
}

struct s_message_ping
{
	word id;
	byte unknown02[2];
	long timestamp;
	bool flag;
};

struct s_message_pong
{
	word id;
	byte unknown02[2];
	long timestamp;
	long state;
};

struct s_message_broadcast_search
{
	word id;
	byte unknown02[2];
	byte nonce[8];
};

struct s_message_broadcast_reply
{
	word id;
	byte unknown02[2];
	byte nonce[8];
	byte session[0x714];
};

struct s_message_connect_request
{
	dword identifier;
	long channel;
};

struct s_message_connect_refuse
{
	dword identifier;
	long reason;
};

struct s_message_connect_establish
{
	dword identifier;
	dword remote_identifier;
};

struct s_message_connect_closed
{
	dword identifier;
	dword remote_identifier;
	long reason;
};

struct s_message_join_request
{
	word id;
	byte unknown02[2];
	byte nonce[8];
	long player_count;
	byte players[16][12];
	long player_indices[16];
	long player_values[16];
	byte nonce3[8];
	bool has_secure_address;
	byte unknown159[3];
	dword secure_address;
	long join_type;
	dword address0;
	dword address1;
	dword address2;
	dword address3;
	long data0;
	long data1;
	long data2;
	byte xnaddr[12];
	byte nonce2[8];
	byte session_data[36];
};

struct s_message_join_abort
{
	byte nonce[8];
	byte nonce2[8];
};

struct s_message_join_refuse
{
	byte nonce[8];
	long reason;
};

/* 0x7ba10 and 0x7c110: write and read a session description */
void __stdcall function_07ba10(s_bitstream *stream, void *session);
bool __stdcall function_07c110(s_bitstream *stream, void *session);

#endif
