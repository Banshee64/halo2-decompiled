// @flags /O2 /Gr
/* UNKNOWN_0AD2E0.CPP: the session protocol message codecs (leave, handoff,
   session control, host decline, election, time synchronize) and the
   registration of their message types */

#include "cseries.h"
#include "bitstream.h"
#include "unknown_1946f0.h"
#include <xtl.h>

/* every session message starts with the 64 bit session id */
struct s_session_id_message
{
	byte session_id[8];
};

struct s_handoff_message
{
	byte session_id[8];
	byte address[0x24];
	short peer_count;
};

struct s_host_decline_message
{
	byte session_id[8];
	bool has_reason;
	bool has_address_flag;
	bool has_address;
	byte unknownb;
	byte address[0x24];
};

struct s_election_message
{
	byte session_id[8];
	byte address0[0x24];
	byte address1[0x24];
	dword unknown50;
	dword unknown54;
	byte address2[4];
	long peer_count;
	byte peers[0x10][6];
	dword mask0;
	dword mask1;
};

struct s_election_refuse_message
{
	byte session_id[8];
	long reason;
	bool has_address;
	byte address[0x24];
};

struct s_time_synchronize_message
{
	byte session_id[8];
	dword unknown08;
	dword unknown0c;
	dword unknown10;
	dword unknown14;
	word type;
};

typedef void (__stdcall *t_message_encode)(s_bitstream *stream, long unknown, void const *message);
typedef bool (__stdcall *t_message_decode)(s_bitstream *stream, long unknown, void *message);
typedef bool (__stdcall *t_message_clear)(s_bitstream *stream, long unknown, void *message);

struct s_message_type
{
	bool initialized;
	char const *name;
	long unknown08;
	long minimum_size;
	long maximum_size;
	t_message_encode encode;
	t_message_decode decode;
	t_message_clear clear;
};

struct c_network_message_type_collection
{
	byte unknown00[0x100];
	s_message_type types[17];
};

/* the messages that carry only the session id each had their own codecs,
   which the linker folded into 0xad5a0, 0xad2e0 and 0xad3f0 */
#define SESSION_ID_ENCODE(name) \
	void __stdcall name##_encode(s_bitstream *stream, long unknown, s_session_id_message const *message) \
	{ \
		function_1955d0(stream, message, 0x40); \
	}

#define LEAVE_DECODE(name) \
	bool __stdcall name##_decode(s_bitstream *stream, long unknown, s_session_id_message *message) \
	{ \
		function_195820(stream, message->session_id, 0x40); \
		bool result = !stream_reading_failed(stream); \
		return result; \
	}

#define SESSION_CONTROL_DECODE(name) \
	bool __stdcall name##_decode(s_bitstream *stream, long unknown, s_session_id_message *message) \
	{ \
		function_195820(stream, message->session_id, 0x40); \
		if (!stream_reading_failed(stream)) \
			return true; \
		return false; \
	}

/* not decompiled yet: the time synchronize codecs call 0x758c0, and the join messages' codecs are in the neighbouring ranges */
dword function_758c0(void *message);
void __stdcall function_acc20(s_bitstream *stream, long unknown, void const *message);
bool __stdcall function_acfa0(s_bitstream *stream, long unknown, void *message);
void __stdcall function_ad1c0(s_bitstream *stream, long unknown, void const *message);
bool __stdcall function_ad1e0(s_bitstream *stream, long unknown, void *message);
void __stdcall function_ad230(s_bitstream *stream, long unknown, void const *message);
bool __stdcall function_ad290(s_bitstream *stream, long unknown, void *message);

static inline void stream_write_bits_checked(s_bitstream *stream, dword value, long bits)
{
	if (bits < 32 && value >= (dword)(1 << bits))
	{
		char message[256];
		message[0] = 0;
		csprintf_256(message, "%u exceeds max value of %u", value, 1 << bits);
	}
	function_195720(stream, value, bits);
}

static inline void stream_write_bit(s_bitstream *stream, bool value)
{
	if ((stream->size_in_bytes << 3) - stream->bit_position >= 1 && value)
	{
		long position = stream->bit_position;
		stream->data[position / 8] |= (1 << (position % 8));
	}
	stream->bit_position++;
}

static inline bool stream_reading_failed(s_bitstream *stream)
{
	bool failed = stream->bit_position > (stream->size_in_bytes << 3);
	if (stream->error)
		failed = true;
	return failed;
}

static inline bool stream_reading_succeeded(s_bitstream *stream)
{
	if (!stream_reading_failed(stream))
		return true;
	return false;
}

static inline bool mask_fits(dword mask, dword count)
{
	return count >= 32 || (mask >> count) == 0;
}

// @retail 0xad2e0
bool __stdcall leave_session_decode(s_bitstream *stream, long unknown, s_session_id_message *message)
{
	function_195820(stream, message->session_id, 0x40);
	bool result = !stream_reading_failed(stream);
	return result;
}

// @retail 0xad320
void __stdcall handoff_encode(s_bitstream *stream, long unknown, s_handoff_message const *message)
{
	function_1955d0(stream, message, 0x40);
	stream_write_bits_checked(stream, message->peer_count, 4);
	function_1955d0(stream, message->address, 0x120);
}

// @retail 0xad390
bool __stdcall handoff_decode(s_bitstream *stream, long unknown, s_handoff_message *message)
{
	function_195820(stream, message->session_id, 0x40);
	message->peer_count = (short)function_1959c0(stream, 4);
	function_195820(stream, message->address, 0x120);
	return stream_reading_succeeded(stream);
}

// @retail 0xad3f0
bool __stdcall session_disband_decode(s_bitstream *stream, long unknown, s_session_id_message *message)
{
	function_195820(stream, message->session_id, 0x40);
	return stream_reading_succeeded(stream);
}

// @retail 0xad430
void __stdcall host_decline_encode(s_bitstream *stream, long unknown, s_host_decline_message const *message)
{
	function_1955d0(stream, message, 0x40);
	stream_write_bit(stream, message->has_reason);
	if (message->has_reason)
	{
		stream_write_bit(stream, message->has_address_flag);
		stream_write_bit(stream, message->has_address);
		if (message->has_address)
			function_1955d0(stream, message->address, 0x120);
	}
}

// @retail 0xad530
bool __stdcall host_decline_decode(s_bitstream *stream, long unknown, s_host_decline_message *message)
{
	function_195820(stream, message->session_id, 0x40);
	message->has_reason = function_1957d0(stream);
	if (message->has_reason)
	{
		message->has_address_flag = function_1957d0(stream);
		message->has_address = function_1957d0(stream);
		if (message->has_address)
			function_195820(stream, message->address, 0x120);
	}
	return stream_reading_succeeded(stream);
}

// @retail 0xad5a0
void __stdcall leave_session_encode(s_bitstream *stream, long unknown, s_session_id_message const *message)
{
	function_1955d0(stream, message, 0x40);
}

// @retail 0xad5c0
void __stdcall election_encode(s_bitstream *stream, long unknown, s_election_message const *message)
{
	function_1955d0(stream, message, 0x40);
	function_1955d0(stream, message->address0, 0x120);
	function_1955d0(stream, message->address1, 0x120);
	function_195720(stream, message->unknown50, 0x20);
	stream_write_bits_checked(stream, message->unknown54, 4);
	function_1955d0(stream, message->address2, 0x20);
	stream_write_bits_checked(stream, message->peer_count, 5);
	for (long index = 0; index < message->peer_count; index++)
		function_1955d0(stream, message->peers[index], 0x30);
	stream_write_bits_checked(stream, message->mask0, 16);
	stream_write_bits_checked(stream, message->mask1, 16);
}

// @retail 0xad710
bool __stdcall election_decode(s_bitstream *stream, long unknown, s_election_message *message)
{
	bool valid = true;
	function_195820(stream, message->session_id, 0x40);
	function_195820(stream, message->address0, 0x120);
	function_195820(stream, message->address1, 0x120);
	message->unknown50 = function_1959c0(stream, 0x20);
	message->unknown54 = function_1959c0(stream, 4);
	function_195820(stream, message->address2, 0x20);
	message->peer_count = function_1959c0(stream, 5);
	if (message->peer_count >= 0 && (dword)message->peer_count <= 0x10)
	{
		for (long index = 0; index < message->peer_count; index++)
			function_195820(stream, message->peers[index], 0x30);
	}
	else
		valid = false;
	message->mask0 = function_1959c0(stream, 0x10);
	message->mask1 = function_1959c0(stream, 0x10);
	if (valid && stream_reading_succeeded(stream))
	{
		if (mask_fits(message->mask0, message->peer_count) && mask_fits(message->mask1, message->peer_count))
			return true;
	}
	return false;
}

// @retail 0xad820
void __stdcall election_refuse_encode(s_bitstream *stream, long unknown, s_election_refuse_message const *message)
{
	function_1955d0(stream, message, 0x40);
	stream_write_bits_checked(stream, message->reason, 4);
	stream_write_bit(stream, message->has_address);
	if (message->has_address)
		function_1955d0(stream, message->address, 0x120);
}

// @retail 0xad8d0
bool __stdcall election_refuse_decode(s_bitstream *stream, long unknown, s_election_refuse_message *message)
{
	function_195820(stream, message->session_id, 0x40);
	message->reason = function_1959c0(stream, 4);
	message->has_address = function_1957d0(stream);
	if (message->has_address)
		function_195820(stream, message->address, 0x120);
	if (!stream_reading_failed(stream) && message->reason > 0 && message->reason < 11)
		return true;
	return false;
}

// @retail 0xad940
void __stdcall time_synchronize_encode(s_bitstream *stream, long unknown, s_time_synchronize_message const *message)
{
	function_1955d0(stream, message, 0x40);
	stream_write_bits_checked(stream, message->type, 1);
	dword time;
	switch (message->type)
	{
	case 0:
		time = GetTickCount();
		break;
	case 1:
		time = function_758c0((void *)message);
		function_195720(stream, message->unknown08, 0x20);
		function_195720(stream, message->unknown10, 0x20);
		break;
	default:
		__assume(0);
	}
	function_195720(stream, time, 0x20);
}

// @retail 0xad9e0
bool __stdcall time_synchronize_decode(s_bitstream *stream, long unknown, s_time_synchronize_message *message)
{
	function_195820(stream, message->session_id, 0x40);
	message->type = (word)function_1959c0(stream, 1);
	if (!stream_reading_failed(stream) && message->type < 2)
	{
		bool result = true;
		switch (message->type)
		{
		case 0:
			message->unknown08 = function_1959c0(stream, 0x20);
			message->unknown0c = NONE;
			message->unknown10 = function_758c0(message);
			message->unknown14 = NONE;
			break;
		case 1:
			message->unknown08 = function_1959c0(stream, 0x20);
			message->unknown0c = GetTickCount();
			message->unknown10 = function_1959c0(stream, 0x20);
			message->unknown14 = function_1959c0(stream, 0x20);
			break;
		default:
			__assume(0);
		}
		return result;
	}
	return false;
}

// @retail 0xada90
bool __stdcall time_synchronize_clear(s_bitstream *stream, long unknown, s_time_synchronize_message *message)
{
	message->unknown08 = NONE;
	message->unknown0c = NONE;
	message->unknown10 = NONE;
	message->unknown14 = NONE;
	return true;
}

SESSION_ID_ENCODE(leave_acknowledge)
SESSION_ID_ENCODE(session_disband)
SESSION_ID_ENCODE(session_boot)
SESSION_ID_ENCODE(host_transition)
SESSION_ID_ENCODE(host_reestablish)
SESSION_ID_ENCODE(peer_reestablish)
SESSION_ID_ENCODE(peer_establish)

LEAVE_DECODE(leave_acknowledge)

SESSION_CONTROL_DECODE(session_boot)
SESSION_CONTROL_DECODE(host_transition)
SESSION_CONTROL_DECODE(host_reestablish)
SESSION_CONTROL_DECODE(peer_reestablish)
SESSION_CONTROL_DECODE(peer_establish)

static inline void message_type_register(c_network_message_type_collection *collection, long type, char const *name, long size, t_message_encode encode, t_message_decode decode, t_message_clear clear)
{
	s_message_type *definition = &collection->types[type];
	definition->name = name;
	definition->unknown08 = 0;
	definition->minimum_size = size;
	definition->maximum_size = size;
	definition->encode = encode;
	definition->decode = decode;
	definition->clear = clear;
	definition->initialized = true;
}

#define REGISTER(type, name, size, encode, decode, clear) \
	message_type_register(collection, type, name, size, (t_message_encode)encode, (t_message_decode)decode, (t_message_clear)clear)

// @retail 0xadab0
void network_message_types_register_session_protocol(c_network_message_type_collection *collection)
{
	REGISTER(0, "join-request", 0x1b8, function_acc20, function_acfa0, 0);
	REGISTER(1, "join-abort", 0x10, function_ad1c0, function_ad1e0, 0);
	REGISTER(2, "join-refuse", 0xc, function_ad230, function_ad290, 0);
	REGISTER(3, "leave-session", 8, leave_session_encode, leave_session_decode, 0);
	REGISTER(4, "leave-acknowledge", 8, leave_acknowledge_encode, leave_acknowledge_decode, 0);
	REGISTER(5, "session-disband", 8, session_disband_encode, session_disband_decode, 0);
	REGISTER(6, "session-boot", 8, session_boot_encode, session_boot_decode, 0);
	REGISTER(7, "host-handoff", 0x2e, handoff_encode, handoff_decode, 0);
	REGISTER(8, "peer-handoff", 0x2e, handoff_encode, handoff_decode, 0);
	REGISTER(9, "host-transition", 8, host_transition_encode, host_transition_decode, 0);
	REGISTER(10, "host-reestablish", 8, host_reestablish_encode, host_reestablish_decode, 0);
	REGISTER(11, "host-decline", 0x30, host_decline_encode, host_decline_decode, 0);
	REGISTER(12, "peer-reestablish", 8, peer_reestablish_encode, peer_reestablish_decode, 0);
	REGISTER(13, "peer-establish", 8, peer_establish_encode, peer_establish_decode, 0);
	REGISTER(14, "election", 0xc8, election_encode, election_decode, 0);
	REGISTER(15, "election-refuse", 0x34, election_refuse_encode, election_refuse_decode, 0);
	REGISTER(16, "time-synchronize", 0x1c, time_synchronize_encode, time_synchronize_decode, time_synchronize_clear);
}