/* UNKNOWN_0AF5F0.H: the membership network message codecs (0xadef0 through
   0xaf5f0), the message type table entry the registration function (0xaf680)
   fills in, and the codecs of the neighbouring file src/unknown_0ae7f0.cpp.
   Retail's registration function stores the codecs' addresses in the message
   type table, which is why they are stdcall: three stack arguments, the
   stream, an unused size and the message */

#ifndef UNKNOWN_0AF5F0_H
#define UNKNOWN_0AF5F0_H

#include "cseries.h"
#include "bitstream.h"

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

struct s_message_0adef0;
struct s_message_0ae7f0;
struct s_message_0aedb0;
struct s_message_0af1f0;
struct s_message_0af270;
struct s_message_0af3c0;
struct s_message_0af490;
struct s_message_0af540;

void __stdcall function_0adef0(s_bitstream *stream, long unused, s_message_0adef0 *message);
bool __stdcall function_0ae7f0(s_bitstream *stream, long unused, s_message_0ae7f0 *message);
void __stdcall function_0aedb0(s_bitstream *stream, long unused, s_message_0aedb0 *message);
bool __stdcall function_0af050(s_bitstream *stream, long unused, s_message_0aedb0 *message);
void __stdcall function_0af1f0(s_bitstream *stream, long unused, s_message_0af1f0 *message);
bool __stdcall function_0af220(s_bitstream *stream, long unused, s_message_0af1f0 *message);
void __stdcall function_0af270(s_bitstream *stream, long unused, s_message_0af270 *message);
bool __stdcall function_0af320(s_bitstream *stream, long unused, s_message_0af270 *message);
void __stdcall function_0af3c0(s_bitstream *stream, long unused, s_message_0af3c0 *message);
bool __stdcall function_0af430(s_bitstream *stream, long unused, s_message_0af3c0 *message);
void __stdcall function_0af490(s_bitstream *stream, long unused, s_message_0af490 *message);
bool __stdcall function_0af4f0(s_bitstream *stream, long unused, s_message_0af490 *message);
void __stdcall function_0af540(s_bitstream *stream, long unused, s_message_0af540 *message);
bool __stdcall function_0af5f0(s_bitstream *stream, long unused, s_message_0af540 *message);

/* 0x7ca70: reads one sub-structure from the stream, true when it is valid */
bool function_07ca70(s_bitstream *stream, void *destination);

#endif
