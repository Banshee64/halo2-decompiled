#include "cseries.h"
#include "unknown_08b110.h"

// @flags /O2 /Ob1 /arch:SSE /Gr

// @retail 0x195720
void function_195720(s_bitstream *stream, dword value, long count)
{
	long position = stream->bit_position;
	long remaining = (stream->size_in_bytes << 3) - position;
	long n = count;
	if (remaining <= count)
		n = remaining;
	if (n > 0)
	{
		value &= 0xffffffff >> (32 - n);
		long first = position >> 5;
		long last = (position + n - 1) >> 5;
		long offset = position & 0x1f;
		if (first == last)
			stream->buffer[first] |= value << offset;
		else
		{
			stream->buffer[first] |= value << offset;
			stream->buffer[first + 1] = value >> (32 - offset);
		}
	}
	stream->bit_position += count;
}

// @retail 0x1959c0
dword function_1959c0(s_bitstream *stream, long count)
{
	long position = stream->bit_position;
	long remaining = (stream->size_in_bytes << 3) - position;
	dword result = 0;
	long n = remaining <= count ? remaining : count;
	if (n > 0)
	{
		long first = position >> 5;
		long last = (position + n - 1) >> 5;
		long offset = position & 0x1f;
		dword *buffer = stream->buffer;
		if (first == last)
			result = buffer[first] >> offset;
		else
			result = (buffer[first + 1] << (32 - offset)) | (buffer[first] >> offset);
		result &= 0xffffffff >> (32 - n);
	}
	stream->bit_position = position + count;
	return result;
}