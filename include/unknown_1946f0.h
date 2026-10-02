/* UNKNOWN_1946F0.H: the bit stream module's vector and real codecs, and the
   quantized-direction helpers (0x24f590, 0x24f6b0, 0x24f7b0) they call.
   src/unknown_195720.cpp holds the stream primitives and src/unknown_1946f0.cpp
   the codecs that inline the small helpers */

#ifndef UNKNOWN_1946F0_H
#define UNKNOWN_1946F0_H

#include "cseries.h"
#include "real_math.h"
#include "bitstream.h"
#include "globals.h"

#define k_real_epsilon 0.0001f
#define k_pi 3.14159265358979323846f
#define k_two_pi 6.28318530717958647692f
#define k_debug_marker 0x64656267
#define k_direction_bits 17
#define k_direction_limit 0x20000

/* one face of the cube a quantized direction lives on (the table at 0x475480) */
struct s_direction_face
{
	real scale;
	real_vector3d axes[3];
	real_vector3d origin;
};

extern s_direction_face g_475480[32];
/* 0xb66f0, src/unknown_0b66c0.cpp */
char *csprintf_256(char *buffer, char const *format, ...);

/* 0x24f590: the quantized index of a direction */
long __fastcall function_24f590(real_vector3d const *direction);

/* 0x24f6b0: the direction a quantized index stands for */
void __fastcall function_24f6b0(dword index, real_vector3d *direction);

real function_30bf0(real_vector3d *v);

/* the stream primitives, src/unknown_195720.cpp */
bool function_1946f0(s_bitstream *stream);
void function_194710(s_bitstream *stream, bool discard);
void function_1947a0(s_bitstream *stream);
void function_1947e0(s_bitstream *stream, dword value, long bits);
real function_194870(real_vector3d const *v, real_vector3d *a, real_vector3d *b);
real function_1949b0(real_vector3d const *v, real_vector3d const *w);
real function_194a10(real_vector3d const *axis, real angle, real_vector3d *out);
void function_194b60(s_bitstream *stream, real value, real lo, real hi, long bits);
void function_194bc0(real_vector3d const *direction, s_bitstream *stream);
void function_194fa0(s_bitstream *stream, word *buffer, long count);
real function_194ff0(s_bitstream *stream, real lo, real hi, long bits);
void function_195240(s_bitstream *stream, real_vector3d *forward, real_vector3d *up);
bool function_1952f0(real a1, real a2, real a3, real a4, long bits);
bool function_195560(real_vector3d const *a, real_vector3d const *b, real_vector3d const *up_a, real_vector3d const *up_b);

#endif
