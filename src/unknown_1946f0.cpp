/* UNKNOWN_1946F0.CPP: the bit stream module's codecs that retail compiled
   with the small helpers (a boolean write, vector normalization, the
   direction decode) expanded inline. The stream primitives they call are in
   unknown_195720.cpp */

#include "cseries.h"
#include "real_math.h"
#include "globals.h"
#include "bitstream.h"
#include "unknown_1946f0.h"
#include <math.h>
#include <string.h>

// @flags /O2 /Gr /arch:SSE

s_direction_face g_475480[32];

static __inline real normalize3d(real_vector3d *v)
{
	real m = (real)sqrt(v->i * v->i + v->j * v->j + v->k * v->k);
	if (!(fabs(m) < k_real_epsilon))
	{
		real inv = 1.f / m;
		v->i = inv * v->i;
		v->j = inv * v->j;
		v->k = v->k * inv;
		return m;
	}
	return 0.f;
}

// @retail 0x194830
void function_194830(s_bitstream *stream, bool value)
{
	if (stream->size_in_bytes * 8 - stream->bit_position >= 1 && value)
		stream->data[stream->bit_position / 8] |= (byte)(1 << (stream->bit_position % 8));
	stream->bit_position++;
}

// @retail 0x194c10
void function_194c10(s_bitstream *stream, real_vector3d const *vector, real lo, real hi, long bits)
{
	real_vector3d direction = *vector;
	real magnitude = normalize3d(&direction);
	if (magnitude > hi)
		magnitude = hi;
	if (magnitude >= lo)
	{
		function_194830(stream, false);
		function_194bc0(&direction, stream);
		function_194b60(stream, magnitude, lo, hi, bits);
	}
	else
		function_194830(stream, true);
}

// @retail 0x194d30
void function_194d30(s_bitstream *stream, real_vector3d const *forward, real_vector3d const *up)
{
	real_vector3d direction;
	real_vector3d const *default_forward = g_4687b0;
	if (fabs(forward->i - default_forward->i) < k_real_epsilon && fabs(forward->j - default_forward->j) < k_real_epsilon && fabs(forward->k - default_forward->k) < k_real_epsilon)
	{
		function_194830(stream, true);
		direction = *default_forward;
	}
	else
	{
		function_194830(stream, false);
		long index = function_24f590(forward);
		if ((dword)index >= k_direction_limit)
		{
			char message[256];
			message[0] = 0;
			csprintf(message, "bitstream value %d exceeds limit %d", index, k_direction_limit);
		}
		function_195720(stream, index, k_direction_bits);
		{
			s_direction_face const *face = &g_475480[index >> 12];
			real x = (real)(index & 0x3f) * (1.f / 63.f) * 2.f;
			real y = (real)((index >> 6) & 0x3f) * (1.f / 63.f) * 2.f;
			real z = 0.f;
			if (face->scale != 1.f)
			{
				x = face->scale * x;
				y = face->scale * y;
				z = face->scale * z;
			}
			direction.i = z * face->axes[2].i + y * face->axes[1].i + x * face->axes[0].i + face->origin.i;
			direction.j = z * face->axes[2].j + y * face->axes[1].j + x * face->axes[0].j + face->origin.j;
			direction.k = z * face->axes[2].k + y * face->axes[1].k + x * face->axes[0].k + face->origin.k;
		}
		normalize3d(&direction);
	}
	real angle = function_1949b0(&direction, up);
	real v = (angle - -k_pi) * 40.42535400390625f;
	long q;
	__asm
	{
		fld v
		fistp q
	}
	function_195720(stream, q, 8);
}

// @retail 0x195070
void function_195070(s_bitstream *stream, real_vector3d *out, real lo, real hi, long bits)
{
	if (function_1957d0(stream))
	{
		*out = *g_4687a4;
		return;
	}
	long index = (long)function_1959c0(stream, k_direction_bits);
	real_vector3d direction;
	{
		s_direction_face const *face = &g_475480[index >> 12];
		real x = (real)(index & 0x3f) * (1.f / 63.f) * 2.f;
		real y = (real)((index >> 6) & 0x3f) * (1.f / 63.f) * 2.f;
		real z = 0.f;
		if (face->scale != 1.f)
		{
			x = face->scale * x;
			y = face->scale * y;
			z = face->scale * z;
		}
		direction.i = z * face->axes[2].i + y * face->axes[1].i + x * face->axes[0].i + face->origin.i;
		direction.j = z * face->axes[2].j + y * face->axes[1].j + x * face->axes[0].j + face->origin.j;
		direction.k = z * face->axes[2].k + y * face->axes[1].k + x * face->axes[0].k + face->origin.k;
	}
	normalize3d(&direction);
	real magnitude = function_194ff0(stream, lo, hi, bits);
	out->i = direction.i * magnitude;
	out->j = direction.j * magnitude;
	out->k = direction.k * magnitude;
}

// @retail 0x195370
bool function_195370(real_vector3d const *a, real_vector3d const *b, real lo, real hi, long bits)
{
	real_vector3d direction_b = *b;
	real magnitude_b = normalize3d(&direction_b);
	real_vector3d direction_a = *a;
	real magnitude_a = normalize3d(&direction_a);
	bool short_b = lo > magnitude_b;
	bool short_a = lo > magnitude_a;
	bool near_b = false;
	if (!short_b)
		near_b = function_1952f0(magnitude_b, lo, lo, hi, bits);
	bool near_a = false;
	if (!short_a)
		near_a = function_1952f0(magnitude_a, lo, lo, hi, bits);
	if (short_b)
	{
		if (short_a)
			return true;
		if (!near_a)
			return false;
	}
	else if (short_a)
	{
		if (!near_b)
			return false;
	}
	if (!((s_direction3d *)&direction_b)->quantized_equal(&direction_a))
		return false;
	return function_1952f0(magnitude_b, magnitude_a, lo, hi, bits);
}
