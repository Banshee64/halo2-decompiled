// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_020560.CPP: the timed effect values, the table of visibility
   slots, a pixel shader and the render state wrappers */

#include "unknown_11c920.h"
#include <string.h>
#include <math.h>
#include <xtl.h>
#include "globals.h"
#include "unknown_0259d0.h"
#include "timed_effect.h"
#include "visibility_slot.h"
#include "unknown_123b30.h"
#include "crc.h"

#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

struct s_slot_key
{
	byte a;
	long b;
	long c;
	long d;
	long e;
};

struct s_tag_flags
{
	byte unknown00[0x28];
	unsigned short flag0 : 1;
	unsigned short flag1 : 1;
	unsigned short flag2 : 1;
	unsigned short flag3 : 1;
	unsigned short flag4 : 1;
	unsigned short flag5 : 1;
};

dword __cdecl pack_color3f(const color3f *color);

struct hash_table;
void hash_table_initialize(hash_table *table);
void function_20e50(long index);


byte g_51f408;
byte g_51f409;
hash_table *g_51f400;
long g_5234c4;
long g_5234b4[4];
extern __int64 g_485aa0;
byte g_485607;
dword g_4850c8;
real g_5234c8;
word g_485648;
word g_48564a;
word g_48564c;
word g_48564e;
dword g_4b81fc[0xa6];
D3DPIXELSHADERDEF g_484f68;
long g_5234b0;
extern long g_485898;
short g_467010 = NONE;

void function_0222d0(D3DRENDERSTATETYPE state, dword value);

struct s_packed_render_states
{
	byte field_00[0x14];
	byte *data;
};

struct s_render_state_source
{
	byte field_00[0x20];
	s_packed_render_states *states;
};

// @retail 0x18900
void __stdcall function_18900(s_render_state_source const *source, word const *range)
{
	byte const *entry = source->states->data + (*range & 0x1ff) * 5;
	for (long i = 0; i < (*range >> 9); entry += 5, i++)
		D3DDevice_SetRenderStateNotInline((D3DRENDERSTATETYPE)entry[0], *(dword const *)(entry + 1));
}

struct s_render_reset_state
{
    long index;
    byte unknown04[8];
    long fields[5];
    s_packed_render_states *states;
    byte *block;
    byte unknown28[0x40 - 0x28];
    long index40;
    byte unknown44[0x1320 - 0x44];
    struct { long index; byte unknown04[12]; real value; byte unknown14[0x2c]; } slots[4];
};

// @retail 0x16b10
void function_16b10(s_render_reset_state *state)
{
    if (state->block)
        function_18900((s_render_state_source const *)state, (word const *)(state->block + 0x11c));
    state->fields[0] = 0;
    state->fields[1] = 0;
    state->fields[2] = 0;
    state->fields[3] = 0;
    state->fields[4] = 0;
    state->states = 0;
    state->block = 0;
    state->index = NONE;
    state->index40 = NONE;
    for (long i = 0; i < 4; i++)
    {
        state->slots[i].index = NONE;
        state->slots[i].value = 3.0f;
    }
}

extern byte *g_485a80;

// @retail 0x16f60
void function_16f60(byte *state, word const *range)
{
    (void)&range;
    long i = 0;
    byte *values = *(byte **)(*(byte **)(state + 0xc) + 8);
    byte *entry = *(byte **)(*(byte **)(state + 0x10) + 0x24) + (*range & 0x1ff) * 4;
    for (; i < (*range >> 9); entry += 4, ++i)
    {
        byte *slot = state + 0x320 + entry[0] * 0x40;
        long *source = (long *)(values + entry[3] * 12);
        if (source[0] != NONE)
        {
            *(long *)slot = source[0];
            *(real *)(slot + 0x18) = (real)source[1];
            *(long *)(slot + 0x14) = source[2];
        }
        else
        {
            *(long *)slot = *(long *)(g_485a80 + 0x64);
            *(real *)(slot + 0x18) = 0.0f;
            *(real *)(slot + 0x14) = 0.0f;
        }
        *(real *)(slot + 0x10) = 3.0f;
    }
}

// @retail 0x15370
void function_15370(short mode)
{
	switch (mode)
	{
	case 0:
		function_0222d0(D3DRS_STENCILENABLE, FALSE);
		break;
	case 1: case 2:
		function_0222d0(D3DRS_STENCILENABLE, TRUE);
		function_0222d0(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		function_0222d0(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		function_0222d0(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
		function_0222d0(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		function_0222d0(D3DRS_STENCILREF, mode == 1 ? 0 : 255);
		function_0222d0(D3DRS_STENCILMASK, 255);
		function_0222d0(D3DRS_STENCILWRITEMASK, 255);
		break;
	case 3:
		function_0222d0(D3DRS_STENCILENABLE, TRUE);
		function_0222d0(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		function_0222d0(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		function_0222d0(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
		function_0222d0(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		function_0222d0(D3DRS_STENCILREF, 0);
		function_0222d0(D3DRS_STENCILMASK, 1);
		function_0222d0(D3DRS_STENCILWRITEMASK, 0);
		break;
	case 4:
		function_0222d0(D3DRS_STENCILENABLE, TRUE);
		function_0222d0(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		function_0222d0(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		function_0222d0(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
		function_0222d0(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
		function_0222d0(D3DRS_STENCILREF, 0);
		function_0222d0(D3DRS_STENCILMASK, 1);
		function_0222d0(D3DRS_STENCILWRITEMASK, 0);
		break;
	case 5:
		function_0222d0(D3DRS_STENCILENABLE, TRUE);
		function_0222d0(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		function_0222d0(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		function_0222d0(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
		function_0222d0(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		function_0222d0(D3DRS_STENCILREF, 2);
		function_0222d0(D3DRS_STENCILMASK, 1);
		function_0222d0(D3DRS_STENCILWRITEMASK, 2);
		break;
	case 6:
		function_0222d0(D3DRS_STENCILENABLE, TRUE);
		function_0222d0(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		function_0222d0(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		function_0222d0(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
		function_0222d0(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		function_0222d0(D3DRS_STENCILREF, 0);
		function_0222d0(D3DRS_STENCILMASK, 3);
		function_0222d0(D3DRS_STENCILWRITEMASK, 0);
		break;
	}
	g_467010 = mode;
}

// @retail 0x15680
void function_15680(short mode)
{
	switch (mode)
	{
	case 0:
		function_0222d0(D3DRS_STIPPLEENABLE, FALSE);
		function_0222d0(D3DRS_SAMPLEALPHA, 0);
		break;
	case 1:
		function_0222d0(D3DRS_STIPPLEENABLE, TRUE);
		function_0222d0(D3DRS_SAMPLEALPHA, 0);
		break;
	case 2:
		function_0222d0(D3DRS_STIPPLEENABLE, FALSE);
		function_0222d0(D3DRS_SAMPLEALPHA, 0x110);
		break;
	}
}

struct s_5093e4
{
	bool flag0;
	bool flag1;
	byte unknown02[2];
	real value4;
	real value8;
	real valuec;
	real value10;
	real value14;
	bool flag18;
	bool flag19;
	bool flag1a;
	byte unknown1b;
	real value1c;
	real value20;
};

extern s_5093e4 *g_5093e4;

// @retail 0x20eb0
void function_020eb0(void)
{
	long size = sizeof(s_5093e4);
	byte *memory = game_state_globals.base_address + game_state_globals.cpu_allocation_size;
	game_state_globals.cpu_allocation_size += size;
	function_163ba0(&game_state_globals.allocation_size_checksum, &size, sizeof(size));
	s_5093e4 *state = (s_5093e4 *)memory;
	g_5093e4 = state;
	state->flag0 = false;
	state->flag1 = false;
	state->value4 = -1.0f;
	state->value8 = -1.0f;
	state->valuec = -1.0f;
	state->value10 = -1.0f;
	state->value14 = -1.0f;
	state->flag18 = true;
	state->flag19 = false;
	state->value1c = -1.0f;
	state->value20 = -1.0f;
	state->flag1a = false;
}

// @retail 0x206a0
void function_0206a0(void)
{
	long frame = g_485aa0;
	if (frame >= 0 && (((g_485898 >= 4 && g_485898 <= 7) && frame == g_5234b0) || frame > g_5234b0 || !g_5234b0))
	{
		for (long i = 0; i < 511; i++)
			g_51f40c.slots[i].valid = false;
		g_5234b0 = frame;
		g_51f408 = true;
		long special_mode = 1;
		if (g_485898 < 4 || g_485898 > 7)
			special_mode = 0;
		g_51f409 = !(byte)special_mode;
	}
	else
		g_51f408 = false;
	g_5234c4 = NONE;
}

// @retail 0x20560
real function_020560(long index)
{
	real result = 0.0f;

	if (g_5093e0 && index >= 0 && index < 32)
	{
		real fraction = 0.0f;

		if (g_5093e0->times[index][1] != g_5093e0->times[index][0])
		{
			fraction = (real)PIN((g_4858a0 - g_5093e0->times[index][0]) / (g_5093e0->times[index][1] - g_5093e0->times[index][0]), 0.0, 1.0);
		}

		result = (1.0f - fraction) * g_5093e0->values[index][0] + g_5093e0->values[index][1] * fraction;
		result = PIN(result, 0.0f, 1.0f);
	}

	return result;
}

// @retail 0x20720
void function_020720(long player)
{
	if (player == NONE)
		return;
	if (g_51f408 && !g_4850c8)
		g_5234c4 = player;
	else
	{
		for (long index = 0; index < g_51f40c.count; index++)
			function_20e50(index);
	}
}

// @retail 0x20770
void function_020770(void)
{
	if (g_51f408 && g_51f409 && g_5234c4 != NONE)
	{
		long frame = g_485aa0;
		long player = g_5234c4;

		if (frame > g_5234b4[player])
		{
			long set = (frame + 1) % 3;

			for (long i = 0; i < g_51f40c.count; i++)
			{
				s_slot *slot = &g_51f40c.slots[i];

				if (player == slot->b)
				{
					if (slot->valid)
					{
						long count = 0;
						if ((1 << (i & 0x1f)) & g_51f40c.bitsets[set][i >> 5])
						{
							ULONGLONG timestamp;

							if (D3DDevice_GetVisibilityTestResult(i * 3 + set, (UINT *)&count, &timestamp) == S_OK)
							{
								if (slot->k > 0)
								{
									real ratio = (real)count / (real)slot->k;
									ratio = PIN(ratio, 0.0f, 1.0f);
									ratio = ratio * 256.0f;
									dword target = (dword)PIN(ratio, 0.0f, 255.0f);

									if (((1 << slot->c) & 0xf) || slot->c == 4)
									{
										bool slow = false;
										bool fast = false;
										dword current;

										if (slot->size >= 24 && *(long *)&slot->data[0] != NONE)
										{
											s_tag_flags *flags = g_4e3b44[*(long *)&slot->data[0] & 0xffff].flags;
											slow = TEST_FIELD_BIT(flags->flag4);
											fast = TEST_FIELD_BIT(flags->flag5);
										}

										current = slot->f;
										if (current > target)
										{
											if (fast)
												slot->f = (target * 15 + current) >> 4;
											else
												slot->f = (current + target) >> 1;
										}
										else if (current < target)
										{
											if (slow)
												slot->f = (current + target) >> 1;
											else
												slot->f = (current * 7 + target) >> 3;
										}
									}
									else
									{
										slot->f = target;
									}

									if (count > 0)
										slot->f = (slot->f < 1) ? 1 : slot->f;
								}
								else
								{
									slot->f = 0;
								}
							}
						}
					}
					else
					{
						function_20e50(i);
					}
				}
			}

			g_5234b4[player] = frame;
		}
	}
}

// @retail 0x209b0
long function_0209b0(s_slot_key *key, const void *data, long size)
{
	if (!g_51f408 || g_5234c4 == NONE || g_485607 || g_4850c8)
		return NONE;

	long index;
	s_slot *slot;

	for (index = 0; index < g_51f40c.count; index++)
	{
		slot = &g_51f40c.slots[index];
		if (slot->used && key->a == slot->a && key->b == slot->b && key->c == slot->c && key->d == slot->d && key->e == slot->e)
			goto found;
	}

	for (index = 0; index < 511; index++)
	{
		slot = &g_51f40c.slots[index];
		if (!slot->used)
			goto create;
	}
	return NONE;

create:
	function_20e50(index);
	slot->a = key->a != 0;
	slot->b = key->b;
	slot->c = key->c;
	slot->d = key->d;
	slot->e = key->e;
	if (index + 1 > g_51f40c.count)
		g_51f40c.count = index + 1;

found:
	slot = &g_51f40c.slots[index];
	if (!slot->valid)
	{
		slot->size = size;
		slot->used = 1;
		slot->valid = 1;
		if (size > 0)
			memcpy(slot->data, data, size);
	}

	return index;
}

// @retail 0x20b40
void function_020b40(long value, long index)
{
	if (g_51f408 && g_5234c4 != NONE)
	{
		long count = g_51f40c.count;

		if (PIN(index, 0, count - 1) == index)
		{
			if (PIN(value, 0, count - 1) == value || value == NONE)
			{
				s_slot *slot = &g_51f40c.slots[index];

				if (value != NONE)
					slot->j = value;
				else
					slot->j = NONE;
			}
		}
	}
}

// @retail 0x20d80
real function_020d80(s_slot *slot, bool skip)
{
	real result = 1.0f;

	if (slot->valid && slot->used)
	{
		result = slot->f * g_5234c8;
		result = PIN(result, 0.0f, 1.0f);

		if (!skip && slot->j != 0x1ff)
		{
			if (PIN(slot->j, 0, g_51f40c.count - 1) == slot->j)
			{
				s_slot *linked = &g_51f40c.slots[slot->j];

				if (linked->j == 0x1ff && linked->b == slot->b && linked->valid && linked->used)
				{
					real linked_result = linked->f * g_5234c8;
					linked_result = PIN(linked_result, 0.0f, 1.0f);
					result = linked_result * result;
				}
			}
		}
	}

	return result;
}

// @retail 0x20f30
void function_020f30(real a, real b)
{
	dword count;
	dword full;
	dword half = 0xc00;

	memset(&g_484f68, 0, sizeof(g_484f68));
	g_484f68.PSTextureModes = 0x8421;

	b = PIN(b, 0.0f, 1.0f);
	a = PIN(a, 0.0f, 1.0f);

	if (a > 0.0f)
	{
		real x = (a + 1.0f) * 0.5f;
		real scale = 255.0f;
		long v = 0;

		x = PIN(x, 0.0f, 1.0f);
		__asm
		{
			fld x
			fld scale
			fmulp st(1), st
			fistp v
			shl v, 24
		}
		g_484f68.PSConstant1[2] = v;
		full = 0x30c00;
		g_484f68.PSAlphaInputs[2] = 0x1c121d12;
		g_484f68.PSAlphaOutputs[2] = half;
	}
	else
	{
		full = 0x30c00;
		g_484f68.PSAlphaInputs[2] = 0x1c201d20;
		g_484f68.PSAlphaOutputs[2] = full;
	}

	g_484f68.PSAlphaInputs[0] = 0x18201920;
	g_484f68.PSAlphaOutputs[0] = full;
	g_484f68.PSAlphaInputs[1] = 0x1a201b20;
	g_484f68.PSAlphaOutputs[1] = 0x30d00;
	g_484f68.PSFinalCombinerInputsEFG = 0x1c00;

	count = 3;

	if (b > 0.0f)
	{
		color3f color;
		color.red = b;
		color.green = b;
		color.blue = b;
		dword pixel = pack_color3f(&color);
		g_484f68.PSConstant0[1] = pixel;
		g_484f68.PSConstant0[4] = pixel;
		g_484f68.PSConstant0[7] = pixel;
		g_484f68.PSRGBInputs[0] = 0x8200940;
		g_484f68.PSRGBOutputs[0] = 0x20c00;
		g_484f68.PSRGBInputs[1] = 0xcc012020;
		g_484f68.PSRGBOutputs[1] = full;
		g_484f68.PSRGBInputs[2] = 0xc082c09;
		g_484f68.PSRGBOutputs[2] = half;
		g_484f68.PSRGBInputs[3] = 0xa200b40;
		g_484f68.PSRGBOutputs[3] = 0x20d00;
		g_484f68.PSRGBInputs[4] = 0xcd012020;
		g_484f68.PSRGBOutputs[4] = 0x30d00;
		g_484f68.PSRGBInputs[5] = 0xd0a2d0b;
		g_484f68.PSRGBOutputs[5] = 0xd00;
		g_484f68.PSRGBInputs[6] = 0xc200d40;
		g_484f68.PSRGBOutputs[6] = 0x20400;
		g_484f68.PSRGBInputs[7] = 0xc4012020;
		g_484f68.PSRGBOutputs[7] = 0x30400;
		g_484f68.PSFinalCombinerInputsABCD = 0x40c0d00;
		count = 8;
	}
	else
	{
		g_484f68.PSRGBInputs[0] = 0x8200920;
		g_484f68.PSRGBOutputs[0] = full;
		g_484f68.PSRGBInputs[1] = 0xa200b20;
		g_484f68.PSRGBOutputs[1] = 0x30d00;
		g_484f68.PSRGBInputs[2] = 0xc200d20;
		g_484f68.PSRGBOutputs[2] = full;
		g_484f68.PSFinalCombinerInputsABCD = 0xc;
	}

	g_484f68.PSCombinerCount = count | 0x11000;
	D3DDevice_SetPixelShaderProgram(&g_484f68);
}

// @retail 0x222d0
void function_0222d0(D3DRENDERSTATETYPE state, dword value)
{
	g_4b81fc[state] = value;
	D3DDevice_SetRenderState(state, value);
}

// @retail 0x224f0
void function_0224f0(dword stage, D3DTEXTURESTAGESTATETYPE type, dword value)
{
	D3DDevice_SetTextureStageState(stage, type, value);
}

// @retail 0x226b0
void function_0226b0(void)
{
	if (g_51f400)
		hash_table_initialize(g_51f400);
}

struct s_0226d0_block
{
	long field_00;
	long checksum;
	byte field_08[0x14];
	long handle;
};

struct s_0226d0_structure
{
	byte field_00[8];
	long checksum;
};

// @retail 0x226d0
bool function_0226d0(void)
{
	bool result = false;
	if (g_4e0344 && g_4e0344->count > 0 && g_4e0348)
	{
		s_0226d0_block *block = (s_0226d0_block *)g_4e0344->bsp;
		if (block->handle != NONE && block->checksum == ((s_0226d0_structure *)g_4e0348)->checksum)
			result = true;
	}
	return result;
}

// @retail 0x22710
bool function_022710(void)
{
	bool result = false;

	if (g_48564a == 0 && g_485648 == 0 && g_48564e == 0x280 && g_48564c == 0x1e0 && g_4ba04c <= 1)
		result = true;

	return result;
}

// @retail 0x22750
void function_022750(const real *a, long size, const real *b, real *out, real t)
{
	long count = size * size;

	for (long i = 0; i < count; i++)
		out[i] = (a[i] - b[i]) * t + b[i];
}

// @retail 0x22c30
real function_022c30(const point3f *a, const point3f *c, const point3f *b)
{
	vector3f v1, v2;
	real result = 0.0f;

	v2.i = a->x - c->x;
	v2.j = a->y - c->y;
	v2.k = a->z - c->z;
	v1.i = a->x - b->x;
	v1.j = a->y - b->y;
	v1.k = a->z - b->z;

	real length = (real)sqrt(v1.k * v1.k + v1.j * v1.j + v1.i * v1.i);
	if (length > 0.0001f)
	{
		real inverse = 1.0f / length;
		result = ((v1.k * inverse) * v2.k + (v1.j * inverse) * v2.j + (v1.i * inverse) * v2.i) * inverse;
	}

	return result;
}


real g_4b8494;
long g_467130;
long g_4858b4;
extern D3DPalette *g_484dbc;
extern long g_484dc0[4];
dword *function_1c290(real value);
void function_0496a0(void);
void function_049740(void);

// @retail 0x445d0
void function_445d0(void)
{
	if (!(fabs(g_4b8494 - 1.0f) < 0.0001f))
	{
		D3DDevice_SetStipple(function_1c290(1.0f));
		g_4b8494 = 1.0f;
	}
	switch (g_467130)
	{
	case 3:
		D3DDevice_SetPalette(0, g_484dbc);
		g_484dc0[0] = NONE;
		break;
	case 10:
		function_0496a0();
		break;
	case 11:
		function_049740();
		break;
	case 16: break;
	case 17: break;
	}
	g_467130 = 0;
	g_4858b4 = NONE;
}
