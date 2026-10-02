// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_033A0B.CPP: vector getters */

/* Not functions of their own: each is a case body of the switch that starts
   function 0x33980, at the address its "case at" line gives. They await
   folding into that function. */

#include "cseries.h"
#include "real_math.h"

struct s_33a0b_view
{
	byte unknown00[0x10];
	real_vector3d vector10;
	byte unknown1c[0x14];
	real_vector3d vector30;
	real_vector3d vector3c;
	real_vector3d vector48;
	real_vector3d vector54;
};

struct s_33a0b_default
{
	dword unknown0;
	real_vector3d vector;
};

struct s_dwords3 { dword a, b, c; };

inline PRIVATE void copy_vector(real_vector3d *dest, real_vector3d const *src)
{
	s_dwords3 *d = (s_dwords3 *)dest;
	s_dwords3 const *s = (s_dwords3 const *)src;
	d->a = s->a;
	d->b = s->b;
	d->c = s->c;
}

real_vector3d g_4859dc;
real_vector3d g_4859ec;
s_33a0b_view *g_485a58;
s_33a0b_default *g_4686d4;
real g_485a6c;
real g_485a70;
real_vector3d g_485768;
real g_485774;
real_vector3d g_485780;

inline PRIVATE real pin_unit(real value)
{
	if (value < 0.0f)
	{
		value = 0.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}
	return value;
}

// case at 0x33a0b
void function_033a0b(real_vector3d *out)
{
	copy_vector(out, &g_4859dc);
}

// case at 0x33a25
void function_033a25(real_vector3d *out)
{
	copy_vector(out, &g_4859ec);
}

// case at 0x33a3f
void function_033a3f(real_vector3d *out)
{
	real_vector3d *vector = !g_485a58 ? &g_4686d4->vector : &g_485a58->vector3c;
	copy_vector(out, vector);
}

// case at 0x33a66
void function_033a66(real_vector3d *out)
{
	real_vector3d *vector = !g_485a58 ? &g_4686d4->vector : &g_485a58->vector30;
	copy_vector(out, vector);
}

// case at 0x33ac5
void function_033ac5(real_vector3d *out)
{
	copy_vector(out, &g_485a58->vector48);
}

// case at 0x33ada
void function_033ada(real_vector3d *out)
{
	real_vector3d *vector = !g_485a58 ? &g_4686d4->vector : &g_485a58->vector30;
	copy_vector(out, vector);
	out->i = pin_unit(out->i * g_485a6c);
	out->j = pin_unit(out->j * g_485a6c);
	out->k = pin_unit(out->k * g_485a6c);
}

// case at 0x33cdd
void function_033cdd(real_vector3d *out)
{
	copy_vector(out, &g_485768);
}

// case at 0x33cf7
void function_033cf7(real_vector3d *out)
{
	out->i = g_485768.i * g_485774;
	out->j = g_485768.j * g_485774;
	out->k = g_485768.k * g_485774;
}

// case at 0x33d36
void function_033d36(real_vector3d *out)
{
	copy_vector(out, &g_485780);
}
