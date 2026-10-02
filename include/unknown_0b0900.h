/* UNKNOWN_0B0900.H: callees of the network message decoder in
   src/unknown_0b0900.cpp that are not decompiled yet (stubbed in
   src/stubs/unknown_0b0900.cpp) */

#ifndef UNKNOWN_0B0900_H
#define UNKNOWN_0B0900_H

#include "cseries.h"
#include "bitstream.h"

/* 0x63980: reads one sub-structure from the stream, true when it is valid */
byte function_063980(s_bitstream *stream, void *destination);

/* 0x7d520 */
bool function_07d520(s_bitstream *stream, void *destination);

/* 0xb23d0 */
bool function_0b23d0(s_bitstream *stream, void *destination);

#endif
