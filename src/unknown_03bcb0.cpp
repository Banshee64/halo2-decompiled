// @flags /O2 /arch:SSE /Ob1 /Gr
/* UNKNOWN_03BCB0.CPP: predicting one bitmap's texture (unknown_03bcb0.h);
   its own /Ob1 file because retail calls it out of line from 0x16e5e0 */

#include "unknown_11c920.h"
#include "unknown_03bcb0.h"
#include <xtl.h>
#include "globals.h"
#include "geometry_cache.h"

struct s_3c9a0_matrix
{
	real scale;
	vector3f forward, left, up;
	point3f position;
};

real function_30bf0(vector3f *vector);

PRIVATE inline void matrix_cross(vector3f const *a, vector3f const *b, vector3f *out)
{
	real i = b->k * a->j - a->k * b->j;
	real j = a->k * b->i - b->k * a->i;
	real k = b->j * a->i - b->i * a->j;
	out->i = i;
	out->j = j;
	out->k = k;
}

// @retail 0x3c9a0
void function_3c9a0(vector3f const *forward, vector3f const *up, s_3c9a0_matrix *matrix)
{
	matrix->scale = 1.0f;
	matrix->up = *up;
	matrix_cross(up, forward, &matrix->left);
	function_30bf0(&matrix->left);
	matrix_cross(&matrix->left, up, &matrix->forward);
	function_30bf0(&matrix->forward);
	matrix->position.x = 0.0f;
	matrix->position.y = 0.0f;
	matrix->position.z = 0.0f;
}

// @retail 0x3bcb0
void function_3bcb0(s_bitmap_data *bitmap)
{
	bitmap_predict_inline((s_bitmap_predict_view *)bitmap, 0xe);
}

struct s_sort_record
{
	word index;
	word key;
	byte unknown04;
	byte group;
	word subkey;
	word value08;
	word unknown0a;
	dword value0c;
	dword value10;
	dword unknown14;
};

struct s_record_source
{
	bool (__stdcall *fill)(long, void *, long, long, long, void *, s_sort_record *);
	dword unknown04;
	long key;
	void *context;
	dword flags;
	byte value14;
	byte value15;
	word unknown16;
};

struct s_record_sources
{
	dword unknown00;
	long count;
	dword flags;
	long first;
	dword unknown10;
	s_record_source *sources;
};

class c_type_4e7709;
c_type_4e7709 *function_137bd0(long tag_index);

class c_record_reference_view
{
public:
	virtual void slot0() = 0;
	virtual void slot1() = 0;
	virtual void slot2() = 0;
	virtual void slot3() = 0;
	virtual void slot4() = 0;
	virtual void slot5() = 0;
	virtual void slot6() = 0;
	virtual void slot7() = 0;
	virtual void slot8() = 0;
	virtual void slot9() = 0;
	virtual long *reference() = 0;
};

PRIVATE __forceinline byte *record_format_groups(long tag_index)
{
	long *reference;
	dword kind = *(dword *)g_4e3b44[(short)tag_index].unknown00;
	if (kind == 0x5052544d || kind == 0x70727433)
	{
		c_record_reference_view *provider = (c_record_reference_view *)function_137bd0(tag_index);
		reference = provider->reference();
	}
	else
		reference = *(long **)(g_4e3b44[tag_index & 0xffff].bytes + 0x24);
	return *(byte **)(g_4e3b44[*reference & 0xffff].bytes + 0x5c);
}

long function_30cd0(bool a, bool b);
long function_30d10(bool a, bool b, bool c);
long function_30da0(bool a, bool b);
long function_30e00(bool a);
bool function_0226d0(void);

// @retail 0x15d70
long function_15d70(long tag, long stage, long pass, bool first, bool second)
{
	(void)&pass; (void)&first; (void)&second;
	byte *definition = g_4e3b44[tag & 0xffff].bytes;
	long result = 0;
	byte *groups = record_format_groups(tag);
	long group = *(word *)(*(byte **)(groups + 4) + pass * 10) & 0x1ff;
	word range = (*(word **)(groups + 0xc))[group + stage];
	if (range >> 9)
	{
		switch (stage)
		{
		case 3: result = function_0226d0() ? NONE : 3; break;
		case 10: result = function_30cd0(second, first); break;
		case 11: result = function_30d10(*(word *)(definition + 0x3e) != 0, true, second); break;
		case 12: result = function_30da0(*(word *)(definition + 0x3e) != 0, true); break;
		case 13: result = function_30e00(*(word *)(definition + 0x3e) != 0); break;
		}
	}
	else
		result = NONE;
	return result;
}

long function_4cbb0(long tag_index, dword block_index);

PRIVATE __forceinline byte *record_material(long tag, long pass, long stage, long entry)
{
	byte *groups = record_format_groups(tag);
	long group = *(word *)(*(byte **)(groups + 4) + pass * 10) & 0x1ff;
	long first = (*(word **)(groups + 0xc))[group + stage] & 0x1ff;
	long material = *(long *)(*(byte **)(groups + 0x14) + (first + entry) * 10 + 4);
	return *(byte **)(*(byte **)(g_4e3b44[material & 0xffff].bytes + 0x20) + 4);
}

// @retail 0x4dfa0
bool __stdcall function_4dfa0(long tag, long context, long pass, long stage, long entry, long handle, s_sort_record *out)
{
	(void)&tag; (void)&context; (void)&pass; (void)&stage;
	(void)&entry; (void)&handle; (void)&out;
	byte *table = *(byte **)((byte *)g_4e0348 + 0x238);
	byte *record = *(byte **)(table + 0x1c) + (((dword)handle >> 8) & 0x3fffff) * 24;
	s_geometry_block_info *block = (s_geometry_block_info *)(*(byte **)(table + 0x14) + *(short *)(record + 6) * 0x2c);
	if (function_12de70(block, 3) && *(word *)(record + 0xe) > 0)
	{
		out->unknown04 = 0;
		out->group = 4;
		out->subkey = 0xffff;
		out->value0c = handle;
		byte *material = record_material(tag, pass, stage, entry);
		out->value10 = (dword)material;
		out->value08 = 0;
		out->unknown14 = 0;
		return function_4cbb0(*(long *)(material + 0x100), 0) != 0;
	}
	return false;
}

// @retail 0x4de20
bool __stdcall function_4de20(long tag, long context, long pass, long stage, long entry, long handle, s_sort_record *out)
{
	(void)&tag; (void)&context; (void)&pass; (void)&stage;
	(void)&entry; (void)&handle; (void)&out;
	out->unknown04 = 0;
	if (stage == 1) return false;
	byte *table = *(byte **)((byte *)g_4e0348 + 0x238);
	byte *record = *(byte **)(table + 0x1c) + (((dword)handle >> 8) & 0x3fffff) * 24;
	long record_tag = (*(long **)((byte *)g_4e0350 + 0x37c))[(signed char)record[0] * 2 + 1];
	byte *definition = g_4e3b44[record_tag & 0xffff].bytes;
	s_geometry_block_info *block = (s_geometry_block_info *)(*(byte **)(table + 0x14) + *(short *)(record + 6) * 0x2c);
	if (function_12de70((s_geometry_block_info *)(definition + 0x38), 3) && function_12de70(block, 3))
	{
		byte index = out->unknown04;
		out->group = 1;
		out->subkey = 0xffff;
		out->value0c = handle;
		byte *material = record_material(tag, pass, stage, entry) + index * 0x132;
		out->value10 = (dword)material;
		out->value08 = 1;
		out->unknown14 = 0;
		return function_4cbb0(*(long *)(material + 0x100), 1) != 0;
	}
	return false;
}

// @retail 0x4e0d0
bool __stdcall function_4e0d0(long tag, long context, long pass, long stage, long entry, long handle, s_sort_record *out)
{
	(void)&tag; (void)&context; (void)&pass; (void)&stage;
	(void)&entry; (void)&handle; (void)&out;
	byte *table = *(byte **)((byte *)g_4e0348 + 0x238);
	byte *blocks = *(byte **)(table + 0x14);
	byte *record = *(byte **)(table + 0x1c) + (((dword)handle >> 8) & 0x3fffff) * 24;
	long record_tag = (*(long **)((byte *)g_4e0350 + 0x37c))[(signed char)record[0] * 2 + 1];
	byte *definition = g_4e3b44[record_tag & 0xffff].bytes;
	byte *part = *(byte **)(definition + 0x14) + record[1] * 20;
	if (function_12de70((s_geometry_block_info *)(blocks + *(short *)(record + 6) * 0x2c), 3) && *(word *)(record + 0xe) > 0)
	{
		out->unknown04 = 0;
		out->group = 4;
		out->subkey = 0xffff;
		out->value0c = handle;
		byte *material = record_material(tag, pass, stage, entry);
		out->value10 = (dword)material;
		switch (part[4])
		{
		case 3: out->value08 = 7; break;
		case 4: out->value08 = 8; break;
		default: out->value08 = 0; break;
		}
		out->unknown14 = 0;
		return function_4cbb0(*(long *)(material + 0x100), out->value08) != 0;
	}
	return false;
}

typedef bool (__stdcall *t_record_fill)(long, void *, long, long, long, void *, s_sort_record *);

// @retail 0x3b9d0
bool function_3b9d0(s_record_sources *data, long tag_index, long pass, dword and_mask, dword or_mask,
	t_record_fill fill, dword value04, void *context, byte value14)
{
	bool result = false;
	if (data->count < (long)data->unknown00)
	{
		byte *groups = record_format_groups(tag_index);
		dword flags = *(dword *)(*(byte **)(groups + 4) + pass * 10 + 2);
		s_record_source *source = &data->sources[data->count++];
		source->fill = fill;
		source->unknown04 = value04;
		source->context = context;
		source->key = tag_index;
		source->value14 = value14;
		source->value15 = (byte)pass;
		source->flags = (flags & and_mask) | or_mask;
		data->flags |= source->flags;
		result = true;
	}
	return result;
}

s_record_sources g_4c1a48[3];

// @retail 0x3b8a0
void function_3b8a0(void)
{
	g_4c1a48[0].unknown00 = 0xa00;
	g_4c1a48[0].count = 0;
	void *buffer = VirtualAlloc(0, 0xf000, 0x101000, PAGE_READWRITE);
	if (!buffer) GetLastError();
	g_4c1a48[0].sources = (s_record_source *)buffer;
	g_4c1a48[1].unknown00 = 0x400;
	g_4c1a48[1].count = 0;
	buffer = VirtualAlloc(0, 0x6000, 0x101000, PAGE_READWRITE);
	if (!buffer) GetLastError();
	g_4c1a48[1].sources = (s_record_source *)buffer;
	g_4c1a48[2].unknown00 = 0x200;
	g_4c1a48[2].count = 0;
	buffer = VirtualAlloc(0, 0x3000, 0x101000, PAGE_READWRITE);
	if (!buffer) GetLastError();
	g_4c1a48[2].sources = (s_record_source *)buffer;
}

// @retail 0x3b950
void function_3b950(void)
{
	if (g_4c1a48[0].sources)
	{
		if (!VirtualFree(g_4c1a48[0].sources, 0, MEM_RELEASE)) GetLastError();
		g_4c1a48[0].sources = 0;
	}
	if (g_4c1a48[1].sources)
	{
		if (!VirtualFree(g_4c1a48[1].sources, 0, MEM_RELEASE)) GetLastError();
		g_4c1a48[1].sources = 0;
	}
	if (g_4c1a48[2].sources)
	{
		if (!VirtualFree(g_4c1a48[2].sources, 0, MEM_RELEASE)) GetLastError();
		g_4c1a48[2].sources = 0;
	}
}

// @retail 0x3ba90
long function_3ba90(long bit, s_record_sources const *data)
{
	long result;
	/* Retail keeps the source-table pointer on the stack. */
	s_record_sources const *const *reference = &data;
	result = 1 << bit;
	result &= (*reference)->flags;
	return !result;
}

// @retail 0x3bb80
bool __stdcall function_3bb80(void const *a, void const *b, void const *context)
{
	s_sort_record const *left = (s_sort_record const *)a;
	s_sort_record const *right = (s_sort_record const *)b;
	if (left->value10 > right->value10) return true;
	if (left->value10 < right->value10) return false;
	if (left->key > right->key) return true;
	if (left->key < right->key) return false;
	if (left->group > right->group) return true;
	if (left->group < right->group) return false;
	if (left->value08 > right->value08) return true;
	if (left->value08 < right->value08) return false;
	return false;
}

// @retail 0x3bbd0
bool __stdcall function_3bbd0(void const *a, void const *b, void const *context)
{
	s_sort_record const *left = (s_sort_record const *)a;
	s_sort_record const *right = (s_sort_record const *)b;
	if (left->group > right->group) return true;
	if (left->group < right->group) return false;
	if (left->subkey > right->subkey) return true;
	if (left->subkey < right->subkey) return false;
	if (left->value10 > right->value10) return true;
	if (left->value10 < right->value10) return false;
	if (left->key > right->key) return true;
	if (left->key < right->key) return false;
	if (left->value0c > right->value0c) return true;
	if (left->value0c < right->value0c) return false;
	if (left->index > right->index) return true;
	if (left->index < right->index) return false;
	return false;
}

typedef bool (__stdcall *t_record_compare)(void const *, void const *, void const *);
typedef void (__stdcall *t_record_draw)(long, void *, long, long, long, void *, s_sort_record *);
void function_13da70(void *elements, unsigned long count, unsigned long element_size, t_record_compare compare, void const *context);

// @retail 0x3bab0
long function_3bab0(s_record_sources *data, void *context, long mode, s_sort_record *out)
{
	/* All four arguments occupy stack slots in retail. */
	(void)&data;
	(void)&context;
	(void)&mode;
	(void)&out;
	long count = 0;
	for (long i = data->first; i < data->count; ++i)
	{
		s_record_source *source = &data->sources[i];
		if (source->flags & (1 << mode))
		{
			long key = source->key;
			long value15 = source->value15;
			s_sort_record *record = &out[count++];
			record->index = (word)i;
			record->key = (word)source->key;
			if (!source->fill(key, context, value15, mode, source->value14, source->context, record))
				--count;
		}
	}
	t_record_compare compare = function_3bb80;
	if (mode == 5)
		compare = function_3bbd0;
	function_13da70(out, count, sizeof(s_sort_record), compare, data);
	return count;
}

// @retail 0x3bc30
void function_3bc30(void *context, s_record_sources *data, long mode)
{
    void *const *context_reference = &context;
    s_sort_record records[2560];
    long count = function_3bab0(data, *context_reference, mode, records);
    if (count > 0)
    {
        s_sort_record *record = records;
        volatile long remaining = count;
        do
        {
            s_record_source *source = &data->sources[record->index];
            if (*(long *)((byte *)record->value10 + 0x100) != NONE)
            {
                t_record_draw callback = (t_record_draw)source->unknown04;
                callback(source->key, *context_reference, source->value15, mode,
                    source->value14, source->context, record);
            }
            count = remaining;
            ++record;
            remaining = --count;
        } while (count);
    }
}


#include "flexible_surface_calls.h"

struct s_40f60_source
{
	long tag;
	long context;
};

// @retail 0x40f60
void function_40f60(void *submission, surface_render_test fill, surface_render_draw submit)
{
	s_40f60_source const *source = (s_40f60_source const *)submission;
	(void)&fill;
	(void)&submit;
	byte *groups = record_format_groups(source->tag);
	long group = **(word **)(groups + 4) & 0x1ff;
	long count = (*(word **)(groups + 0xc))[group + 15] >> 9;
	for (long i = 0; i < count; ++i)
	{
		s_sort_record record;
		if (fill(source->tag, 0, 0, 15, i, source->context, &record))
			submit(source->tag, 0, 0, 15, i, source->context, &record);
	}
}

bool __stdcall function_4cbf0(long, long, long, long, long, long, void *);
void __stdcall function_4d0b0(long, long, long, long, long, long, void *);

// @retail 0x41020
void __stdcall function_41020(void *submission)
{
	void *const *submission_reference = &submission;
	function_40f60(*submission_reference, function_4cbf0, function_4d0b0);
}

extern long g_467130;
void *g_467134 = (void *)NONE;
s_record_sources *g_467138;
long g_4858b0, g_48574c;
byte g_485a75, g_485a76;
extern byte g_4670bc;
extern byte g_485b48[0x1fc0];
struct s_render_reset_state;
void function_16b10(s_render_reset_state *state);

// @retail 0x44550
void function_44550(void)
{
    g_4858b0 = g_467130;
    g_4670bc = true;
    function_16b10((s_render_reset_state *)g_485b48);
    long mode = g_467130;
    if (((1 << mode) & 0xbffdee) &&
        (mode != 16 || g_48574c == 3 || g_485a75 || g_485a76))
        function_3bc30(g_467134, g_467138, mode);
    g_4858b0 = NONE;
}

extern bool g_4ba019;
extern real g_485b28[7];

// @retail 0x1bf50
bool function_1bf50(long tag, long level, real distance, long *out)
{
    long selected = 0;
    byte *groups = record_format_groups(tag);
    long count = *(long *)groups;
    if (g_4ba019)
        distance *= 2.0f;
    if (level != 5)
    {
        real threshold = g_485b28[level] * distance;
        for (selected = 0; selected < count; ++selected)
        {
            byte *entries = *(byte **)(record_format_groups(tag) + 4);
            if (threshold >= *(real *)(entries + selected * 10 + 6))
                break;
        }
    }
    *out = selected;
    if (selected < 0)
        *out = 0;
    else
        *out = selected > count - 1 ? count - 1 : selected;
    return *out < count;
}

void function_1cf50(void);

struct s_4daa0_part
{
    byte unknown00[6];
    word first, count;
    short subpart, subpart_count;
};
struct s_4daa0_subpart
{
    word first, count;
    dword unknown04;
};
struct s_4daa0_geometry
{
    byte unknown00[0xc];
    s_4daa0_subpart *subparts;
    byte unknown10[0x14];
    word *indices;
};

// @retail 0x4daa0
long function_4daa0(bool filtered, bool strip, s_4daa0_geometry const *geometry,
    s_4daa0_part const *part, dword const *mask)
{
    long result = 0;
    if (!filtered)
    {
        long count = part->count;
        word *indices = geometry->indices + part->first;
        function_1cf50();
        D3DDevice_DrawIndexedVertices(strip ? D3DPT_TRIANGLESTRIP : D3DPT_TRIANGLELIST, count, indices);
        return count;
    }
    if (strip)
    {
        for (long i = part->subpart; i < part->subpart + part->subpart_count; ++i)
        {
            if (mask[i >> 5] & (1 << (i & 31)))
            {
                s_4daa0_subpart const *subpart = &geometry->subparts[i];
                long count = subpart->count;
                result += count;
                word *indices = geometry->indices + subpart->first;
                function_1cf50();
                D3DDevice_DrawIndexedVertices(D3DPT_TRIANGLESTRIP, count, indices);
            }
        }
    }
    else
    {
        long first = NONE;
        long last = NONE;
        for (long i = part->subpart; i < part->subpart + part->subpart_count; ++i)
        {
            if (mask[i >> 5] & (1 << (i & 31)))
            {
                s_4daa0_subpart const *subpart = &geometry->subparts[i];
                if (first == NONE)
                {
                    first = subpart->first;
                    last = first + subpart->count - 1;
                }
                else if (subpart->first != last + 1)
                {
                    word *indices = geometry->indices + first;
                    function_1cf50();
                    D3DDevice_DrawIndexedVertices(D3DPT_TRIANGLELIST, last - first + 1, indices);
                    result += last - first + 1;
                    first = subpart->first;
                    last = first + subpart->count - 1;
                }
                else
                    last = subpart->first + subpart->count - 1;
            }
        }
        if (first != NONE)
        {
            word *indices = geometry->indices + first;
            function_1cf50();
            D3DDevice_DrawIndexedVertices(D3DPT_TRIANGLELIST, last - first + 1, indices);
            result += last - first + 1;
        }
    }
    return result;
}

void function_40e30(short group, long tag, real distance, long level, word kind,
    dword and_mask, dword or_mask, t_record_fill fill, dword value, void *context);

struct s_cache_record;
s_cache_record *function_1e2d0(void);
long function_40e10(bool first, bool second);

struct s_frame_offset
{
    point3f position;
    vector3f forward;
    vector3f up;
};
extern s_frame_offset g_485618;

typedef void (__stdcall *t_41490_callback)(void *);

struct s_41490_record
{
    long type;
    real depth;
    long flags;
    byte active;
    byte unknown0d[0x13];
    void (__stdcall *callback)(void *);
    long tag;
    void *context;
    byte unknown2c[0x38];
    point3f position;
};

// @retail 0x41490
void function_41490(long tag, short group, word kind, real distance, t_record_fill fill,
    dword value, t_41490_callback callback, void *context, point3f const *position)
{
    byte *definition = g_4e3b44[tag & 0xffff].bytes;
    long pass;
    function_1bf50(tag, *(word *)(definition + 0x3c), distance, &pass);
    byte *groups = record_format_groups(tag);
    dword flags = *(dword *)(*(byte **)(groups + 4) + pass * 10 + 2);
    if (flags & 0x10006e)
        function_40e30(group, tag, distance, *(word *)(definition + 0x3c), kind,
            0xffffffff, 0, fill, value, context);
    if ((flags & 0x8000) && group == 0)
    {
        s_41490_record *record = (s_41490_record *)function_1e2d0();
        if (record)
        {
            vector3f delta;
            delta.i = position->x - g_485618.position.x;
            delta.j = position->y - g_485618.position.y;
            delta.k = position->z - g_485618.position.z;
            record->context = context;
            record->tag = tag;
            record->type = function_40e10((bool)(definition[0x16] & 1), (bool)((definition[0x16] >> 1) & 1));
            record->flags = 0;
            record->callback = callback;
            real depth = delta.k * g_485618.forward.k;
            depth += g_485618.forward.i * delta.i;
            depth += g_485618.forward.j * delta.j;
            record->depth = 0.0f - depth;
            record->position = *position;
        }
    }
}

#include <math.h>
dword __cdecl pack_color3f(color3f const *color);

// @retail 0x3d000
void function_3d000(real const *state)
{
    real direction[4] = { 1.0f, -1.0f, -1.0f, 0.5f };
    D3DDevice_SetVertexShaderConstant(-75, direction, 1);
    double sine = sin(state[30]);
    direction[0] = (real)(cos(state[29]) * sine);
    direction[1] = (real)(sin(state[29]) * sine);
    direction[2] = (real)cos(state[30]);
    direction[3] = state[25];
    real mapped[4];
    mapped[0] = (real)((direction[0] + 1.0) * 0.5);
    mapped[1] = (real)((direction[1] + 1.0) * 0.5);
    mapped[2] = (real)((direction[2] + 1.0) * 0.5);
    mapped[3] = 0.4970000088214874f;
    D3DDevice_SetVertexShaderConstant(-73, mapped, 1);
    if (state[34] > 0.0001f && state[40] > 0.0001f && state[41] > 0.0001f &&
        state[41] - state[40] > 0.0001f)
    {
        double inverse = 1.0 / state[34];
        real low = (real)(inverse * state[40] * 16777215.0);
        real high = (real)(inverse * state[41] * 16777215.0);
        direction[0] = 1.0f / (high - low);
        direction[1] = 0.0f - direction[0] * low;
        direction[2] = 0.0f;
        direction[3] = 0.0f;
    }
    else
    {
        direction[0] = 0.0f;
        direction[1] = 0.0f;
        direction[2] = 0.0f;
        direction[3] = 0.0f;
    }
    D3DDevice_SetVertexShaderConstant(-72, direction, 1);
    D3DDevice_SetRenderState(D3DRS_TEXTUREFACTOR, pack_color3f((color3f const *)(state + 22)));
    real parameters[4];
    memcpy(parameters, state + 36, sizeof(parameters));
    if (parameters[2] > 0.0001f)
        parameters[2] = 1.0f / parameters[2];
    if (parameters[3] > 0.0001f)
        parameters[3] = 1.0f / parameters[3];
    parameters[1] = 0.0f - state[37];
    D3DDevice_SetVertexShaderConstant(-74, parameters, 1);
    parameters[0] = state[13];
    parameters[1] = state[14];
    parameters[2] = state[15];
    parameters[3] = 1.0f;
    D3DDevice_SetVertexShaderConstant(-71, parameters, 1);
}
