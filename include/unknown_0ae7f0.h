/* UNKNOWN_0AE7F0.H: the network message codecs in src/unknown_0ae7f0.cpp and
   the callees of theirs that are not decompiled yet (stubbed in
   src/stubs/unknown_0ae7f0.cpp). Retail's registration function (0xaf680)
   stores the codecs' addresses in the message type table, which is why they
   are stdcall: three stack arguments, the stream, an unused size and the
   message */

#ifndef UNKNOWN_0AE7F0_H
#define UNKNOWN_0AE7F0_H

#include "cseries.h"
#include "bitstream.h"

struct s_message_0ae7f0;
struct s_message_0aedb0;
struct s_message_0af1f0;
struct s_message_0af270;
struct s_message_0af3c0;
struct s_message_0af490;
struct s_message_0af540;

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

/* 0x7ca70: reads one sub-structure from the stream, true when it is valid */
bool function_07ca70(s_bitstream *stream, void *destination);

/* 0x7c5a0: writes the same sub-structure */
void function_07c5a0(s_bitstream *stream, void const *source);

#endif