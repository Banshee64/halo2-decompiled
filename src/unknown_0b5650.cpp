// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0B5650.CPP: writes a simulation entity identifier (10 bits of
   index, 4 bits of salt) to a bitstream */

#include "cseries.h"
#include "bitstream.h"

// @retail 0xb5650
void function_b5650(long identifier, s_bitstream *stream)
{
	dword salt = identifier;
	long index = salt & 0x3ff;
	salt >>= 28;
	stream_write_checked(stream, index, 10);
	stream_write_checked(stream, (byte)salt, 4);
}
