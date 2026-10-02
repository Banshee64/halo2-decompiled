// @flags /O2 /Gr
#include "cseries.h"
#include "unknown_1946f0.h"
#include "unknown_0ac490.h"
#include <string.h>

extern byte g_440070[12];

// @retail 0x000ac490
void __stdcall message_ping_encode(s_bitstream *stream, long size, s_message_ping *message)
{
	stream_write_checked(stream, message->id, 16);
	function_195720(stream, message->timestamp, 32);
	stream_write_bit(stream, message->flag);
}

// @retail 0x000ac530
bool __stdcall message_ping_decode(s_bitstream *stream, long size, s_message_ping *message)
{
	message->id = (word)function_1959c0(stream, 16);
	message->timestamp = function_1959c0(stream, 32);
	message->flag = function_1957d0(stream);
	return stream_overflowed(stream) ? false : true;
}

// @retail 0x000ac580
void __stdcall message_pong_encode(s_bitstream *stream, long size, s_message_pong *message)
{
	stream_write_checked(stream, message->id, 16);
	function_195720(stream, message->timestamp, 32);
	stream_write_checked(stream, message->state, 2);
}

// @retail 0x000ac610
bool __stdcall message_pong_decode(s_bitstream *stream, long size, s_message_pong *message)
{
	message->id = (word)function_1959c0(stream, 16);
	message->timestamp = function_1959c0(stream, 32);
	message->state = function_1959c0(stream, 2);
	if (stream_overflowed(stream) || message->state < 0 || message->state >= 3)
		return false;
	return true;
}

// @retail 0x000ac670
void __stdcall message_broadcast_search_encode(s_bitstream *stream, long size, s_message_broadcast_search *message)
{
	stream_write_checked(stream, message->id, 16);
	function_1955d0(stream, &message->nonce, 64);
}

// @retail 0x000ac6e0
bool __stdcall message_broadcast_search_decode(s_bitstream *stream, long size, s_message_broadcast_search *message)
{
	message->id = (word)function_1959c0(stream, 16);
	function_195820(stream, &message->nonce, 64);
	return stream_overflowed(stream) ? false : true;
}

// @retail 0x000ac730
void __stdcall message_broadcast_reply_encode(s_bitstream *stream, long size, s_message_broadcast_reply *message)
{
	stream_write_checked(stream, message->id, 16);
	function_1955d0(stream, &message->nonce, 64);
	function_07ba10(stream, &message->session);
}

// @retail 0x000ac7a0
bool __stdcall message_broadcast_reply_decode(s_bitstream *stream, long size, s_message_broadcast_reply *message)
{
	message->id = (word)function_1959c0(stream, 16);
	function_195820(stream, &message->nonce, 64);
	return function_07c110(stream, &message->session) && !stream_overflowed(stream);
}

// @retail 0x000ac800
void message_types_register_discovery(s_message_type *types)
{
	message_type_define(&types[0], "ping", sizeof(s_message_ping), (t_message_encode)message_ping_encode, (t_message_decode)message_ping_decode);
	message_type_define(&types[1], "pong", sizeof(s_message_pong), (t_message_encode)message_pong_encode, (t_message_decode)message_pong_decode);
	message_type_define(&types[2], "broadcast-search", sizeof(s_message_broadcast_search), (t_message_encode)message_broadcast_search_encode, (t_message_decode)message_broadcast_search_decode);
	message_type_define(&types[3], "broadcast-reply", sizeof(s_message_broadcast_reply), (t_message_encode)message_broadcast_reply_encode, (t_message_decode)message_broadcast_reply_decode);
}

// @retail 0x000ac8a0
void __stdcall message_connect_request_encode(s_bitstream *stream, long size, s_message_connect_request *message)
{
	function_195720(stream, message->identifier, 32);
	stream_write_checked(stream, message->channel, 8);
}

// @retail 0x000ac900
bool __stdcall message_connect_request_decode(s_bitstream *stream, long size, s_message_connect_request *message)
{
	message->identifier = function_1959c0(stream, 32);
	message->channel = function_1959c0(stream, 8);
	return stream_overflowed(stream) ? false : true;
}

// @retail 0x000ac940
void __stdcall message_connect_refuse_encode(s_bitstream *stream, long size, s_message_connect_refuse *message)
{
	function_195720(stream, message->identifier, 32);
	stream_write_checked(stream, message->reason, 3);
}

// @retail 0x000ac9a0
bool __stdcall message_connect_refuse_decode(s_bitstream *stream, long size, s_message_connect_refuse *message)
{
	message->identifier = function_1959c0(stream, 32);
	message->reason = function_1959c0(stream, 3);
	return stream_overflowed(stream) ? false : true;
}

// @retail 0x000ac9e0
void __stdcall message_connect_establish_encode(s_bitstream *stream, long size, s_message_connect_establish *message)
{
	function_195720(stream, message->identifier, 32);
	function_195720(stream, message->remote_identifier, 32);
}

// @retail 0x000aca00
bool __stdcall message_connect_establish_decode(s_bitstream *stream, long size, s_message_connect_establish *message)
{
	message->identifier = function_1959c0(stream, 32);
	message->remote_identifier = function_1959c0(stream, 32);
	return stream_overflowed(stream) ? false : true;
}

// @retail 0x000aca40
void __stdcall message_connect_closed_encode(s_bitstream *stream, long size, s_message_connect_closed *message)
{
	function_195720(stream, message->identifier, 32);
	function_195720(stream, message->remote_identifier, 32);
	stream_write_checked(stream, message->reason, 5);
}

// @retail 0x000acab0
bool __stdcall message_connect_closed_decode(s_bitstream *stream, long size, s_message_connect_closed *message)
{
	message->identifier = function_1959c0(stream, 32);
	message->remote_identifier = function_1959c0(stream, 32);
	message->reason = function_1959c0(stream, 5);
	if (stream_overflowed(stream) || message->reason < 0 || message->reason >= 18)
		return false;
	return true;
}

// @retail 0x000acb10
void message_types_register_connection(s_message_type *types)
{
	message_type_define(&types[4], "connect-request", sizeof(s_message_connect_request), (t_message_encode)message_connect_request_encode, (t_message_decode)message_connect_request_decode);
	message_type_define(&types[5], "connect-refuse", sizeof(s_message_connect_refuse), (t_message_encode)message_connect_refuse_encode, (t_message_decode)message_connect_refuse_decode);
	message_type_define(&types[6], "connect-establish", sizeof(s_message_connect_establish), (t_message_encode)message_connect_establish_encode, (t_message_decode)message_connect_establish_decode);
	message_type_define(&types[7], "connect-closed", sizeof(s_message_connect_closed), (t_message_encode)message_connect_closed_encode, (t_message_decode)message_connect_closed_decode);
}

// @retail 0x000acc20
void __stdcall message_join_request_encode(s_bitstream *stream, long size, s_message_join_request *message)
{
	stream_write_checked(stream, message->id, 16);
	function_1955d0(stream, &message->nonce, 64);
	function_1955d0(stream, &message->nonce2, 64);
	function_1955d0(stream, &message->nonce3, 64);
	function_1955d0(stream, &message->session_data, 288);
	stream_write_checked(stream, message->player_count, 5);
	for (long i = 0; i < message->player_count; i++)
	{
		function_1955d0(stream, &message->players[i], 96);
		stream_write_checked(stream, message->player_indices[i] + 1, 8);
		stream_write_checked(stream, message->player_values[i] + 1, 31);
	}
	stream_write_bit(stream, message->has_secure_address);
	if (message->has_secure_address)
		function_1955d0(stream, &message->secure_address, 32);
	stream_write_checked(stream, message->join_type, 2);
	if (message->join_type == 2)
	{
		stream_write_checked(stream, message->data0, 7);
		stream_write_checked(stream, message->data1, 7);
		stream_write_checked(stream, message->data2, 7);
		function_1955d0(stream, &message->address0, 32);
		function_1955d0(stream, &message->address1, 32);
		function_1955d0(stream, &message->address3, 32);
		function_1955d0(stream, &message->address2, 32);
		bool has_xnaddr = memcmp(&message->xnaddr, g_440070, sizeof(message->xnaddr)) != 0;
		stream_write_bit(stream, has_xnaddr);
		if (has_xnaddr)
			function_1955d0(stream, &message->xnaddr, 96);
	}
}

// @retail 0x000acfa0
bool __stdcall message_join_request_decode(s_bitstream *stream, long size, s_message_join_request *message)
{
	message->id = (word)function_1959c0(stream, 16);
	function_195820(stream, &message->nonce, 64);
	function_195820(stream, &message->nonce2, 64);
	function_195820(stream, &message->nonce3, 64);
	function_195820(stream, &message->session_data, 288);
	message->player_count = function_1959c0(stream, 5);
	for (long i = 0; i < message->player_count; i++)
	{
		function_195820(stream, &message->players[i], 96);
		message->player_indices[i] = function_1959c0(stream, 8) - 1;
		message->player_values[i] = function_1959c0(stream, 31) - 1;
	}
	message->has_secure_address = stream_read_bit(stream);
	if (message->has_secure_address)
		function_195820(stream, &message->secure_address, 32);
	message->join_type = function_1959c0(stream, 2);
	if (message->join_type == 2)
	{
		message->data0 = function_1959c0(stream, 7);
		message->data1 = function_1959c0(stream, 7);
		message->data2 = function_1959c0(stream, 7);
		function_195820(stream, &message->address0, 32);
		function_195820(stream, &message->address1, 32);
		function_195820(stream, &message->address3, 32);
		function_195820(stream, &message->address2, 32);
		if (stream_read_bit(stream))
			function_195820(stream, &message->xnaddr, 96);
		else
			memset(&message->xnaddr, 0, sizeof(message->xnaddr));
	}
	if (stream_overflowed(stream) || message->player_count < 0)
		return false;
	return true;
}

// @retail 0x000ad1c0
void __stdcall message_join_abort_encode(s_bitstream *stream, long size, s_message_join_abort *message)
{
	function_1955d0(stream, &message->nonce, 64);
	function_1955d0(stream, &message->nonce2, 64);
}

// @retail 0x000ad1e0
bool __stdcall message_join_abort_decode(s_bitstream *stream, long size, s_message_join_abort *message)
{
	function_195820(stream, &message->nonce, 64);
	function_195820(stream, &message->nonce2, 64);
	if (!stream_overflowed(stream))
		return true;
	return false;
}

// @retail 0x000ad230
void __stdcall message_join_refuse_encode(s_bitstream *stream, long size, s_message_join_refuse *message)
{
	function_1955d0(stream, &message->nonce, 64);
	stream_write_checked(stream, message->reason, 4);
}

// @retail 0x000ad290
bool __stdcall message_join_refuse_decode(s_bitstream *stream, long size, s_message_join_refuse *message)
{
	function_195820(stream, &message->nonce, 64);
	message->reason = function_1959c0(stream, 4);
	return stream_overflowed(stream) ? false : true;
}
