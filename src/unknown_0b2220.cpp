// @flags /O2 /Gr
/* UNKNOWN_0B2220.CPP: network message codecs: the parameters, view,
   player, synchronous, results and test message types, and the functions
   that register them in the message type table. Each codec reads or writes
   the message through the bit stream (unknown_195720.cpp) */

#include "cseries.h"
#include "bitstream.h"
#include "unknown_1946f0.h"

typedef void (__stdcall *t_message_encode)(s_bitstream *stream, long size, void *message);
typedef bool (__stdcall *t_message_decode)(s_bitstream *stream, long size, void *message);
typedef bool (__stdcall *t_message_compare)(long size, void *a, void *b);

/* one registered message type: 0x20 bytes */
struct s_message_type
{
	bool registered;
	byte unknown01[3];
	char const *name;
	long unknown08;
	long minimum_size;
	long maximum_size;
	t_message_encode encode;
	t_message_decode decode;
	t_message_compare compare;
};

struct s_message_type_table
{
	s_message_type types[45];
};

struct s_view_establishment_message
{
	long mode;
	long value;
};

struct s_player_acknowledge_message
{
	dword mask;
	dword acknowledged;
	byte entries[16][12];
};

struct s_player_action
{
	byte unknown00[0x5c];
};

struct s_synchronous_actions_message
{
	long unknown00;
	long unknown04;
	bool flag;
	byte unknown09[3];
	dword mask;
	s_player_action actions[4];
};

struct s_synchronous_join_message
{
	long unknown00;
};

struct s_synchronous_gamestate_message
{
	long unknown00;
	long size;
	byte data[1];
};

struct s_synchronous_update_message
{
	byte unknown00[0x4048];
};

struct s_game_results_message
{
	dword unknown00;
	dword unknown04;
	byte results[0x4fb0];
};

struct s_test_message
{
	bool flag;
	byte unknown01[3];
	long value;
};

struct s_parameters_part
{
	long unknown00;
	byte unknown04[8];
	byte unknown0c[16];
	byte unknown1c[36];
	long unknown40;
};

/* codecs of other source files, registered below */
void __stdcall function_af890(s_bitstream *stream, long size, void *message);
bool __stdcall function_b0900(s_bitstream *stream, long size, void *message);
void __stdcall function_b14a0(s_bitstream *stream, long size, void *message);
bool __stdcall function_b1c80(s_bitstream *stream, long size, void *message);
void __stdcall function_b1fd0(s_bitstream *stream, long size, void *message);
bool __stdcall function_b20c0(s_bitstream *stream, long size, void *message);
void __stdcall function_b2140(s_bitstream *stream, long size, void *message);
bool __stdcall function_b21b0(s_bitstream *stream, long size, void *message);

/* the synchronous message helpers, 0x86f90 and following */
void function_86f90(s_bitstream *stream, s_player_action *action);
bool function_874c0(s_bitstream *stream, s_player_action *action);
bool function_87830(s_player_action *a, s_player_action *b);
void function_87d00(s_bitstream *stream, void *message);
bool function_87e90(s_bitstream *stream, void *message);
bool function_88060(void *a, void *b);
void function_197680(s_bitstream *stream, void *results);
byte function_197d80(s_bitstream *stream, void *results);

static __inline void stream_write_bool(s_bitstream *stream, bool value)
{
	if (stream->size_in_bytes * 8 - stream->bit_position >= 1 && value)
		stream->data[stream->bit_position / 8] |= (byte)(1 << (stream->bit_position % 8));
	stream->bit_position++;
}

static __forceinline bool stream_overflowed(s_bitstream *stream)
{
	bool result = stream->bit_position > (stream->size_in_bytes << 3);
	if (stream->error)
		result = true;
	return result;
}

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

// @retail 0xb2220
void messages_register_parameters(s_message_type_table *table)
{
	s_message_type *type;

	type = &table->types[33];
	type->name = "parameters-update";
	type->unknown08 = 0;
	type->minimum_size = 0x14d8;
	type->maximum_size = 0x14d8;
	type->encode = (t_message_encode)function_af890;
	type->decode = (t_message_decode)function_b0900;
	type->compare = 0;
	type->registered = true;

	type = &table->types[34];
	type->name = "parameters-request";
	type->unknown08 = 0;
	type->minimum_size = 0x59c;
	type->maximum_size = 0x59c;
	type->encode = (t_message_encode)function_b14a0;
	type->decode = (t_message_decode)function_b1c80;
	type->compare = 0;
	type->registered = true;

	type = &table->types[35];
	type->name = "countdown-timer";
	type->unknown08 = 0;
	type->minimum_size = 0x20;
	type->maximum_size = 0x20;
	type->encode = (t_message_encode)function_b1fd0;
	type->decode = (t_message_decode)function_b20c0;
	type->compare = 0;
	type->registered = true;

	type = &table->types[36];
	type->name = "mode-acknowledge";
	type->unknown08 = 0;
	type->minimum_size = 0x10;
	type->maximum_size = 0x10;
	type->encode = (t_message_encode)function_b2140;
	type->decode = (t_message_decode)function_b21b0;
	type->compare = 0;
	type->registered = true;
}

// @retail 0xb2330
void function_b2330(s_parameters_part *message, s_bitstream *stream)
{
	stream_write_checked(stream, message->unknown00, 1);
	function_1955d0(stream, message->unknown04, 0x40);
	function_1955d0(stream, message->unknown0c, 0x80);
	function_1955d0(stream, message->unknown1c, 0x120);
	stream_write_checked(stream, message->unknown40, 2);
}

// @retail 0xb23d0
bool function_b23d0(s_bitstream *stream, s_parameters_part *message)
{
	message->unknown00 = function_1959c0(stream, 1);
	function_195820(stream, message->unknown04, 0x40);
	function_195820(stream, message->unknown0c, 0x80);
	function_195820(stream, message->unknown1c, 0x120);
	message->unknown40 = function_1959c0(stream, 2);
	if (message->unknown00 >= 0 && message->unknown00 < 2 && message->unknown40 >= 0 && message->unknown40 < 3)
		return true;
	return false;
}

// @retail 0xb2440
void __stdcall function_b2440(s_bitstream *stream, long size, s_view_establishment_message *message)
{
	stream_write_checked(stream, message->mode, 3);
	stream_write_bool(stream, message->value != NONE);
	if (message->value != NONE)
		function_195720(stream, message->value, 32);
}

// @retail 0xb24e0
bool __stdcall function_b24e0(s_bitstream *stream, long size, s_view_establishment_message *message)
{
	message->mode = function_1959c0(stream, 3);
	if (function_1957d0(stream))
		message->value = function_1959c0(stream, 32);
	else
		message->value = NONE;
	if (!stream_overflowed(stream) && message->mode >= 0 && message->mode < 6 && (message->value == NONE || message->value >= 0))
		return true;
	return false;
}

// @retail 0xb2550
void __stdcall function_b2550(s_bitstream *stream, long size, s_player_acknowledge_message *message)
{
	stream_write_checked(stream, message->mask, 16);
	stream_write_checked(stream, message->acknowledged, 16);
	for (long i = 0; i < 16; i++)
	{
		if (message->mask & (1 << i))
			function_1955d0(stream, message->entries[i], 0x60);
	}
}

// @retail 0xb2600
bool __stdcall function_b2600(s_bitstream *stream, long size, s_player_acknowledge_message *message)
{
	message->mask = function_1959c0(stream, 16);
	message->acknowledged = function_1959c0(stream, 16);
	for (long i = 0; i < 16; i++)
	{
		if (message->mask & (1 << i))
			function_195820(stream, message->entries[i], 0x60);
	}
	if (!stream_overflowed(stream) && (message->acknowledged & ~message->mask) == 0)
		return true;
	return false;
}

// @retail 0xb2680
void messages_register_simulation(s_message_type_table *table)
{
	s_message_type *type;

	type = &table->types[37];
	type->name = "view-establishment";
	type->unknown08 = 0;
	type->minimum_size = 8;
	type->maximum_size = 8;
	type->encode = (t_message_encode)function_b2440;
	type->decode = (t_message_decode)function_b24e0;
	type->compare = 0;
	type->registered = true;

	type = &table->types[38];
	type->name = "player-acknowledge";
	type->unknown08 = 0;
	type->minimum_size = 0xc8;
	type->maximum_size = 0xc8;
	type->encode = (t_message_encode)function_b2550;
	type->decode = (t_message_decode)function_b2600;
	type->compare = 0;
	type->registered = true;
}

// @retail 0xb2710
void __stdcall function_b2710(s_bitstream *stream, long size, s_synchronous_update_message *message)
{
	function_87d00(stream, message);
}

// @retail 0xb2730
bool __stdcall function_b2730(s_bitstream *stream, long size, s_synchronous_update_message *message)
{
	bool result = function_87e90(stream, message);
	if (!stream_overflowed(stream) && result)
		return true;
	return false;
}

// @retail 0xb2770
bool __stdcall function_b2770(long size, s_synchronous_update_message *a, s_synchronous_update_message *b)
{
	return function_88060(a, b);
}

// @retail 0xb2790
void __stdcall function_b2790(s_bitstream *stream, long size, s_synchronous_actions_message *message)
{
	function_195720(stream, message->unknown00, 32);
	function_195720(stream, message->unknown04 + 1, 32);
	stream_write_bool(stream, message->flag);
	stream_write_checked(stream, message->mask, 4);
	for (long i = 0; i < 4; i++)
	{
		if (message->mask & (1 << i))
			function_86f90(stream, &message->actions[i]);
	}
}

// @retail 0xb2860
bool __stdcall function_b2860(s_bitstream *stream, long size, s_synchronous_actions_message *message)
{
	bool result = true;
	message->unknown00 = function_1959c0(stream, 32);
	message->unknown04 = function_1959c0(stream, 32) - 1;
	message->flag = function_1957d0(stream);
	message->mask = function_1959c0(stream, 4);
	for (long i = 0; i < 4; i++)
	{
		if (message->mask & (1 << i))
			result = result && function_874c0(stream, &message->actions[i]);
	}
	if (result && !stream_overflowed(stream) && message->unknown00 >= 0 && message->unknown04 >= NONE)
		return true;
	return false;
}

// @retail 0xb2920
bool __stdcall function_b2920(long size, s_synchronous_actions_message *a, s_synchronous_actions_message *b)
{
	bool result = true;
	if (a->mask != b->mask)
		return false;
	for (long i = 0; i < 4; i++)
	{
		if (a->mask & (1 << i))
			result = result && function_87830(&a->actions[i], &b->actions[i]);
	}
	return result;
}

// @retail 0xb2980
void __stdcall function_b2980(s_bitstream *stream, long size, s_synchronous_join_message *message)
{
	function_195720(stream, message->unknown00, 32);
}

// @retail 0xb29a0
bool __stdcall function_b29a0(s_bitstream *stream, long size, s_synchronous_join_message *message)
{
	message->unknown00 = function_1959c0(stream, 32);
	if (!stream_overflowed(stream) && message->unknown00 >= 0)
		return true;
	return false;
}

// @retail 0xb29e0
void __stdcall function_b29e0(s_bitstream *stream, long size, s_synchronous_gamestate_message *message)
{
	byte *data = message->data;
	stream_write_checked(stream, message->unknown00, 23);
	stream_write_checked(stream, message->size, 13);
	if (message->size > 0)
		function_1955d0(stream, data, message->size * 8);
}

// @retail 0xb2a90
bool __stdcall function_b2a90(s_bitstream *stream, long size, s_synchronous_gamestate_message *message)
{
	byte *data = message->data;
	message->unknown00 = function_1959c0(stream, 23);
	message->size = function_1959c0(stream, 13);
	if (stream_overflowed(stream) || message->unknown00 < 0 || message->size < 0 || message->size > 0xffff || size != message->size + 8)
		return false;
	bool valid = true;
	if (message->size > 0)
	{
		function_195820(stream, data, message->size * 8);
		if (stream_overflowed(stream))
			return false;
		return true;
	}
	return valid;
}

// @retail 0xb2b30
void messages_register_synchronous(s_message_type_table *table)
{
	s_message_type *type;

	type = &table->types[39];
	type->name = "synchronous-update";
	type->unknown08 = 0;
	type->minimum_size = 0x4048;
	type->maximum_size = 0x4048;
	type->encode = (t_message_encode)function_b2710;
	type->decode = (t_message_decode)function_b2730;
	type->compare = (t_message_compare)function_b2770;
	type->registered = true;

	type = &table->types[40];
	type->name = "synchronous-actions";
	type->unknown08 = 0;
	type->minimum_size = 0x180;
	type->maximum_size = 0x180;
	type->encode = (t_message_encode)function_b2790;
	type->decode = (t_message_decode)function_b2860;
	type->compare = (t_message_compare)function_b2920;
	type->registered = true;

	type = &table->types[41];
	type->name = "synchronous-join";
	type->unknown08 = 0;
	type->minimum_size = 4;
	type->maximum_size = 4;
	type->encode = (t_message_encode)function_b2980;
	type->decode = (t_message_decode)function_b29a0;
	type->compare = 0;
	type->registered = true;

	type = &table->types[42];
	type->name = "synchronous-gamestate";
	type->unknown08 = 1;
	type->minimum_size = 8;
	type->maximum_size = 0xffff;
	type->encode = (t_message_encode)function_b29e0;
	type->decode = (t_message_decode)function_b2a90;
	type->compare = 0;
	type->registered = true;
}

// @retail 0xb2c50
void __stdcall function_b2c50(s_bitstream *stream, long size, s_game_results_message *message)
{
	function_1955d0(stream, &message->unknown00, 32);
	function_1955d0(stream, &message->unknown04, 32);
	function_197680(stream, message->results);
}

// @retail 0xb2c80
bool __stdcall function_b2c80(s_bitstream *stream, long size, s_game_results_message *message)
{
	function_195820(stream, &message->unknown00, 32);
	function_195820(stream, &message->unknown04, 32);
	if (function_197d80(stream, message->results))
		return true;
	return false;
}

// @retail 0xb2cc0
void messages_register_results(s_message_type_table *table)
{
	s_message_type *type = &table->types[43];
	type->name = "game-results";
	type->unknown08 = 0;
	type->minimum_size = 0x4fb8;
	type->maximum_size = 0x4fb8;
	type->encode = (t_message_encode)function_b2c50;
	type->decode = (t_message_decode)function_b2c80;
	type->compare = 0;
	type->registered = true;
}

// @retail 0xb2d10
void __stdcall function_b2d10(s_bitstream *stream, long size, s_test_message *message)
{
	stream_write_bool(stream, message->flag);
	stream_write_checked(stream, message->value, 16);
}

// @retail 0xb2da0
bool __stdcall function_b2da0(s_bitstream *stream, long size, s_test_message *message)
{
	message->flag = function_1957d0(stream);
	message->value = function_1959c0(stream, 16);
	bool ok = !stream_overflowed(stream);
	return ok;
}

// @retail 0xb2de0
void messages_register_test(s_message_type_table *table)
{
	s_message_type *type = &table->types[44];
	type->name = "test";
	type->unknown08 = 0;
	type->minimum_size = 8;
	type->maximum_size = 8;
	type->encode = (t_message_encode)function_b2d10;
	type->decode = (t_message_decode)function_b2da0;
	type->compare = 0;
	type->registered = true;
}
