/* BITSTREAM.H: the bit stream shared by the entity definitions (09a9f0,
   09fe30) and the 0x450c94 classes (096e90, 0984d0), and by the network
   message codecs: 195720 writes into it and 1959c0 reads from it. The
   module itself is src/unknown_1946f0.cpp */

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
	long unknown08;
	long mode;                 /* 1 is writing, 3 is reading */
	long bit_position;
	bool error;
	byte unknown15[3];
	long checkpoint_count;
	long checkpoints[4];       /* bit positions saved by the push, popped at 194710 */
	long unknown2c;
	long unknown30;
};

void function_195720(s_bitstream *stream, dword value, long count);
dword function_1959c0(s_bitstream *stream, long count);
bool function_1957d0(s_bitstream *stream);
void function_1955d0(s_bitstream *stream, void const *source, long bits);
void function_195820(s_bitstream *stream, void *destination, long bits);

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
