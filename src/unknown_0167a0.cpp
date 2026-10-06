// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include <xmmintrin.h>
#include <string.h>

struct input_mapping_entry
{
	byte type;
	signed char key;
	byte unknown2[5];
	byte flag0 : 1;
	byte flag1 : 1;
	byte flag2 : 1;
	byte flag3 : 1;
	byte flag4 : 1;
	byte flags_pad : 3;
	byte extra;
	byte unknown9[3];
};

input_mapping_entry g_43e909[0x5e];

struct input_bit_vectors
{
	dword v58, v5c, v60, v64, v68;
	dword v6c[2], v74[2], v7c[2], v84[2], v8c[2], v94[2];
	dword v9c[2], va4[2], vac[2], vb4[2], vbc[2];
};

input_bit_vectors g_485058;

// @retail 0x167a0
void function_0167a0()
{
	dword *tables[4][5] = {
		{ 0, 0, 0, 0, 0 },
		{ g_485058.v6c, g_485058.v8c, g_485058.v84, g_485058.v7c, g_485058.v74 },
		{ g_485058.v9c, g_485058.vbc, g_485058.vb4, g_485058.vac, g_485058.va4 },
		{ &g_485058.v5c, &g_485058.v68, &g_485058.v64, &g_485058.v60, &g_485058.v58 },
	};

	memset(g_485058.vbc, 0, sizeof(g_485058.vbc));
	memset(g_485058.vb4, 0, sizeof(g_485058.vb4));
	memset(g_485058.vac, 0, sizeof(g_485058.vac));
	memset(g_485058.va4, 0, sizeof(g_485058.va4));
	memset(g_485058.v9c, 0, sizeof(g_485058.v9c));
	memset(g_485058.v94, 0, sizeof(g_485058.v94));
	memset(g_485058.v8c, 0, sizeof(g_485058.v8c));
	memset(g_485058.v84, 0, sizeof(g_485058.v84));
	memset(g_485058.v7c, 0, sizeof(g_485058.v7c));
	memset(g_485058.v74, 0, sizeof(g_485058.v74));
	memset(g_485058.v6c, 0, sizeof(g_485058.v6c));
	g_485058.v68 = 0;
	g_485058.v64 = 0;
	g_485058.v60 = 0;
	g_485058.v58 = 0;
	g_485058.v5c = 0;

	input_mapping_entry *e = g_43e909;
	for (long i = 0x5e; i > 0; i--, e++)
	{
		if (e->flag0)
			tables[e->type][0][e->key >> 5] |= 1 << (e->key & 0x1f);
		if (e->flag1)
			tables[e->type][1][e->key >> 5] |= 1 << (e->key & 0x1f);
		if (e->flag2)
			tables[e->type][2][e->key >> 5] |= 1 << (e->key & 0x1f);
		if (e->flag3)
			tables[e->type][3][e->key >> 5] |= 1 << (e->key & 0x1f);
		if (e->flag4)
			tables[e->type][4][e->key >> 5] |= 1 << (e->key & 0x1f);
		if (e->extra && e->type == 2)
			g_485058.v94[e->key >> 5] |= 1 << (e->key & 0x1f);
	}
}

word g_485ac0;
dword g_467014, g_467018;
__int64 g_485aa0;

// @retail 0x169f0
void function_0169f0()
{
	g_485aa0 = 1;
	*(dword *)&g_485ad4.lo = g_467014;
	*(dword *)&g_485ad4.hi = g_467018;
	if (g_485ac0 == 0)
		g_485ac0 = 60;
}

struct short_rect
{
	short v0, v1, v2, v3;
};

struct short_rect_pair
{
	short_rect a, b;
};

short_rect_pair g_485a8a;

// @retail 0x17000
void function_17000(byte *context, word const *range)
{
	word const *const *range_reference = &range;
	dword *banks[2] = { (dword *)(context + 0x1530), (dword *)(context + 0x1630) };
	dword *masks[2] = { (dword *)(context + 0x1730), (dword *)(context + 0x1734) };
	byte *definition = *(byte **)(context + 0xc);
	byte *instance = *(byte **)(context + 0x10);
	dword *values = *(dword **)(definition + 0x18);
	byte *record = *(byte **)(instance + 0x24) + (**range_reference & 0x1ff) * 4;
	for (long i = 0; i < (**range_reference >> 9); ++i, record += 4)
	{
		word packed = *(word *)record;
		long bank = (packed >> 4) & 1;
		dword *base = banks[bank];
		dword *mask = masks[bank];
		long slot = packed & 15;
		dword *out = base + slot * 4;
		*mask |= 1 << slot;
		dword *source = values + record[3] * 4;
		switch (((dword)*(word *)record >> 5) & 31)
		{
		case 0: case 5: out[0] = source[0]; break;
		case 1: case 6: out[1] = source[1]; break;
		case 2: case 7: out[2] = source[2]; break;
		case 3: case 8: out[3] = source[3]; break;
		case 4: case 16: case 17:
			out[0] = source[0]; out[1] = source[1]; out[2] = source[2]; break;
		case 9: case 11:
			out[0] = source[0]; out[1] = source[1]; break;
		case 10: case 12:
			out[2] = source[2]; out[3] = source[3]; break;
		case 13: case 18: case 19: case 20:
			out[0] = source[0]; out[1] = source[1]; out[2] = source[2]; out[3] = source[3]; break;
		case 14: case 15:
			out[0] = source[0]; out[1] = source[1]; out[3] = source[3]; break;
		}
	}
}

const real g_45dd7c = 480.0f, g_45dd80 = 640.0f;

struct s_2f6b0_point
{
	short x, y;
};

// @retail 0x2f6b0
void function_2f6b0(s_2f6b0_point const *position, s_2f6b0_point const *grid,
	s_2f6b0_point const *span, short_rect *outer, short_rect *inner)
{
	(void)&grid;
	(void)&span;
	(void)&outer;
	short_rect full = g_485a8a.a;
	short_rect bounds = g_485a8a.b;
	s_2f6b0_point step;
	step.x = (bounds.v3 - bounds.v1) / grid->x;
	step.y = (bounds.v2 - bounds.v0) / grid->y;
	short x = bounds.v1;
	short y = bounds.v0;
	bounds.v1 = x + (word)position->x * step.x;
	bounds.v3 = x + (word)(span->x + position->x) * step.x;
	bounds.v0 = y + (word)position->y * step.y;
	bounds.v2 = y + (word)(span->y + position->y) * step.y;
	*outer = bounds;
	*inner = bounds;
	if (position->x == 0)
		outer->v1 = full.v1;
	else
		inner->v1 += 4;
	if (position->x + span->x >= grid->x)
		outer->v3 = full.v3;
	else
		inner->v3 -= 4;
	if (position->y == 0)
		outer->v0 = full.v0;
	else
		inner->v0 += 4;
	if (position->y + span->y >= grid->y)
		outer->v2 = full.v2;
	else
		inner->v2 -= 4;
}

// @retail 0x16a30
void function_016a30(real scale, short y, short x)
{
	g_485a8a.a.v1 = 0;
	g_485a8a.a.v0 = 0;
	g_485a8a.a.v3 = (short)(scale * g_45dd80);
	g_485a8a.a.v2 = (short)(scale * g_45dd7c);
	g_485a8a.b.v3 = g_485a8a.a.v3 - y;
	g_485a8a.b.v1 = y;
	g_485a8a.b.v0 = x;
	g_485a8a.b.v2 = g_485a8a.a.v2 - x;
}

byte g_485ac2;

// @retail 0x16a90
byte function_016a90()
{
	s_game_options_view *s = g_4e6948;
	if (s && s->flag && s->index != NONE && s->state == 3)
		return false;
	return g_485ac2;
}

long g_485898;

// @retail 0x16ac0
bool function_016ac0()
{
	return g_485898 >= 5 && g_485898 <= 7;
}

// @retail 0x16ae0
long function_016ae0(real value)
{
	__asm {
		movss xmm0, value
		cvttss2si eax, xmm0
		cvtsi2ss xmm1, eax
		cmpneqss xmm1, xmm0
		cmpltss xmm0, g_45dbd8
		andps xmm0, xmm1
		movmskps ecx, xmm0
		sub eax, ecx
	}
}

dword g_485b78[5];

// @retail 0x1bdf0
void function_1bdf0(void)
{
    g_485b78[0] |= g_485058.v74[0];
    g_485b78[1] |= g_485058.v74[1];
    g_485b78[2] |= g_485058.va4[0];
    g_485b78[3] |= g_485058.va4[1];
    g_485b78[4] |= g_485058.v58;
}

// @retail 0x1be50
void function_1be50(void)
{
    g_485b78[0] |= g_485058.v84[0];
    g_485b78[1] |= g_485058.v84[1];
    g_485b78[2] |= g_485058.vb4[0];
    g_485b78[3] |= g_485058.vb4[1];
    g_485b78[4] |= g_485058.v64;
}

struct s_33a0b_view;
extern s_33a0b_view *g_485a58;

// @retail 0x1bd50
void function_1bd50(void *state)
{
    if (g_485a58 != state || !state)
    {
        g_485a58 = (s_33a0b_view *)state;
        g_485b78[0] |= g_485058.v8c[0];
        g_485b78[1] |= g_485058.v8c[1];
        g_485b78[2] |= g_485058.vbc[0];
        g_485b78[3] |= g_485058.vbc[1];
        g_485b78[4] |= g_485058.v68;
        g_485b78[0] |= g_485058.v7c[0];
        g_485b78[1] |= g_485058.v7c[1];
        g_485b78[2] |= g_485058.vac[0];
        g_485b78[3] |= g_485058.vac[1];
        g_485b78[4] |= g_485058.v60;
    }
}


#include <math.h>

struct s_1b230_callback
{
	void *context;
	real (__stdcall *evaluate)(void *, long);
};

struct s_1b230_function
{
	long size;
	byte *data;
};

extern double g_4858a0;
real function_13b390(void const *function, real input, real range);

// @retail 0x1b230
real function_1b230(s_1b230_function const *definition, long input_index, long range_index, real period)
{
	(void)&range_index;
	(void)&period;
	real input = 0.0f;
	real range = 0.0f;
	s_1b230_callback *state = (s_1b230_callback *)g_485a58;
	if (state && state->evaluate)
	{
		input = state->evaluate(state->context, input_index);
		range = ((s_1b230_callback *)g_485a58)->evaluate(((s_1b230_callback *)g_485a58)->context, range_index);
	}
	if (!input_index)
	{
		input = (real)(g_4858a0 / period);
		if (definition->data[0] != 3)
			input = (real)fmod((double)input, 1.0);
	}
	return function_13b390(definition, input, range);
}
