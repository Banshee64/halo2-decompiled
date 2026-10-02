/* BITSTREAM.H: the bit stream shared by the entity definitions (09a9f0,
   09fe30) and the 0x450c94 classes (096e90, 0984d0): 195720 writes into it
   and 1959c0 reads from it */

#ifndef BITSTREAM_H
#define BITSTREAM_H

#include "cseries.h"

/* a bitstream: the data, its size in bytes, and the current bit position */
struct s_bitstream
{
	union
	{
		byte *data;
		dword *buffer;
	};
	long size_in_bytes;
	byte unknown08[8];
	long bit_position;
};

void function_195720(s_bitstream *stream, dword value, long count);
dword function_1959c0(s_bitstream *stream, long count);

/* reads one bit; 0x974c0 and 0x97240 inline it */
inline bool stream_read_bit(s_bitstream *stream)
{
	long position = stream->bit_position;
	bool bit = false;
	if (position <= (stream->size_in_bytes << 3))
		bit = (stream->data[position / 8] & (1 << (position % 8))) != 0;
	stream->bit_position = position + 1;
	return bit;
}

#endif
