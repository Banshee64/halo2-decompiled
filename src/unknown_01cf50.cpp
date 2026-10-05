// @flags /O2 /Ob1 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include <xtl.h>
#include <string.h>
#include "unknown_058ee0.h"

struct s_buffer_pair
{
	long first;
	long second;
};

s_buffer_pair g_5093c0;
long g_5093c8;
long g_5093cc;

// @retail 0x13d50
void function_13d50(s_buffer_pair const *values, long mode, long count)
{
	/* The count remains on the stack in retail. */
	(void)&count;
	long first = values->first;
	long second = values->second;
	g_5093c8 = mode;
	g_5093c0.first = first;
	g_5093c0.second = second;
	g_5093cc = count;
}

/* the texture stages: [0] the textures set on the device (0x51f3c8), [1] the
   textures wanted (0x51f3d8) */
IDirect3DBaseTexture8 *g_51f3c8[2][4];
long g_5093d8;
dword g_487288[17][32];
D3DSurface *g_51f3f4;
D3DSurface *g_51f3f8;
byte g_51f3fc;

extern D3DResource *g_509374, *g_509378, *g_50937c, *g_509380;
D3DTexture *g_509354[2];
D3DSurface *g_50935c, *g_509360, *g_509364, *g_509370;
D3DTexture *g_509368, *g_50936c;
D3DSurface *g_509384;
D3DTexture *g_50938c;
D3DSurface *g_509390, *g_509394, *g_509398, *g_50939c, *g_5093a0, *g_5093a4, *g_5093a8;
short g_485602;
real g_4670c8 = 1.0f;
extern byte g_485607, g_5093fc;
extern word g_485648, g_48564a, g_48564c, g_48564e;
extern __int64 g_485aa0;
long g_485af4[4], g_485b04[4];

void *function_01dcf0(long index);
void *function_01dd20(long index, long element);
void *function_01dcc0(long index);
bool function_01dd60(long index, long *width, long *height);
long function_25960(void);
bool function_1cd30(D3DSurface *target, D3DSurface *depth, bool flag);

// @retail 0x14bc0
void function_14bc0(short index, short element, bool use_depth)
{
	D3DSurface *target = NULL;
	D3DSurface *depth = NULL;
	bool flag = false;
	switch (index)
	{
	case 0: target = g_50935c; depth = g_509364; flag = true; break;
	case 4: target = g_509360; flag = true; break;
	case 16: case 29: target = (D3DSurface *)g_509378; break;
	case 17: target = (D3DSurface *)g_509380; break;
	case 3: target = g_509370; depth = g_509364; flag = true; break;
	case 1: target = (D3DSurface *)function_01dcf0(1); depth = g_509364; break;
	case 18: target = (D3DSurface *)function_01dcf0(18); depth = g_509364; break;
	case 9:
		target = g_509384;
		depth = (D3DSurface *)function_01dcf0(9);
		target->Size = depth->Size;
		target->Data = ((D3DSurface *)function_01dcf0(9))->Data;
		break;
	case 10: case 11: case 12: case 25: case 26: case 27:
		target = (D3DSurface *)function_01dcf0(index); depth = NULL; break;
	case 13: target = (D3DSurface *)function_01dd20(13, element); depth = NULL; break;
	case 19: target = (D3DSurface *)function_01dcf0(19); depth = NULL; break;
	case 20: target = (D3DSurface *)function_01dcf0(20); depth = NULL; break;
	case 23: depth = NULL; break;
	case 5: case 6: case 7: case 8: case 15:
		target = (D3DSurface *)function_01dcf0(index); depth = g_509364; break;
	case 33: target = g_509390; depth = g_5093a8; break;
	case 34: target = g_509394; depth = g_5093a8; break;
	case 35: target = g_509398; depth = g_5093a8; break;
	case 36: target = g_50939c; depth = g_5093a8; break;
	case 37: target = g_5093a0; depth = g_5093a8; break;
	case 38: target = g_5093a4; depth = g_5093a8; break;
	case 22: target = (D3DSurface *)function_01dcf0(22); depth = NULL; break;
	case 21: target = (D3DSurface *)function_01dcf0(21); depth = NULL; break;
	case 14: break;
	default: __assume(0);
	}
	function_1cd30(target, use_depth ? depth : NULL, flag);
	D3DVIEWPORT8 viewport;
	if (g_485602 == 2)
	{
		viewport.X = 0; viewport.Y = 0;
		viewport.Width = 128; viewport.Height = 128;
	}
	else if (index == 0 || index == 2 || index == 1 || index == 18 || index == function_25960())
	{
		real scale;
		if (index == function_25960())
		{
			if (g_485607)
			{
				viewport.X = 0; viewport.Y = 0;
				viewport.Width = 640; viewport.Height = 480;
				goto viewport_ready;
			}
			scale = 1.0f;
		}
		else
		{
			if (g_485607 && !g_5093fc)
				scale = g_4670c8 < 0.0625f ? 0.0625f : g_4670c8 > 1.0f ? 1.0f : g_4670c8;
			else
				scale = 1.0f;
		}
		viewport.X = (long)((short)g_48564a * (double)scale);
		viewport.Y = (long)((short)g_485648 * (double)scale);
		viewport.Width = (long)(((short)g_48564e - (short)g_48564a) * (double)scale);
		viewport.Height = (long)(((short)g_48564c - (short)g_485648) * (double)scale);
	}
	else
	{
		D3DSURFACE_DESC desc;
		D3DSurface_GetDesc(target, &desc);
		viewport.X = 0; viewport.Y = 0;
		viewport.Width = desc.Width; viewport.Height = desc.Height;
	}
viewport_ready:
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	D3DDevice_SetViewport(&viewport);
}

// @retail 0x14f60
void function_14f60(short stage, short index)
{
	D3DBaseTexture *texture = NULL;
	long width = 0, height = 0;
	function_01dd60(index, &width, &height);
	g_485af4[stage] = width;
	g_485b04[stage] = height;
	switch (index)
	{
	case 0: texture = g_509354[(g_485aa0 - 1) % 2]; break;
	case 4: texture = g_509354[g_485aa0 % 2]; break;
	case 16: case 29: texture = (D3DBaseTexture *)g_509374; break;
	case 17: texture = (D3DBaseTexture *)g_50937c; break;
	case 3: texture = g_509368; break;
	case 24: texture = g_50936c; break;
	case 1: texture = (D3DBaseTexture *)function_01dcc0(1); break;
	case 18: texture = (D3DBaseTexture *)function_01dcc0(18); break;
	case 19: texture = (D3DBaseTexture *)function_01dcc0(19); break;
	case 23: texture = g_509354[(g_485aa0 - 1) % 2]; break;
	case 13: texture = (D3DBaseTexture *)function_01dcc0(13); break;
	case 20: texture = (D3DBaseTexture *)function_01dcc0(20); break;
	case 33: case 34: case 35: case 36: case 37: case 38: texture = g_50938c; break;
	case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 15: case 21: case 22: case 25: case 26: case 27:
		texture = (D3DBaseTexture *)function_01dcc0(index); break;
	case 14: break;
	default: __assume(0);
	}
	g_51f3c8[1][stage] = texture;
}

struct s_mask_offset
{
	short x;
	short y;
};

s_mask_offset const g_43f188[16] =
{
	{0, 0}, {2, 2}, {0, 2}, {2, 0},
	{1, 1}, {3, 3}, {3, 1}, {1, 3},
	{0, 1}, {2, 3}, {3, 0}, {1, 2},
	{1, 0}, {3, 2}, {0, 3}, {2, 1}
};

// @retail 0x1c1b0
void function_1c1b0(void)
{
	memset(g_487288, 0, sizeof(g_487288));
	for (long count = 0; count <= 16; count++)
	{
		long x;
		dword *mask = g_487288[count];
		for (x = 0; x < 32; x += 4)
		{
			long base = x;
			for (long rows = 8; rows > 0; rows--, base += 128)
			{
				for (long i = 0; i < count; i++)
				{
					long bit = g_43f188[i].y * 32 + base + g_43f188[i].x;
					mask[bit / 32] = mask[bit / 32] | (1 << (bit % 32));
				}
			}
		}
	}
}

// @retail 0x1cd30
bool function_1cd30(D3DSurface *target, D3DSurface *depth, bool flag)
{
	if (g_51f3f4 != target || g_51f3f8 != depth)
	{
		if (g_51f3f4 && target && target->Common == g_51f3f4->Common && target->Format == g_51f3f4->Format && target->Size == g_51f3f4->Size)
			D3DDevice_SetRenderTargetFast(target, depth, 0);
		else
			D3DDevice_SetRenderTarget(target, depth);
		g_51f3f4 = target;
		g_51f3f8 = depth;
		g_51f3fc = flag;
	}
	return true;
}

// @retail 0x1c290
dword *function_1c290(real value)
{
	real scaled = value * 16.0f;
	long index;
	__asm
	{
		fld scaled
		fistp index
	}
	if (index < 0)
		index = 0;
	else if (index > 16)
		index = 16;
	return g_487288[index];
}

struct s_01b050_shader_state
{
	byte field_0000[0x1424];
	D3DPIXELSHADERDEF program;
	bool changed;
};

// @retail 0x1b050
void function_1b050(s_01b050_shader_state *state)
{
	D3DDevice_SetPixelShaderProgram(&state->program);
	state->changed = false;
}

// @retail 0x15180
void function_15180(D3DPIXELSHADERDEF const *program)
{
	D3DDevice_SetPixelShaderProgram(program);
}

// @retail 0x151c0
void function_151c0(real const *constants)
{
	D3DDevice_SetVertexShaderConstantFast(-46, constants, 3);
	g_5093d8 = 0;
}

// @retail 0x1ccf0
bool function_1ccf0(D3DPIXELSHADERDEF const *program)
{
	D3DDevice_SetPixelShaderProgram(program);
	return true;
}

// @retail 0x1cf50
void function_1cf50()
{
	IDirect3DBaseTexture8 **wanted = g_51f3c8[1];
	for (long stage = 0; stage < 4; wanted++, stage++)
	{
		IDirect3DBaseTexture8 **current = wanted - 4;
		if (*current != *wanted)
		{
			D3DDevice_SetTexture(stage, *wanted);
			*current = *wanted;
		}
	}
}

// @retail 0x1cf80
void function_1cf80(void)
{
	for (long stage = 0; stage < 4; stage++)
	{
		D3DDevice_SetTexture(stage, g_51f3c8[1][stage]);
		g_51f3c8[0][stage] = g_51f3c8[1][stage];
	}
}

// @retail 0x1c8a0
bool __stdcall function_1c8a0(D3DPRIMITIVETYPE type, word const *indices, long count)
{
	function_1cf50();
	D3DDevice_DrawIndexedVertices(type, count, indices);
	return true;
}

struct s_597d0_object
{
	byte unknown00[0x741c];
	long field_741c;
};

// @retail 0x597d0
bool function_597d0(s_597d0_object **out)
{
	bool result = false;
	long mode = 0;
	if (g_527330.initialized)
		mode = g_527330.state;

	switch (mode)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		result = false;
		if (g_527330.initialized)
		{
			s_597d0_object *object = g_527330.session_a;
			if (object->field_741c)
			{
				if (out)
					*out = object;
				result = true;
			}
		}
		break;
	case 7:
	case 8:
	case 9:
		result = false;
		if (g_527330.initialized)
		{
			s_597d0_object *object = g_527330.session_b;
			if (object->field_741c)
			{
				if (out)
					*out = object;
				result = true;
			}
		}
		break;
	}
	return result;
}


struct s_shader_entry
{
	long field_00;
	dword input_count;
	word *inputs;
	long byte_count;
	dword *program;
	byte field_14[8];
};

struct s_shader_tag
{
	long field_00;
	dword entry_count;
	s_shader_entry *entries;
};

struct s_shader_program
{
	dword const *program;
	long field_04;
	dword byte_count;
	long field_0c;
	real field_10;
};

dword const g_43f2b0[85] =
{
	0x00152078, 0x00000000, 0x0056e000, 0x7c2a1000, 0x2ca00000,
	0x00000000, 0x0056e0aa, 0x7c021000, 0x23a00000, 0x00000000,
	0x00570000, 0x8c2a1000, 0x2cb00000, 0x00000000, 0x0096e615,
	0x38aaf856, 0x9ca00000, 0x00000000, 0x0096e601, 0x39fefaae,
	0x93a00000, 0x00000000, 0x00970615, 0x38ab1856, 0xdcb00000,
	0x00000000, 0x00d6201b, 0x08363800, 0x20b08800, 0x00000000,
	0x00d6401b, 0x08365800, 0x20b04800, 0x00000000, 0x00d6601b,
	0x08367800, 0x20b02800, 0x00000000, 0x00d6801b, 0x08369800,
	0x20b01800, 0x00000000, 0x02575215, 0xa42b586e, 0x6ca0f81c,
	0x00000000, 0x065740ab, 0xa5575bff, 0x13a10000, 0x00000000,
	0x00576015, 0xb42b7800, 0x2cb00000, 0x00000000, 0x00770015,
	0xa40012fe, 0x3ca00000, 0x00000000, 0x007720ab, 0xa4001006,
	0x73a00000, 0x00000000, 0x00772015, 0xb40012fe, 0x7cb00000,
	0x00000000, 0x0041401a, 0xc4355800, 0x20b0e800, 0x00000000,
	0x0056a015, 0xa42ab800, 0x2090c848, 0x00000000, 0x0056c0bf,
	0xa42ad800, 0x20a0c850, 0x00000000, 0x0056c015, 0xb57ed800,
	0x20a0c858, 0x00000000, 0x0081601a, 0xc5fe286a, 0xf0b0e801,
};

s_shader_program g_4670ec[1] =
{
	{ g_43f2b0, NONE, 0x154, NONE, 1.0f }
};

// @retail 0x1c2e0
dword const *function_1c2e0(long tag, long index, long *count)
{
	dword const *result;
	if (tag != NONE)
	{
		s_shader_entry *entry = &((s_shader_tag *)g_4e3b44[tag & 0xffff].bytes)->entries[index];
		result = entry->program;
		if (count)
			*count = entry->byte_count >> 4;
	}
	else
	{
		result = g_4670ec[index].program;
		if (count)
			*count = g_4670ec[index].byte_count >> 4;
	}
	return result;
}

// @retail 0x1cb20
long function_1cb20(long mode, dword index, long tag)
{
	/* The candidate index remains a stack argument in retail. */
	dword const *index_reference = &index;
	switch (mode)
	{
	case 1: return 1;
	case 2: return 2;
	case 3:
		if (*index_reference + 2 < ((s_shader_tag *)g_4e3b44[tag & 0xffff].bytes)->entry_count)
			return *index_reference + 2;
	case 0: return 0;
	default: __assume(0);
	}
}

// @retail 0x1cb70
dword function_1cb70(long tag, long index)
{
	dword result = 0;
	s_shader_entry *entry = &((s_shader_tag *)g_4e3b44[tag & 0xffff].bytes)->entries[index];
	for (dword i = 0; i < entry->input_count; i++)
		result |= 1 << entry->inputs[i];
	return result;
}

struct s_shader_binding
{
	long field_00;
	long tag;
	long index;
	long field_0c;
};

struct s_shader_stream
{
	long field_00;
	long field_04;
	long field_08;
};

struct s_shader_cache
{
	s_shader_binding bindings[8];
	long active;
	long field_84;
	bool field_88;
	byte field_89[3];
	byte const *wanted[16];
	byte const *current[16];
	byte field_10c[0x100];
	bool descriptors_changed;
	byte field_20d[3];
	s_shader_stream streams[16];
	long stream_count;
	bool streams_changed;
	byte field_2d5[3];
};

// @retail 0x1c4a0
void function_1c4a0(s_shader_cache *state)
{
	state->streams_changed = false;
	state->descriptors_changed = false;
	state->stream_count = 0;
	for (long i = 0; i < 8; i++)
	{
		state->bindings[i].field_0c = NONE;
		state->bindings[i].tag = NONE;
		state->bindings[i].index = NONE;
	}
	state->active = NONE;
	for (long j = 0; j < 16; j++)
		state->wanted[j] = NULL;
	for (long k = 0; k < 16; k++)
	{
		state->streams[k].field_08 = 0;
		state->streams[k].field_04 = 0;
		state->streams[k].field_00 = 0;
	}
}

// @retail 0x1c6b0
void function_1c6b0(void *memory)
{
	s_shader_cache *state = (s_shader_cache *)memory;
	state->stream_count = 0;
	state->streams_changed = true;
	state->descriptors_changed = true;
	for (long i = 0; i < 16; i++)
	{
		state->streams[i].field_08 = 0;
		state->streams[i].field_04 = 0;
		state->streams[i].field_00 = 0;
	}
	state->descriptors_changed = true;
	memset(state->current, 0, sizeof(state->current));
	memset(state->wanted, 0, sizeof(state->wanted));
}

// @retail 0x1c590
void function_1c590(s_shader_cache *state, long tag, long index)
{
	long count;
	dword const *program = function_1c2e0(tag, index, &count);
	state->active = 0;
	if (state->bindings[0].tag != tag || state->bindings[0].index != index)
	{
		state->bindings[0].field_00 = 0;
		state->bindings[0].tag = tag;
		state->bindings[0].index = index;
		state->bindings[0].field_0c = 0;
		D3DDevice_LoadVertexShaderProgram(program, 0);
		state->field_84 = 0;
		state->field_88 = true;
		memset(state->current, 0, sizeof(state->current));
		state->descriptors_changed = true;
		for (long i = 0; i < 16; i++)
		{
			state->streams[i].field_08 = 0;
			state->streams[i].field_04 = 0;
			state->streams[i].field_00 = 0;
		}
		state->streams_changed = true;
		state->stream_count = 0;
	}
}

// @retail 0x1c620
void function_1c620(s_shader_cache *state, long a, long b, long c, byte const *descriptor)
{
	long index = state->stream_count;
	if (state->streams[index].field_04 != b || state->streams[index].field_00 != a || state->streams[index].field_08 != c)
	{
		state->streams[index].field_08 = c;
		state->streams[index].field_04 = b;
		state->streams[index].field_00 = a;
		state->streams_changed = true;
	}
	state->wanted[index] = descriptor;
	if (state->current[index] != descriptor)
		state->descriptors_changed = true;
	state->stream_count++;
}

byte g_485af1;

extern byte g_51f0f0[0x2d8];
extern byte *g_485a80;

struct s_shader_slot
{
	dword unknown00;
	long tag;
};

// @retail 0x1cbb0
dword function_1cbb0(long mode, dword index, long tag)
{
	long const *mode_reference = &mode;
	long selected = function_1cb20(*mode_reference, index, tag);
	function_1c590((s_shader_cache *)g_51f0f0, tag, selected);
	return function_1cb70(tag, selected);
}

// @retail 0x1cbe0
dword function_1cbe0(long mode, dword index, long slot_index)
{
	(void)&mode;
	(void)&index;
	s_shader_slot *slot = &(*(s_shader_slot **)(g_485a80 + 0x5c))[slot_index];
	long selected = function_1cb20(mode, index, slot->tag);
	function_1c590((s_shader_cache *)g_51f0f0, slot->tag, selected);
	return function_1cb70(slot->tag, selected);
}

// @retail 0x1cc30
dword function_1cc30(long index)
{
	s_shader_slot *slot = &(*(s_shader_slot **)(g_485a80 + 0x5c))[index];
	function_1c590((s_shader_cache *)g_51f0f0, slot->tag, 0);
	return function_1cb70(slot->tag, 0);
}

// @retail 0x1c7f0
void __stdcall function_1c7f0(long index)
{
	/* The binding index occupies a stack slot in retail. */
	long const *reference = &index;
	s_shader_binding *binding = &((s_shader_cache *)g_51f0f0)->bindings[*reference & 0xffff];
	binding->field_0c = NONE;
	binding->tag = NONE;
	binding->index = NONE;
}

#include "physical_memory.h"

byte g_51f3f0;
s_physical_object *g_487b08;

PRIVATE bool __stdcall shader_block_busy(long index)
{
	return false;
}

// @retail 0x1c810
void function_1c810(void)
{
	g_51f3f0 = false;
	g_51f3f4 = NULL;
	g_51f3f8 = NULL;
	g_51f3fc = false;
	memset(g_51f3c8[1], 0, sizeof(g_51f3c8[1]));
	memset(g_51f3c8[0], 0, sizeof(g_51f3c8[0]));
	g_487b08 = physical_memory_new("vertex shader lruv cache", 0x88, 0, 8,
		function_1c7f0, shader_block_busy, NULL, g_468758);
}



// @retail 0x1e8c0
long __stdcall function_1e8c0(char const *key)
{
	/* This key is passed on the stack by the cache callback interface. */
	char const *const *reference = &key;
	return (*reference)[3] * 59 + (*reference)[2] * 53 + (*reference)[1] * 43 + (*reference)[0] * 17;
}

struct s_cache_key
{
	word key;
	word flags;
};

// @retail 0x1e8f0
long __stdcall function_1e8f0(s_cache_key const *a, s_cache_key const *b)
{
	/* Both pointers are stack arguments in the cache callback interface. */
	s_cache_key const *const *local_6e666f = &a;
	s_cache_key const *const *right_reference = &b;
	word right_flags = (*right_reference)->flags;
	word left_flags = (*local_6e666f)->flags;
	if ((bool)(right_flags & 1) == (bool)(left_flags & 1) && (*local_6e666f)->key == (*right_reference)->key &&
		(short)((left_flags ^ right_flags) & ~1) == 0)
		return 1;
	return 0;
}

struct s_cache_record
{
	long count;
	byte unknown04[8];
	byte active;
	byte unknown0d[0x63];
};

struct s_cache_record_state
{
	long count;
	dword unknown04;
	s_cache_record *records;
	void *buffer;
	byte unknown10;
	bool available;
	byte unknown12[2];
};

s_cache_record_state g_4b6280;

struct hash_table;
extern hash_table *g_51f400;

struct hash_node;
hash_node *function_13e2d0(hash_table *table, void *key);

struct s_cache_lookup_key
{
	short index;
	word flag : 1;
	word part : 15;
	byte unknown04[16];
};

// @retail 0x1e280
void *function_1e280(short index, bool flag, long part)
{
	(void)&part;
	s_cache_lookup_key key;
	key.index = index;
	key.flag = flag;
	key.part = (word)part;
	hash_node *node = function_13e2d0(g_51f400, &key);
	if (node && *(byte **)node)
		return *(byte **)node + 4;
	return 0;
}

struct s_cache_hash_view
{
	byte unknown00[0x34];
	c_data_allocator *allocator;
};

// @retail 0x1e310
void function_1e310(void)
{
	if (g_4b6280.records)
	{
		if (!VirtualFree(g_4b6280.records, 0, MEM_RELEASE)) GetLastError();
		if (!VirtualFree(g_4b6280.buffer, 0, MEM_RELEASE)) GetLastError();
	}
	((s_cache_hash_view *)g_51f400)->allocator->deallocate(g_51f400);
	g_51f400 = 0;
}

// @retail 0x1e2d0
s_cache_record *function_1e2d0(void)
{
	s_cache_record *result = 0;
	if (g_4b6280.count < 1024)
	{
		result = &g_4b6280.records[g_4b6280.count];
		result->count = 0;
		g_4b6280.count++;
		result->active = 0;
	}
	else if (g_4b6280.available)
		g_4b6280.available = false;
	return result;
}

// @retail 0x1cdd0
void function_1cdd0(real const *bounds, byte flags)
{
	/* The reference retains the flag argument's retail stack placement. */
	byte const *flags_reference = &flags;
	real constants[12];
	constants[0] = 1.0f;
	constants[1] = 1.0f;
	constants[2] = 1.0f;
	constants[3] = 1.0f;
	constants[4] = 0.0f;
	constants[5] = 0.0f;
	constants[6] = 0.0f;
	constants[7] = 0.0f;
	constants[8] = 1.0f;
	constants[9] = 1.0f;
	constants[10] = 0.0f;
	constants[11] = 0.0f;
	bool active = false;
	if ((*flags_reference & 3) && bounds)
	{
		active = true;
		if (*flags_reference & 1)
		{
			constants[0] = (bounds[1] - bounds[0]) * 0.5f;
			constants[1] = (bounds[3] - bounds[2]) * 0.5f;
			constants[2] = (bounds[5] - bounds[4]) * 0.5f;
			constants[3] = 1.0f;
			constants[4] = (bounds[1] + bounds[0]) * 0.5f;
			constants[5] = (bounds[2] + bounds[3]) * 0.5f;
			constants[6] = (bounds[4] + bounds[5]) * 0.5f;
			constants[7] = 0.0f;
		}
		if (*flags_reference & 2)
		{
			constants[8] = (bounds[7] - bounds[6]) * 0.5f;
			constants[9] = (bounds[9] - bounds[8]) * 0.5f;
			constants[10] = (bounds[6] + bounds[7]) * 0.5f;
			constants[11] = (bounds[9] + bounds[8]) * 0.5f;
		}
	}
	if (active || g_485af1)
	{
		D3DDevice_SetVertexShaderConstantFast(74, constants, 3);
		g_485af1 = active;
	}
}

byte const g_43f408[63][21] =
{
	{ 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x01, 0x00, 0x02, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x02, 0x00, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x03, 0x00, 0x02, 0x01, 0x04, 0xfe, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x04, 0x00, 0x0e, 0x01, 0x04, 0xfe, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x05, 0x00, 0x02, 0x01, 0x05, 0x02, 0x05, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x06, 0x00, 0x0e, 0xfe, 0x02, 0x01, 0x05, 0x02, 0x05, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x07, 0x00, 0x02, 0x01, 0x06, 0xfe, 0x01, 0x02, 0x06, 0xfe, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x08, 0x00, 0x0e, 0x01, 0x06, 0x02, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x09, 0x00, 0x02, 0x01, 0x07, 0x02, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0a, 0x00, 0x0e, 0xfe, 0x02, 0x01, 0x07, 0x02, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0b, 0x01, 0x04, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0c, 0x01, 0x05, 0x02, 0x05, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0d, 0x01, 0x06, 0x02, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0e, 0x01, 0x07, 0x02, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0f, 0x0a, 0x02, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x10, 0x0a, 0x0e, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x11, 0x0a, 0x02, 0x0b, 0x04, 0xfe, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x12, 0x0a, 0x0e, 0x0b, 0x04, 0xfe, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x13, 0x0d, 0x04, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x14, 0x00, 0x02, 0xfe, 0x04, 0x10, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x15, 0x00, 0x0e, 0xfe, 0x02, 0x10, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x16, 0x00, 0x02, 0x01, 0x04, 0xfe, 0x03, 0x10, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x17, 0x00, 0x0e, 0x01, 0x04, 0xfe, 0x01, 0x10, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x18, 0x03, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x19, 0x03, 0x0d, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x1a, 0x04, 0x02, 0x05, 0x02, 0x06, 0x02, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x1b, 0x04, 0x10, 0x05, 0x10, 0x06, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x1c, 0x07, 0x02, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x1d, 0x07, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x1e, 0x09, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x1f, 0x09, 0x0d, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x20, 0x03, 0x01, 0x04, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x21, 0x03, 0x0d, 0x04, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x22, 0x03, 0x01, 0x04, 0x10, 0x05, 0x10, 0x06, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x23, 0x03, 0x0d, 0x04, 0x10, 0x05, 0x10, 0x06, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x24, 0x00, 0x01, 0x03, 0x01, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x25, 0x00, 0x03, 0x03, 0x01, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x26, 0x00, 0x03, 0x01, 0x01, 0x02, 0x01, 0x03, 0x01, 0x04, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x27, 0x00, 0x02, 0x03, 0x0d, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x28, 0x00, 0x02, 0x03, 0x01, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x29, 0x00, 0x02, 0x03, 0x02, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x2a, 0x00, 0x03, 0x01, 0x02, 0x02, 0x00, 0x03, 0x03, 0x04, 0x02, 0x05, 0x01, 0x06, 0x03, 0x07, 0x03, 0x09, 0x11, 0xff, 0x00 },
	{ 0x2b, 0x00, 0x0b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x2c, 0x00, 0x02, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x2d, 0x00, 0x02, 0x03, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x2e, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x2f, 0x0e, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x30, 0x08, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x31, 0x00, 0x03, 0x03, 0x01, 0x0e, 0x11, 0x0f, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x32, 0x00, 0x02, 0x04, 0x02, 0x06, 0x02, 0x05, 0x02, 0x03, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x33, 0x00, 0x02, 0x04, 0x10, 0x06, 0x10, 0x05, 0x10, 0x03, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x34, 0x00, 0x02, 0x03, 0x01, 0x09, 0x01, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x35, 0x00, 0x02, 0x03, 0x0d, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x36, 0x00, 0x02, 0x11, 0x02, 0x12, 0x02, 0x03, 0x01, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x37, 0x00, 0x02, 0x11, 0x10, 0x12, 0x10, 0x03, 0x0d, 0x0e, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x38, 0x13, 0x00, 0x14, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x39, 0x13, 0x08, 0x14, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3a, 0x00, 0x01, 0x01, 0x01, 0x05, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3b, 0x00, 0x02, 0x04, 0x02, 0x06, 0x02, 0x05, 0x02, 0x03, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3c, 0x00, 0x02, 0x04, 0x02, 0x06, 0x02, 0x05, 0x02, 0x03, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3d, 0x00, 0x02, 0x04, 0x10, 0x06, 0x10, 0x05, 0x10, 0x03, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3e, 0x00, 0x03, 0x03, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
};


struct s_stream_description
{
    byte format;
    byte stride;
    byte unknown02[6];
    long offset;
    long unknown0c;
    long buffer;
};

// @retail 0x1cda0
void function_1cda0(s_stream_description const *stream)
{
    function_1c620((s_shader_cache *)g_51f0f0, stream->buffer, stream->stride,
        stream->offset, g_43f408[stream->format]);
}

extern word g_485ac0;
extern byte g_485ac2;
byte g_485ac3, g_485ac4, g_485ac5;
bool g_485ac6;

// @retail 0x123b0
void function_123b0(void)
{
    dword standard = XGetVideoStandard();
    dword flags = XGetVideoFlags();
    if (standard == 3)
        g_485ac0 = (byte)flags & 0x40 ? 60 : 50;
    byte wide = (byte)((flags >> 4) & 1);
    byte low = (byte)(flags & 1);
    flags &= 8;
    g_485ac2 = low;
    g_485ac3 = wide;
    g_485ac6 = (long)flags != 0;
    g_485ac5 = flags ? 1 : 0;
    g_485ac4 = standard == 3;
}

long g_467004 = NONE;

// @retail 0x13cd0
void function_13cd0(void)
{
    g_467004 = NONE;
    memset(g_51f3c8[1], 0, sizeof(g_51f3c8[1]));
    function_1cf50();
    D3DDevice_SetIndices(0, 0);
    D3DDevice_SetPixelShader(0);
}

// @retail 0x14ac0
void function_14ac0(void)
{
    D3DVIEWPORT8 viewport;
    D3DVIEWPORT8 saved;
    viewport.X = 0;
    viewport.Y = 0;
    viewport.Width = 640;
    viewport.Height = 480;
    viewport.MinZ = 0.0f;
    viewport.MaxZ = 1.0f;
    D3DDevice_GetViewport(&saved);
    D3DDevice_SetViewport(&viewport);
    D3DDevice_Clear(0, 0, 0xf0, 0, 0.0f, 0);
    D3DDevice_SetViewport(&saved);
}

long g_4858b8;
real g_485adc, g_485ae0;

extern byte *g_50934c;
extern double g_4858a0;

// @retail 0x137a0
void __stdcall function_137a0(real *first, real *second)
{
    real now = (real)g_4858a0;
    real *times;
    real *a;
    real *b;
    if (*g_50934c)
    {
        times = (real *)(g_50934c + 0x1c);
        a = (real *)(g_50934c + 0x24);
        b = (real *)(g_50934c + 0x2c);
    }
    else
    {
        times = (real *)(g_50934c + 4);
        a = (real *)(g_50934c + 0xc);
        b = (real *)(g_50934c + 0x14);
    }
    real x, y;
    if (now <= times[0])
    {
        x = a[0];
        y = b[0];
    }
    else if (!(now > times[1]) && times[1] - times[0] > 0.0001f)
    {
        real fraction = (now - times[0]) / (times[1] - times[0]);
        x = (a[1] - a[0]) * fraction + a[0];
        y = (b[1] - b[0]) * fraction + b[0];
    }
    else
    {
        x = a[1];
        y = b[1];
    }
    *first = x;
    *second = y;
}

// @retail 0x15720
void __stdcall function_15720(real x, real y)
{
    (void)&x;
    (void)&y;
    x = g_485adc;
    y = g_485ae0;
    real const *near_plane = &x;
    real const *far_plane = &y;
    D3DDevice_SetDepthClipPlanes(*near_plane, *far_plane, D3DSDCP_SET_VERTEXPROGRAM_PLANES);
}

byte g_4b6290;
extern dword g_4850c8;

// @retail 0x13630
bool function_13630(void)
{
    g_4b6290 = false;
    if (g_4850c8)
    {
        g_485adc = 0.0f;
        g_4858b8 = 0;
        g_485ae0 = 16777215.0f;
        function_14bc0(0, 0, true);
        function_15720(0.0f, 0.0f);
    }
    return true;
}

byte g_4858bc;
dword g_4b843c;

// @retail 0x4a780
void function_4a780(void)
{
    g_4858bc = false;
    g_4b843c = 0;
    D3DDevice_SetRenderState(D3DRS_STENCILENABLE, 0);
    D3DDevice_SetScissors(0, FALSE, 0);
    function_15720(0.0f, 0.0f);
    function_14bc0((short)g_4858b8, 0, true);
}

byte g_485b48[0x1fc0];
byte g_4670bc = true;
struct s_render_reset_state;
void function_16b10(s_render_reset_state *state);

// @retail 0x1bbd0
void function_1bbd0(void)
{
    g_4670bc = true;
    function_16b10((s_render_reset_state *)g_485b48);
}

typedef void (__stdcall *t_render_pair_callback)(long, long);
struct s_render_pair_request
{
    long first;
    long second;
    t_render_pair_callback callback;
};

// @retail 0x48e40
void __stdcall function_48e40(s_render_pair_request const *request)
{
    (void)&request;
    g_4670bc = true;
    function_16b10((s_render_reset_state *)g_485b48);
    request->callback(request->first, request->second);
}

struct s_frame_offset
{
    point3f position;
    vector3f forward;
    vector3f up;
};
extern s_frame_offset g_485618;
byte g_55e6c8;

// @retail 0x47870
void function_47870(long first, long second, point3f const *position, t_render_pair_callback callback)
{
    (void)&first;
    (void)&second;
    if (callback)
    {
        s_cache_record *record = function_1e2d0();
        if (record)
        {
            real x = position->x - g_485618.position.x;
            real y = position->y - g_485618.position.y;
            real z = position->z - g_485618.position.z;
            record->count = 3;
            *(real *)record->unknown04 = 0.0f - (g_485618.forward.k * z + g_485618.forward.j * y + g_485618.forward.i * x);
            *(void (__stdcall **)(s_render_pair_request const *))((byte *)record + 0x20) = function_48e40;
            *(point3f *)((byte *)record + 0x64) = *position;
            s_render_pair_request *request = (s_render_pair_request *)((byte *)record + 0x24);
            request->first = first;
            request->second = second;
            request->callback = callback;
        }
        else if (!g_55e6c8)
            g_55e6c8 = true;
    }
}


struct s_shader_constant_state
{
    byte unknown00[0x1630];
    real constants[16][4];
    long unknown1730;
    dword changed;
};

// @retail 0x18e80
void function_18e80(s_shader_constant_state *state)
{
    dword remaining = state->changed;
    while (remaining)
    {
        long index;
        __asm
        {
            bsf ecx, remaining
            mov index, ecx
        }
        D3DDevice_SetVertexShaderConstantFast(index - 78, state->constants[index], 1);
        remaining &= ~(1 << index);
        state->changed &= ~(1 << index);
    }
}

extern s_record_pool *g_509434;
extern long g_4c1bd0;

// @retail 0x3d270
void function_3d270(void)
{
	g_509434 = data_new_inlined("cached object render states", 256, 256, 0, g_510c2c);
	g_4c1bd0 = NONE;
}

typedef dword (__stdcall *t_cache_hash)(void const *);
typedef bool (__stdcall *t_cache_compare)(void const *, void const *);
hash_table *function_13e1a0(char const *name, long data_size, long bucket_count,
    t_cache_hash hash, t_cache_compare compare, long maximum_count, c_data_allocator *allocator);

// @retail 0x1e110
bool function_1e110(void)
{
    volatile bool result = true;
    void *buffer = VirtualAlloc(0, 0x1c000, 0x101000, PAGE_READWRITE);
    if (!buffer) GetLastError();
    g_4b6280.records = (s_cache_record *)buffer;
    buffer = VirtualAlloc(0, 0x800, 0x101000, PAGE_READWRITE);
    if (!buffer) GetLastError();
    g_4b6280.buffer = buffer;
    g_4b6280.available = true;
    if (!g_4b6280.records)
        result = false;
    g_4b6280.count = 0;
    g_4b6280.unknown04 = 0;
    g_4b6280.unknown10 = 0;
    g_51f400 = function_13e1a0("transparent planes", 16, 4096,
        (t_cache_hash)function_1e8c0, (t_cache_compare)function_1e8f0, 2048, g_468758);
    return result;
}

bool function_13e270(hash_table *table, void *key, void const *data);

struct s_cache_source_entries
{
    byte unknown00[0x158];
    long count;
    s_cache_lookup_key *entries;
};

// @retail 0x1e1c0
void function_1e1c0(void)
{
    s_cache_source_entries *source = (s_cache_source_entries *)g_4e0348;
    if (g_51f400)
    {
        for (long i = 0; i < source->count; ++i)
        {
            s_cache_lookup_key *key = &source->entries[i];
            function_13e270(g_51f400, key, key->unknown04);
        }
    }
}

struct s_shader_transform_row
{
    real x, y, z;
    long unused;
};

struct s_shader_transform_entry
{
    short first;
    word count;
};

struct s_shader_transform_table
{
    long unknown00;
    s_shader_transform_entry entries[16];
    s_shader_transform_row transforms[1][3];
};

s_shader_transform_table *g_485a5c;
extern byte g_485af0;

// @retail 0x151e0
void function_151e0(long index)
{
    if (index != NONE)
    {
        s_shader_transform_table *data = g_485a5c;
        s_shader_transform_row *transform = data->transforms[data->entries[index].first];
        long count = data->entries[index].count * 3;
        D3DDevice_SetVertexShaderConstantFast(-46, transform, count);
        g_485af0 = true;
    }
    else
    {
        s_shader_transform_row transform[3];
        transform[0].x = 1.0f;
        transform[0].y = 0.0f;
        transform[0].z = 0.0f;
        transform[0].unused = 0;
        transform[1].x = 0.0f;
        transform[1].y = 1.0f;
        transform[1].z = 0.0f;
        transform[1].unused = 0;
        transform[2].x = 0.0f;
        transform[2].y = 0.0f;
        transform[2].z = 1.0f;
        transform[2].unused = 0;
        D3DDevice_SetVertexShaderConstantFast(-46, transform, 3);
    }
}


struct s_frame_offset
{
	point3f position;
	vector3f forward;
	vector3f up;
};
extern s_frame_offset g_485618;

typedef void (__stdcall *t_1e4e0_callback)(void *);

struct s_1e4e0_record
{
	long type;
	real depth;
	long flags;
	byte active;
	byte unknown0d[0x13];
	void (__stdcall *callback)(void *);
	byte payload[0x40];
	point3f position;
};

// @retail 0x1e4e0
bool function_1e4e0(point3f const *position, t_1e4e0_callback callback, void const *data, long size)
{
	(void)&callback;
	(void)&data;
	bool result = false;
	if (g_4b6280.count < 1024)
	{
		g_4b6280.records[g_4b6280.count].count = 0;
		s_1e4e0_record *record = (s_1e4e0_record *)&g_4b6280.records[g_4b6280.count];
		record->active = false;
		vector3f delta;
		delta.i = position->x - g_485618.position.x;
		delta.j = position->y - g_485618.position.y;
		delta.k = position->z - g_485618.position.z;
		++g_4b6280.count;
		if (size)
			memcpy(record->payload, data, size);
		record->type = 3;
		record->flags = 0;
		record->callback = callback;
		real depth = g_485618.forward.k * delta.k;
		depth += g_485618.forward.j * delta.j;
		depth += g_485618.forward.i * delta.i;
		record->depth = 0.0f - depth;
		record->position = *position;
		result = true;
	}
	else if (g_4b6280.available)
		g_4b6280.available = false;
	return result;
}


DWORD const g_43fdc0[5] = {1, 2, 5, 8, 6};
bool g_47fe85 = true;
typedef void (__stdcall *t_4b220_fill)(void *, long, void *);

// @retail 0x4b220
long function_4b220(long mode, long primitive, long stride, t_4b220_fill fill, void *context, long count)
{
	long result = NONE;
	long const *mode_reference = &mode;
	(void)&primitive;
	(void)&stride;
	(void)&fill;
	(void)&context;
	if (!*mode_reference)
	{
		long bytes = count * stride;
		dword words = (dword)bytes >> 2;
		long requested = words + 5;
		if (requested < 2048)
		{
			function_1cf50();
			DWORD *push = D3DDevice_BeginPush(requested);
			*push++ = D3DPUSH_ENCODE(D3DPUSH_SET_BEGIN_END, 1);
			*push++ = g_43fdc0[primitive];
			*push++ = D3DPUSH_ENCODE(D3DPUSH_INLINE_ARRAY | D3DPUSH_NOINCREMENT_FLAG, words);
			fill(push, bytes, context);
			push += words;
			*push++ = D3DPUSH_ENCODE(D3DPUSH_SET_BEGIN_END, 1);
			*push++ = 0;
			D3DDevice_EndPush(push);
		}
		else if (g_47fe85)
			g_47fe85 = false;
	}
	return result;
}

real *table_entry_data(long handle);

// @retail 0x152a0
void function_152a0(long handle, long index)
{
    if (index != NONE)
    {
        s_shader_transform_table *data = (s_shader_transform_table *)table_entry_data(handle);
        s_shader_transform_row *transform = data->transforms[data->entries[index].first];
        long count = data->entries[index].count * 3;
        D3DDevice_SetVertexShaderConstantFast(-46, transform, count);
        g_485af0 = true;
    }
    else
    {
        s_shader_transform_row transform[3];
        transform[0].x = 1.0f;
        transform[0].y = 0.0f;
        transform[0].z = 0.0f;
        transform[0].unused = 0;
        transform[1].x = 0.0f;
        transform[1].y = 1.0f;
        transform[1].z = 0.0f;
        transform[1].unused = 0;
        transform[2].x = 0.0f;
        transform[2].y = 0.0f;
        transform[2].z = 1.0f;
        transform[2].unused = 0;
        D3DDevice_SetVertexShaderConstantFast(-46, transform, 3);
    }
}

real g_4b9fa4, g_4b9ff8, g_4b9f18, g_4b9f9c;

PRIVATE __forceinline real maximum_25ca0(real first, real second)
{
    real result = second;
    if (first > second)
        result = first;
    return result;
}

// @retail 0x25ca0
void function_25ca0(bool enabled)
{
    real divisor = maximum_25ca0(0.0001f, g_4b9fa4);
    real value = g_4b9ff8;
    value *= 1.0f / divisor;
    value = 0.0f - value;
    real constants[4];
    constants[0] = value < 0.0f ? 0.0f : value > 1.0f ? 1.0f : value;
    constants[1] = 0.0f;
    constants[2] = g_4b9f18;
    if (enabled)
    {
        constants[3] = g_4b9f9c;
        D3DDevice_SetVertexShaderConstant(-81, constants, 1);
    }
    else
    {
        constants[3] = 0.0f;
        D3DDevice_SetVertexShaderConstant(-81, constants, 1);
    }
}
