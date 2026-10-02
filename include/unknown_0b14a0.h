/* UNKNOWN_0B14A0.H: the parameters-request, countdown-timer and
   mode-acknowledge network message codecs in src/unknown_0b14a0.cpp, and the
   callees of theirs that are not decompiled yet (stubbed in
   src/stubs/unknown_0b14a0.cpp). Retail's registration function (0xb2220)
   stores the codecs' addresses in the message type table, which is why they
   are stdcall: three stack arguments, the stream, an unused size and the
   message */

#ifndef UNKNOWN_0B14A0_H
#define UNKNOWN_0B14A0_H

#include "cseries.h"
#include "bitstream.h"

struct s_parameters_part;

void __stdcall function_b14a0(s_bitstream *stream, long size, void *message);
bool __stdcall function_b1c80(s_bitstream *stream, long size, void *message);
void __stdcall function_b1fd0(s_bitstream *stream, long size, void *message);
bool __stdcall function_b20c0(s_bitstream *stream, long size, void *message);
void __stdcall function_b2140(s_bitstream *stream, long size, void *message);
bool __stdcall function_b21b0(s_bitstream *stream, long size, void *message);

/* 0xb2330 and 0xb23d0 (src/unknown_0b2220.cpp): write and read the part of
   the parameters message at offset 0x558 */
void function_b2330(s_parameters_part *message, s_bitstream *stream);
bool function_b23d0(s_bitstream *stream, s_parameters_part *message);

/* 0x63690 and 0x63980: write and read the sub-structure at offset 0x3c */
void __stdcall function_063690(void *part, s_bitstream *stream);
byte function_063980(s_bitstream *stream, void *part);

/* 0x7cc50 and 0x7d520: write and read the sub-structure at offset 0x3dc */
void __stdcall function_07cc50(s_bitstream *stream, void *part);
bool function_07d520(s_bitstream *stream, void *part);

#endif
