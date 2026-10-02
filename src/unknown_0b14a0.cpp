// @flags /O2 /Gr
/* UNKNOWN_0B14A0.CPP: network message codecs: the parameters-request,
   countdown-timer and mode-acknowledge messages. Each codec reads or writes
   the message through the bit stream (unknown_195720.cpp); a decoder returns
   whether the message it read is valid. The codecs are callbacks, so they are
   stdcall */

#include "cseries.h"
#include "bitstream.h"
#include "unknown_1946f0.h"
#include "unknown_0b14a0.h"

/* writes one bit; retail inlines it */
static __inline void stream_write_bit(s_bitstream *stream, bool value)
{
	if ((stream->size_in_bytes << 3) - stream->bit_position >= 1 && value)
		stream->data[stream->bit_position / 8] |= (byte)(1 << (stream->bit_position % 8));
	stream->bit_position++;
}

/* the checked write (1947e0) retail inlines */
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

/* function_1946f0, which retail inlines */
static __inline bool stream_overflowed(s_bitstream *stream)
{
	bool result = stream->bit_position > (stream->size_in_bytes << 3);
	if (stream->error)
		result = true;
	return result;
}

/* a zero terminated string of at most count bytes, one byte at a time; retail
   inlines the read */
static __inline void read_byte_string(s_bitstream *stream, byte *buffer, long count)
{
	long i;
	for (i = 0; i < count; i++)
	{
		buffer[i] = (byte)function_1959c0(stream, 8);
		if (buffer[i] == 0)
			break;
	}
	if (i >= count)
	{
		buffer[count - 1] = 0;
		stream->error = true;
	}
}

/* every message starts with a 64 bit header */
struct s_message_header
{
	byte data[8];
};

/* the parameters-request message, 0x59c bytes: a run of optional fields,
   each preceded by a flag */
struct s_message_parameters_request
{
	s_message_header header;
	bool has_value0;
	long value0;
	bool has_value1;
	long value1;
	bool has_value2;
	long value2;
	bool has_value3;
	long value3;
	bool has_flag4;
	bool flag4;
	bool has_value5;
	long value5;
	bool has_flag6;
	bool flag6;
	bool has_value7;
	long value7;
	bool has_part8;
	bool part8_valid;
	dword part8[0xc2];
	bool has_name;
	dword name_value0;
	dword name_value1;
	byte name[0x80];
	bool has_value10;
	long value10;
	bool has_part11;
	dword part11[0x4c];
	bool has_words;
	word words[0x20];
	bool has_flag13;
	bool flag13;
	bool has_value14;
	short value14;
	bool has_part15;
	bool part15_valid;
	dword part15[0x11];
};

// @retail 0xb14a0
void __stdcall function_b14a0(s_bitstream *stream, long size, void *message_)
{
	s_message_parameters_request *message = (s_message_parameters_request *)message_;

	function_1955d0(stream, message, 0x40);
	stream_write_bit(stream, message->has_value0);
	if (message->has_value0)
		stream_write_checked(stream, message->value0, 5);
	stream_write_bit(stream, message->has_value1);
	if (message->has_value1)
		stream_write_checked(stream, message->value1, 4);
	stream_write_bit(stream, message->has_value2);
	if (message->has_value2)
		stream_write_checked(stream, message->value2, 2);
	stream_write_bit(stream, message->has_value3);
	if (message->has_value3)
		stream_write_checked(stream, message->value3, 2);
	stream_write_bit(stream, message->has_flag4);
	if (message->has_flag4)
		stream_write_bit(stream, message->flag4);
	stream_write_bit(stream, message->has_value5);
	if (message->has_value5)
		stream_write_checked(stream, message->value5 + 1, 5);
	stream_write_bit(stream, message->has_flag6);
	if (message->has_flag6)
		stream_write_bit(stream, message->flag6);
	stream_write_bit(stream, message->has_value7);
	if (message->has_value7)
		stream_write_checked(stream, message->value7, 5);
	stream_write_bit(stream, message->has_part8);
	if (message->has_part8)
	{
		stream_write_bit(stream, message->part8_valid);
		if (message->part8_valid)
			function_063690(message->part8, stream);
	}
	stream_write_bit(stream, message->has_name);
	if (message->has_name)
	{
		function_195720(stream, message->name_value0, 32);
		function_195720(stream, message->name_value1, 32);
		for (long i = 0; i < 0x80; i++)
		{
			byte c = message->name[i];
			function_195720(stream, c, 8);
			if (c == 0)
				break;
		}
	}
	stream_write_bit(stream, message->has_value10);
	if (message->has_value10)
		stream_write_checked(stream, message->value10, 2);
	stream_write_bit(stream, message->has_part11);
	if (message->has_part11)
		function_07cc50(stream, message->part11);
	stream_write_bit(stream, message->has_words);
	if (message->has_words)
	{
		for (long i = 0; i < 0x20; i++)
		{
			word w = message->words[i];
			function_195720(stream, w, 16);
			if (w == 0)
				break;
		}
	}
	stream_write_bit(stream, message->has_flag13);
	if (message->has_flag13)
		stream_write_bit(stream, message->flag13);
	stream_write_bit(stream, message->has_value14);
	if (message->has_value14)
		stream_write_checked(stream, message->value14 + 1, 3);
	stream_write_bit(stream, message->has_part15);
	if (message->has_part15)
	{
		stream_write_bit(stream, message->part15_valid);
		if (message->part15_valid)
			function_b2330((s_parameters_part *)message->part15, stream);
	}
}

// @retail 0xb1c80
bool __stdcall function_b1c80(s_bitstream *stream, long size, void *message_)
{
	s_message_parameters_request *message = (s_message_parameters_request *)message_;
	bool valid = true;

	function_195820(stream, message, 0x40);
	message->has_value0 = function_1957d0(stream);
	if (message->has_value0)
		message->value0 = function_1959c0(stream, 5);
	message->has_value1 = function_1957d0(stream);
	if (message->has_value1)
		message->value1 = function_1959c0(stream, 4);
	message->has_value2 = function_1957d0(stream);
	if (message->has_value2)
		message->value2 = function_1959c0(stream, 2);
	message->has_value3 = function_1957d0(stream);
	if (message->has_value3)
		message->value3 = function_1959c0(stream, 2);
	message->has_flag4 = function_1957d0(stream);
	if (message->has_flag4)
		message->flag4 = function_1957d0(stream);
	message->has_value5 = function_1957d0(stream);
	if (message->has_value5)
		message->value5 = function_1959c0(stream, 5) - 1;
	message->has_flag6 = function_1957d0(stream);
	if (message->has_flag6)
		message->flag6 = function_1957d0(stream);
	message->has_value7 = function_1957d0(stream);
	if (message->has_value7)
		message->value7 = function_1959c0(stream, 5);
	message->has_part8 = function_1957d0(stream);
	if (message->has_part8)
	{
		message->part8_valid = function_1957d0(stream);
		if (message->part8_valid)
			valid = function_063980(stream, message->part8);
	}
	message->has_name = function_1957d0(stream);
	if (message->has_name)
	{
		message->name_value0 = function_1959c0(stream, 32);
		message->name_value1 = function_1959c0(stream, 32);
		read_byte_string(stream, message->name, 0x80);
	}
	message->has_value10 = function_1957d0(stream);
	if (message->has_value10)
		message->value10 = function_1959c0(stream, 2);
	message->has_part11 = function_1957d0(stream);
	if (message->has_part11)
		valid = valid && function_07d520(stream, message->part11);
	message->has_words = function_1957d0(stream);
	if (message->has_words)
		function_194fa0(stream, message->words, 0x20);
	message->has_flag13 = function_1957d0(stream);
	if (message->has_flag13)
		message->flag13 = function_1957d0(stream);
	message->has_value14 = function_1957d0(stream);
	if (message->has_value14)
	{
		message->value14 = (short)(function_1959c0(stream, 3) - 1);
		valid = valid && (message->value14 == -1 || (message->value14 >= 0 && message->value14 < 4));
	}
	message->has_part15 = function_1957d0(stream);
	if (message->has_part15)
	{
		message->part15_valid = function_1957d0(stream);
		if (message->part15_valid)
			valid = valid && function_b23d0(stream, (s_parameters_part *)message->part15);
	}
	bool result = valid && !stream_overflowed(stream);
	if (message->has_value0)
		result = result && message->value0 > 0 && message->value0 < 0x13;
	if (message->has_value1)
		result = result && message->value1 >= 0 && message->value1 < 9;
	if (message->has_value2)
		result = result && message->value2 >= 0 && message->value2 < 3;
	if (message->has_value3)
		result = result && message->value3 >= 0 && message->value3 < 3;
	if (message->has_value5)
		result = result && (message->value5 == NONE || (message->value5 >= 0 && message->value5 < 0x10));
	if (message->has_value7)
		result = result && message->value7 >= 0 && message->value7 < 0x14;
	if (message->has_value10)
		result = result && message->value10 >= 0 && message->value10 < 3;
	return result;
}

/* the countdown-timer message, 0x20 bytes */
struct s_message_countdown_timer
{
	s_message_header header;
	bool flag;
	long value;
	long kind;
	byte data[0xc];
};

// @retail 0xb1fd0
void __stdcall function_b1fd0(s_bitstream *stream, long size, void *message_)
{
	s_message_countdown_timer *message = (s_message_countdown_timer *)message_;

	function_1955d0(stream, message, 0x40);
	stream_write_bit(stream, message->flag);
	stream_write_checked(stream, message->value, 12);
	stream_write_checked(stream, message->kind, 2);
	if (message->kind == 1)
		function_1955d0(stream, message->data, 0x60);
}

// @retail 0xb20c0
bool __stdcall function_b20c0(s_bitstream *stream, long size, void *message_)
{
	s_message_countdown_timer *message = (s_message_countdown_timer *)message_;

	function_195820(stream, message, 0x40);
	message->flag = function_1957d0(stream);
	message->value = function_1959c0(stream, 12);
	message->kind = function_1959c0(stream, 2);
	if (message->kind == 1)
		function_195820(stream, message->data, 0x60);
	if (!stream_overflowed(stream) && message->value >= 0 && message->kind >= 0 && message->kind < 3)
		return true;
	return false;
}

/* the mode-acknowledge message, 0x10 bytes */
struct s_message_mode_acknowledge
{
	s_message_header header;
	long mode;
	long value;
};

// @retail 0xb2140
void __stdcall function_b2140(s_bitstream *stream, long size, void *message_)
{
	s_message_mode_acknowledge *message = (s_message_mode_acknowledge *)message_;

	function_1955d0(stream, message, 0x40);
	stream_write_checked(stream, message->mode, 5);
	function_195720(stream, message->value, 32);
}

// @retail 0xb21b0
bool __stdcall function_b21b0(s_bitstream *stream, long size, void *message_)
{
	s_message_mode_acknowledge *message = (s_message_mode_acknowledge *)message_;

	function_195820(stream, message, 0x40);
	message->mode = function_1959c0(stream, 5);
	message->value = function_1959c0(stream, 32);
	if (!stream_overflowed(stream) && message->mode > 0 && message->mode < 0x13 && message->value >= 0)
		return true;
	return false;
}
