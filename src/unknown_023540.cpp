// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include <xmmintrin.h>
#include <string.h>
#include <math.h>
#include <xtl.h>
#include "geometry_cache.h"

struct s_23600_section
{
	byte unknown00[0x34];
	byte *data;
	s_geometry_block_info block;
};

struct s_23600_definition
{
	byte unknown00[0x28];
	s_23600_section *sections;
};

struct s_23600_object_header
{
	byte unknown00[8];
	byte *object;
};

// @retail 0x23600
void *function_23600(long tag, long object_index, byte const *indices, long section_index, long node_index)
{
	void *result = NULL;
	(void)&indices;
	(void)&section_index;
	(void)&node_index;
	if (tag != NONE)
	{
		s_23600_definition *definition = (s_23600_definition *)g_4e3b44[tag & 0xffff].bytes;
		s_23600_section *section = &definition->sections[indices[section_index]];
		if (function_12de70(&section->block, 3))
		{
			byte *object = ((s_23600_object_header *)g_4e0300->data)[object_index & 0xffff].object;
			byte *nodes = *(byte **)(section->data + 0x68);
			result = object + *(short *)(object + 0x116) + 0x28 + nodes[node_index] * 0x34;
		}
	}
	return result;
}

byte g_43f220[16] = { 0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85 };

struct s_bucket_source
{
	byte unknown0[6];
	struct
	{
		word value;
		word unknown2;
	} entries[1];
};

/* one entry of the 0x68 byte table at 0x4b89b0 */
struct s_table_entry
{
	long key;
	long unknown4;
	long data_handle;
	long data_offset[16];
	byte data_count[16];
	vector3f scale;
};

struct s_surface_size
{
	short unknown0;
	short width;
	short count;
};

struct s_table_entry g_4b89b0[1];
long g_5093e8;
struct s_487b10_arena
{
	byte initialized;
	byte field_01[7];
	long field_08;
	byte data[0x27000];
	long count;
	byte active;
	byte field_27011[3];
	long field_27014;
	long field_27018;
	long field_2701c;
	byte field_27020[0x20];
	long record_count;
	byte records[0x6000];
};

s_487b10_arena g_487b10;

// @retail 0x1d660
void function_01d660(void)
{
	g_487b10.field_08 = 0;
	g_487b10.count = 0;
	memset(g_487b10.data, 0, sizeof(g_487b10.data));
	g_487b10.active = false;
	g_487b10.field_27014 = 0;
	g_487b10.field_27018 = 0;
	g_487b10.field_2701c = 0;
	g_487b10.record_count = 0;
	memset(g_487b10.records, 0, sizeof(g_487b10.records));
	g_487b10.initialized = true;
}

// @retail 0x23540
void function_023540(
	short count,
	s_bucket_source const *source,
	s_table_entry *entry,
	long multiplier)
{
	long total = 0;
	for (long i = 0; i < count; i++)
	{
		entry->data_offset[i] = total * multiplier;
		long value = source->entries[i].value;
		if (value > 2)
			value = 2;
		total += value;
		entry->data_count[i] = (byte)value;
	}
}

// @retail 0x235a0
void function_0235a0(
	real const *source,
	transform4x3f *matrix)
{
	real *m = (real *)matrix;
	m[1] = source[0];
	m[4] = source[1];
	m[7] = source[2];
	m[10] = source[3];
	m[2] = source[4];
	m[5] = source[5];
	m[8] = source[6];
	m[11] = source[7];
	m[3] = source[8];
	m[6] = source[9];
	m[9] = source[10];
	m[12] = source[11];
	m[0] = 1.0f;
}

// @retail 0x24550
real distance_sq3f(
	point3f const *a,
	point3f const *b)
{
	vector3f v;
	v.i = b->x - a->x;
	v.j = b->y - a->y;
	v.k = b->z - a->z;
	real sum = v.k * v.k;
	sum += v.i * v.i;
	sum += v.j * v.j;
	return sum;
}

// @retail 0x24590
real magnitude3d(
	vector3f const *v)
{
	return (real)sqrt(v->i*v->i + v->j*v->j + v->k*v->k);
}

// @retail 0x24e50
dword function_024e50(
	word a,
	word b)
{
	return
		((((((((g_43f220[a >> 12] << 1 | g_43f220[b >> 12]) << 7 | g_43f220[(a >> 8) & 0xf]) << 1 | g_43f220[(b >> 8) & 0xf]) << 7 | g_43f220[(a >> 4) & 0xf]) << 1 | g_43f220[(b >> 4) & 0xf]) << 7 | g_43f220[a & 0xf]) << 1) | g_43f220[b & 0xf]);
}

__inline void histogram_add(
	real *histogram,
	byte index)
{
	histogram[index] += 1.0f;
}

// @retail 0x24a70
void function_024a70(
	real *histogram,
	dword const *pixels,
	word stride)
{
	memset(histogram, 0, 256 * sizeof(real));
	for (long y = 0; y < 48; y++)
	{
		for (long x = 0; x < 64; x++)
		{
			histogram_add(histogram, (byte)*pixels);
			pixels++;
		}
		pixels += stride - 64;
	}
}

// @retail 0x24b80
void function_024b80(
	real *histogram)
{
	real total = 0.0f;
	for (long i = 0; i < 256; i++)
		total += histogram[i];
	if (total != 0.0f)
	{
		real inverse = 1.0f / total;
		for (long i = 0; i < 256; i++)
			histogram[i] *= inverse;
	}
}

/* the depth terms of function_350e0, inlined with x == 1 */
__inline void clip_depth_terms(
	real *out,
	real x)
{
	real scale;
	real v;
	real w;

	scale = g_485ad4.hi / (g_485ad4.hi - g_485ad4.lo);
	v = (scale * x - g_485ad4.lo * scale) / x * 16777215.0f;
	if (0.0f > v)
		v = 0.0f;
	else if (v > 16777215.0f)
		v = 16777215.0f;
	out[2] = v;

	w = x / g_485ad4.hi;
	w = w * 16777215.0f;
	if (0.0f > w)
		w = 0.0f;
	else if (w > 16777215.0f)
		w = 16777215.0f;
	out[3] = w;
}

// @retail 0x24860
bool __stdcall function_024860(
	long mode,
	real const *bounds,
	real const *t,
	real *out,
	long unused)
{
	switch (mode)
	{
		case 0:
			out[0] = t[0] * 64.0f;
			out[1] = t[1] * 48.0f;
			clip_depth_terms(out, 1.0f);
			return true;
		case 1:
			out[0] = (bounds[1] - bounds[0]) * t[0] + bounds[0];
			out[1] = (bounds[3] - bounds[2]) * t[1] + bounds[2];
			out[2] = 0.0f;
			out[3] = 1.0f;
			return true;
		default:
			return false;
	}
}

// @retail 0x254c0
bool __stdcall function_0254c0(
	long mode,
	real const *bounds,
	real const *t,
	real *out,
	long unused)
{
	switch (mode)
	{
		case 0:
			out[0] = (bounds[1] - bounds[0]) * t[0] + bounds[0];
			out[1] = (bounds[3] - bounds[2]) * t[1] + bounds[2];
			clip_depth_terms(out, 1.0f);
			return true;
		case 1:
			out[0] = (bounds[1] - bounds[0]) * t[0] + bounds[0];
			out[1] = (bounds[3] - bounds[2]) * t[1] + bounds[2];
			out[2] = 0.0f;
			out[3] = 1.0f;
			return true;
		default:
			return false;
	}
}

real g_509400;
extern real g_485ae0;

real g_48565c;
real g_485778, g_48577c, g_485790, g_485794;
real g_4b9c74[5][14];
real g_4b9d90, g_4b9d94;
void function_350e0(real *out, real const *a, real const *b, real x);

// @retail 0x26710
bool __stdcall function_26710(long mode, real const *bounds, real const *t, real *out, long unused)
{
	(void)&mode;
	(void)&bounds;
	(void)&t;
	(void)&out;
	(void)&unused;
	if (g_509400 <= 0.0f)
		g_509400 = g_485ae0;
	switch (mode)
	{
	case 0:
		function_350e0(out, bounds, t, g_509400);
		return true;
	case 1: case 4:
		out[0] = (bounds[1] - bounds[0]) * t[0] + bounds[0];
		out[1] = (bounds[3] - bounds[2]) * t[1] + bounds[2];
		out[2] = 0.0f;
		out[3] = 1.0f;
		return true;
	case 2:
		{
			real span = g_48577c - g_485778;
			real scale = 1.0f / (0.0001f > span ? 0.0001f : span);
			out[0] = g_48565c * scale;
			out[1] = 0.0f;
			out[2] = 0.0f - g_485778 * scale;
			out[3] = 0.0f;
			return true;
		}
	case 3:
		{
			real span = g_485794 - g_485790;
			real scale = 1.0f / (0.0001f > span ? 0.0001f : span);
			out[0] = g_48565c * scale;
			out[1] = 0.0f;
			out[2] = 0.0f - g_485790 * scale;
			out[3] = 0.0f;
			return true;
		}
	default: return false;
	}
}

// @retail 0x26ca0
bool __stdcall function_26ca0(long mode, real const *bounds, real const *t, real *out, long unused)
{
	(void)&mode;
	(void)&bounds;
	(void)&t;
	(void)&out;
	(void)&unused;
	real const *matrix = g_4b9c74[mode];
	if (g_509400 <= 0.0f)
		g_509400 = g_485ae0;
	if (mode == 0)
	{
		out[0] = (bounds[1] - bounds[0]) * t[0] + bounds[0];
		out[1] = (bounds[3] - bounds[2]) * t[1] + bounds[2];
		clip_depth_terms(out, 1.0f);
		out[0] *= g_4b9d90;
		out[1] *= g_4b9d90;
		return true;
	}
	if (mode > 0 && mode <= 4)
	{
		out[0] = (t[0] * 2.0f - 1.0f) * matrix[6] - (t[1] * 2.0f - 1.0f) * matrix[7] * g_4b9d94 + matrix[9];
		out[1] = (t[0] * 2.0f - 1.0f) * matrix[10] - (t[1] * 2.0f - 1.0f) * matrix[11] * g_4b9d94 + matrix[13];
		out[2] = 0.0f;
		out[3] = 1.0f;
		return true;
	}
	return false;
}

// @retail 0x27960
bool __stdcall function_27960(long mode, real const *bounds, real const *t, real *out, long unused)
{
	(void)&mode;
	(void)&bounds;
	(void)&t;
	(void)&out;
	(void)&unused;
	if (g_509400 <= 0.0f)
		g_509400 = g_485ae0;
	switch (mode)
	{
	case 0:
		out[0] = (bounds[1] - bounds[0]) * t[0] + bounds[0];
		out[1] = (bounds[3] - bounds[2]) * t[1] + bounds[2];
		clip_depth_terms(out, g_509400);
		return true;
	case 1:
		out[0] = (bounds[1] - bounds[0]) * t[0] + bounds[0];
		out[1] = (bounds[3] - bounds[2]) * t[1] + bounds[2];
		out[2] = 0.0f;
		out[3] = 1.0f;
		return true;
	default:
		return false;
	}
}

/* the table entry whose key matches, or NONE */
__inline long find_table_entry(
	long key)
{
	short i;
	for (i = 0; i < g_5093e8; i++)
	{
		if (g_4b89b0[i].key == key)
			return i;
	}
	return NONE;
}

struct s_24490_entry
{
	long key;
	long count;
};

struct s_24490_group
{
	long unknown00;
	long count;
	s_24490_entry *entries;
};

struct s_24490_definition
{
	byte unknown00[0x14];
	long count;
	s_24490_group *groups;
	byte unknown1c[0x14];
	byte *data;
	s_geometry_block_info block;
};

// @retail 0x24490
void *function_24490(long key, s_24490_definition *definition, long entry_key)
{
	long index = find_table_entry(key);
	if (index == NONE)
		return NULL;
	long group = 5 - (short)g_4b89b0[index].unknown4;
	if (group > definition->count - 1)
		group = definition->count - 1;
	s_24490_group *selected = &definition->groups[group];
	for (long i = 0; i < selected->count; ++i)
	{
		if (selected->entries[i].key == entry_key)
		{
			long count = selected->entries[i].count / 5;
			if (!function_12de70(&definition->block, 3))
				return NULL;
			byte *data = definition->data;
			long size = data[0] == 0x39 ? count * 10 : count * 20;
			*(long *)(data + 4) = size;
			*(long *)(data + 8) = size;
			return data;
		}
	}
	return NULL;
}

// @retail 0x1d6b0
__inline real *table_entry_data(
	long handle)
{
	byte *base = NULL;
	if (handle != NONE && !(handle & 0x80000000))
		base = g_487b10.data + (handle & 0xfffffff);
	return (real *)base;
}

// @retail 0x24040
void function_024040(
	long key,
	s_surface_size const *size,
	long index)
{
	long entry_index = find_table_entry(key);
	if (entry_index == NONE)
		return;

	real *data = table_entry_data(g_4b89b0[entry_index].data_handle) + g_4b89b0[entry_index].data_offset[index];
	long total = (size->count * 3 + 4) * size->width;
	byte stored = g_4b89b0[entry_index].data_count[index];
	long count;
	if (stored > 2)
		count = 2;
	else
	{
		count = stored;
		if (count <= 0)
			return;
	}

	long vectors = (count * total) / 4;
	real constants[4] = { 1.0f, 4.0f, 10200.5f, 0.0039254902f };

	D3DDevice_SetVertexShaderConstant(-28, &g_4b89b0[entry_index].scale, 1);
	D3DDevice_SetVertexShaderConstant(-27, constants, 1);
	D3DDevice_SetVertexShaderConstant(-26, data, vectors);
}

/* a sample location: a matrix index (stored as a real) and four weights */
struct s_sample_point
{
	real index;
	real weight[4];
};

// @retail 0x241c0
bool function_0241c0(
	long key,
	long block,
	vector3f *scale_out,
	long index,
	s_sample_point const *point0,
	s_sample_point const *point1,
	s_sample_point const *point2,
	real fraction0,
	real fraction1,
	vector3f *result)
{
	long entry_index = find_table_entry(key);
	if (entry_index != NONE)
	{
		*scale_out = g_4b89b0[entry_index].scale;
		scale_out->i *= 0.8f;
		scale_out->j *= 0.8f;
		scale_out->k *= 0.8f;

		real const *data = table_entry_data(g_4b89b0[entry_index].data_handle) + g_4b89b0[entry_index].data_offset[index];
		if (block > g_4b89b0[entry_index].data_count[index] - 1)
			block = 0;
		data += block * 160;

		s_sample_point const *points[3] = { point0, point1, point2 };
		real corner[3][3];
		long i = 0;
		do
		{
			s_sample_point const *point = points[i];
			real const *m = data + (long)point->index * 16;
			real x = m[6] * point->weight[2];
			x += m[4] * point->weight[0];
			x += m[7] * point->weight[3];
			x += m[5] * point->weight[1];
			x += m[0];
			corner[i][0] = x;
			real y = m[10] * point->weight[2];
			y += m[8] * point->weight[0];
			y += m[11] * point->weight[3];
			y += m[9] * point->weight[1];
			y += m[1];
			corner[i][1] = y;
			real z = m[14] * point->weight[2];
			z += m[12] * point->weight[0];
			z += m[15] * point->weight[3];
			z += m[13] * point->weight[1];
			z += m[2];
			corner[i][2] = z;
			i++;
		}
		while (i < 3);

		for (long k = 0; k < 3; k++)
		{
			real v = (corner[1][k] - corner[0][k]) * fraction0 + (corner[2][k] - corner[0][k]) * fraction1 + corner[0][k];
			real r = v;
			if (v > 1.0f)
				r = 1.0f;
			if (0.0f > v)
				r = 0.0f;
			result->n[k] = r;
		}
		return true;
	}
	return false;
}

struct s_block_layout
{
	short size;
	short groups;
	short columns;
};

/* dot product of two 3x3 blocks stored as nine reals */
#define DOT_PRODUCT9(result, a, b) \
	__asm mov ecx, a \
	__asm mov edx, b \
	__asm movlps xmm0, qword ptr [ecx] \
	__asm movlps xmm1, qword ptr [edx] \
	__asm movlps xmm2, qword ptr [ecx + 0x10] \
	__asm movlps xmm3, qword ptr [edx + 0x10] \
	__asm movhps xmm0, qword ptr [ecx + 8] \
	__asm movhps xmm1, qword ptr [edx + 8] \
	__asm movhps xmm2, qword ptr [ecx + 0x18] \
	__asm movhps xmm3, qword ptr [edx + 0x18] \
	__asm mulps xmm0, xmm1 \
	__asm mulps xmm2, xmm3 \
	__asm addps xmm0, xmm2 \
	__asm movss xmm1, dword ptr [ecx + 0x20] \
	__asm movhlps xmm2, xmm0 \
	__asm mulss xmm1, dword ptr [edx + 0x20] \
	__asm addps xmm0, xmm2 \
	__asm addss xmm1, xmm0 \
	__asm shufps xmm0, xmm0, 0x55 \
	__asm addss xmm0, xmm1 \
	__asm movss result, xmm0

// @retail 0x23ca0
void function_023ca0(
	s_block_layout const *layout,
	real const *data,
	real const *vector0,
	real const *vector1,
	real const *vector2,
	real *out)
{
	long block_size = layout->size * layout->size;
	long group_size = block_size * 3;
	long stride = (layout->columns + 1) * group_size;
	real const *block0 = data;
	real const *block1 = data + block_size;
	real const *block2 = data + block_size * 2;
	real const *first_cell0 = data + group_size;
	real const *first_cell1 = data + group_size + block_size;
	real const *first_cell2 = data + group_size + block_size * 2;
	for (long i = 0; i < layout->groups; i++)
	{
		real const *cell0 = first_cell0;
		real const *cell1 = first_cell1;
		real const *cell2 = first_cell2;
		real const *block;
		real dot0;
		real dot1;
		real dot2;
		block = block0;
		DOT_PRODUCT9(dot0, block, vector0);
		block = block1;
		DOT_PRODUCT9(dot1, block, vector1);
		block = block2;
		DOT_PRODUCT9(dot2, block, vector2);
		out[(layout->columns * 3 + 4) * i] = dot0;
		out[(layout->columns * 3 + 4) * i + 1] = dot1;
		out[(layout->columns * 3 + 4) * i + 2] = dot2;
		real *row = out + (layout->columns * 3 + 4) * i + 4;
		for (long j = 0; j < layout->columns; j++)
		{
			real const *cell_block;
			real cell_dot0;
			real cell_dot1;
			real cell_dot2;
			cell_block = cell0;
			DOT_PRODUCT9(cell_dot0, cell_block, vector0);
			cell_block = cell1;
			DOT_PRODUCT9(cell_dot1, cell_block, vector1);
			cell_block = cell2;
			DOT_PRODUCT9(cell_dot2, cell_block, vector2);
			row[j] = cell_dot0;
			row[layout->columns + j] = cell_dot1;
			row[layout->columns * 2 + j] = cell_dot2;
			cell0 += group_size;
			cell1 += group_size;
			cell2 += group_size;
		}
		block0 += stride;
		block1 += stride;
		block2 += stride;
		first_cell0 += stride;
		first_cell1 += stride;
		first_cell2 += stride;
	}
}

struct s_24c70_range
{
    real low, high, average;
    real smoothed_low, smoothed_high, smoothed_average;
};

PRIVATE __forceinline real histogram_blend(real previous, real value)
{
    real result;
    if (previous > value)
        result = previous * (1.0f - 0.16f) + value * 0.16f;
    else
        result = previous * (1.0f - 0.16f) + value * 0.16f;
    return result;
}

// @retail 0x24c70
void function_24c70(real const *histogram, s_24c70_range *range)
{
    real low, high;
    real sum = 0.0f;
    for (long i = 0; i < 256; ++i)
    {
        sum += histogram[i];
        if (sum >= 0.0f)
        {
            low = (real)i * (1.0f / 255.0f);
            break;
        }
    }
    sum = 0.0f;
    for (long j = 255; j >= 0; --j)
    {
        sum += histogram[j];
        if (sum >= 0.01f)
        {
            high = (real)j * (1.0f / 255.0f);
            break;
        }
    }
    sum = 0.0f;
    for (long k = 0; k < 256; ++k)
        sum += (real)k * histogram[k];
    real average = sum * (1.0f / 255.0f);
    range->low = low;
    range->high = high;
    range->average = average;
    range->smoothed_low = histogram_blend(range->smoothed_low, low);
    range->smoothed_high = histogram_blend(range->smoothed_high, high);
    range->smoothed_average = range->smoothed_average * 0.8f + average * 0.2f;
    range->smoothed_low = range->smoothed_low < 0.0f ? 0.0f : range->smoothed_low > 0.0f ? 0.0f : range->smoothed_low;
    range->smoothed_high = range->smoothed_high < 0.7f ? 0.7f : range->smoothed_high > 1.0f ? 1.0f : range->smoothed_high;
    range->smoothed_average = range->smoothed_average < range->smoothed_low ? range->smoothed_low :
        range->smoothed_average > range->smoothed_high ? range->smoothed_high : range->smoothed_average;
}

namespace D3D { namespace PixelJar {
    void __stdcall FindSurfaceWithinTexture(D3DPixelContainer *container, D3DCUBEMAP_FACES face, unsigned int level,
        byte **bits, dword *pitch, dword *width, dword *height, dword *slice);
} }

void *function_01dcc0(long index);
struct s_histogram_state
{
    s_24c70_range range;
    real *histogram;
};

// @retail 0x24970
void __cdecl function_24970(s_histogram_state *state)
{
    (void)&state;
    real *histogram = state->histogram;
    XSaveFloatingPointStateForDpc();
    byte *bits;
    dword pitch, width, height, slice;
    D3D::PixelJar::FindSurfaceWithinTexture((D3DPixelContainer *)function_01dcc0(14), (D3DCUBEMAP_FACES)0, 0,
        &bits, &pitch, &width, &height, &slice);
    function_024a70(histogram, (dword const *)bits, (word)(pitch >> 2));
    function_024b80(histogram);
    function_24c70(histogram, &state->range);
    XRestoreFloatingPointStateForDpc();
}

bool g_4b9d9c;
real g_4857dc, g_4857e0;

// @retail 0x27520
bool __stdcall function_27520(long mode, real const *bounds, real const *t, real *out, long unused)
{
    (void)&mode; (void)&bounds; (void)&t; (void)&out; (void)&unused;
    if (g_509400 <= 0.0f)
        g_509400 = g_485ae0;
    switch (mode)
    {
    case 0:
        function_350e0(out, bounds, t, g_509400);
        return true;
    case 1:
        out[0] = (bounds[1] - bounds[0]) * t[0] + bounds[0];
        out[1] = (bounds[3] - bounds[2]) * t[1] + bounds[2];
        out[2] = 0.0f;
        out[3] = 1.0f;
        return true;
    case 2:
        if (!g_4b9d9c) goto scaled;
    case 3:
        {
            real span = g_4857e0 - g_4857dc;
            real scale = 1.0f / (0.0001f > span ? 0.0001f : span);
            out[0] = g_48565c * scale;
            out[1] = 0.0f;
            out[2] = 0.0f - g_4857dc * scale;
            out[3] = 0.0f;
            return true;
        }
    case 4:
scaled:
        out[0] = (bounds[1] - bounds[0]) * t[0] + bounds[0];
        out[1] = (bounds[3] - bounds[2]) * t[1] + bounds[2];
        out[2] = 0.0f;
        out[3] = 1.0f;
        out[0] *= g_4b9d90;
        out[1] *= g_4b9d90;
        return true;
    default:
        return false;
    }
}

extern byte g_4b99b0[0x4c];

// @retail 0x298f0
bool __stdcall function_298f0(long mode, real const *bounds, real const *t, real *out, long unused)
{
    (void)&mode; (void)&bounds; (void)&t; (void)&out; (void)&unused;
    if (g_509400 <= 0.0f)
        g_509400 = g_485ae0;
    switch (mode)
    {
    case 0:
        function_350e0(out, bounds, t, g_509400);
        return true;
    case 2:
        if (g_4b99b0[0x41] && g_4b99b0[1])
        {
            out[0] = *(real *)(g_4b99b0 + 0x24);
            out[1] = 0.0f;
            out[2] = t[0] * 640.0f;
            out[3] = 0.0f;
            return true;
        }
    case 3:
        if (g_4b99b0[0x41] && g_4b99b0[1])
        {
            out[0] = 0.0f;
            out[1] = *(real *)(g_4b99b0 + 0x28);
            out[2] = t[1] * 480.0f;
            out[3] = 0.0f;
            return true;
        }
    case 1: case 4:
        {
            out[0] = (bounds[1] - bounds[0]) * t[0] + bounds[0];
            out[1] = (bounds[3] - bounds[2]) * t[1] + bounds[2];
            out[2] = 0.0f;
            out[3] = 1.0f;
            real strength = *(real *)(g_4b99b0 + 0x3c);
            if (strength != 0.0f)
            {
                real fraction = (5 - mode) * 0.25f;
                real x = *(real *)(g_4b99b0 + 0x1c) * strength * fraction + 1.0f;
                real y = *(real *)(g_4b99b0 + 0x20) * strength * fraction + 1.0f;
                real inverse_x = x > 0.0f ? 1.0f / x : 0.0f;
                real inverse_y = y > 0.0f ? 1.0f / y : 0.0f;
                out[0] = (bounds[1] - bounds[0]) * (1.0f - inverse_x) * 0.5f + out[0] * inverse_x;
                out[1] = (bounds[3] - bounds[2]) * (1.0f - inverse_y) * 0.5f + out[1] * inverse_y;
                return true;
            }
            return mode == 1;
        }
    default:
        return false;
    }
}

extern D3DPIXELSHADERDEF g_484f68;
extern long g_4858b8;
extern byte *g_485a80;
extern byte g_51f0f0[0x2d8];
color3f g_4857a8;
real g_4857b4;
void function_14bc0(short index, short element, bool use_depth);
void function_0222d0(D3DRENDERSTATETYPE state, dword value);
bool function_1ccf0(D3DPIXELSHADERDEF const *program);
struct s_shader_cache;
void function_1c590(s_shader_cache *state, long tag, long index);
void __stdcall function_1c710(void *state);
dword __cdecl function_131f40(real alpha, color3f const *color);

// @retail 0x26520
bool __stdcall function_26520(void *context)
{
    g_509400 = g_485ae0;
    if (g_4857b4 > 0.0f)
    {
        function_14bc0((short)g_4858b8, 0, true);
        function_0222d0(D3DRS_COLORWRITEENABLE, 0x1010101);
        function_0222d0(D3DRS_ALPHABLENDENABLE, 1);
        function_0222d0(D3DRS_SRCBLEND, D3DBLEND_CONSTANTCOLOR);
        function_0222d0(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        function_0222d0(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        function_0222d0(D3DRS_BLENDCOLOR, 0xffffff);
        function_0222d0(D3DRS_ALPHATESTENABLE, 0);
        function_0222d0(D3DRS_CULLMODE, 0);
        function_0222d0(D3DRS_STENCILENABLE, 0);
        function_0222d0(D3DRS_ZENABLE, 2);
        function_0222d0(D3DRS_ZWRITEENABLE, 0);
        function_0222d0(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
        function_0222d0(D3DRS_ZBIAS, 0);
        memset(&g_484f68, 0, sizeof(g_484f68));
        g_484f68.PSCombinerCount = 0x11001;
        g_484f68.PSFinalCombinerConstant0 = function_131f40(g_4857b4, &g_4857a8);
        g_484f68.PSFinalCombinerInputsABCD = 0x1200000;
        g_484f68.PSFinalCombinerInputsEFG = 0x1100;
        function_1ccf0(&g_484f68);
        function_1c590((s_shader_cache *)g_51f0f0, *(long *)(*(byte **)(g_485a80 + 0x5c) + 0x64), 0);
        function_1c710(g_51f0f0);
        return true;
    }
    return false;
}

void function_14f60(short stage, short index);

// @retail 0x2b000
bool __stdcall function_2b000(void *context)
{
    g_509400 = g_485ae0;
    function_14bc0(16, 0, false);
    function_14f60(0, (short)g_4858b8);
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
    D3DDevice_SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
    D3DDevice_SetTextureStageState(0, D3DTSS_MIPFILTER, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAXANISOTROPY, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MIPMAPLODBIAS, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAXMIPLEVEL, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_COLORSIGN, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_ALPHAKILL, 0);
    function_0222d0(D3DRS_COLORWRITEENABLE, 0x10101);
    function_0222d0(D3DRS_ALPHABLENDENABLE, 0);
    function_0222d0(D3DRS_ALPHATESTENABLE, 0);
    function_0222d0(D3DRS_CULLMODE, 0);
    function_0222d0(D3DRS_STENCILENABLE, 0);
    function_0222d0(D3DRS_ZENABLE, 0);
    function_0222d0(D3DRS_ZBIAS, 0);
    memset(&g_484f68, 0, sizeof(g_484f68));
    g_484f68.PSTextureModes = 1;
    g_484f68.PSCombinerCount = 0x11001;
    g_484f68.PSFinalCombinerInputsABCD = 8;
    g_484f68.PSFinalCombinerInputsEFG = 0;
    D3DDevice_SetPixelShaderProgram(&g_484f68);
    function_1c590((s_shader_cache *)g_51f0f0, *(long *)(*(byte **)(g_485a80 + 0x5c) + 0x64), 0);
    function_1c710(g_51f0f0);
    return true;
}

// @retail 0x296c0
bool __stdcall function_296c0(void *context)
{
    if (!g_4b99b0[0] && !g_4b99b0[1])
        return false;
    g_509400 = g_485ae0;
    function_14bc0(*(short *)(g_4b99b0 + 0x44), 0, false);
    function_14f60(0, *(short *)(g_4b99b0 + 0x34));
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_POINT);
    D3DDevice_SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_POINT);
    D3DDevice_SetTextureStageState(0, D3DTSS_MIPFILTER, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAXANISOTROPY, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MIPMAPLODBIAS, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAXMIPLEVEL, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_COLORSIGN, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_ALPHAKILL, 0);
    function_0222d0(D3DRS_COLORWRITEENABLE, 0x10101);
    function_0222d0(D3DRS_ALPHABLENDENABLE, 0);
    function_0222d0(D3DRS_ALPHATESTENABLE, 0);
    function_0222d0(D3DRS_CULLMODE, 0);
    function_0222d0(D3DRS_STENCILENABLE, 0);
    function_0222d0(D3DRS_ZENABLE, 0);
    function_0222d0(D3DRS_ZBIAS, 0);
    memset(&g_484f68, 0, sizeof(g_484f68));
    g_484f68.PSTextureModes = 1;
    g_484f68.PSCombinerCount = 0x11001;
    g_484f68.PSFinalCombinerInputsABCD = 8;
    g_484f68.PSFinalCombinerInputsEFG = 0;
    D3DDevice_SetPixelShaderProgram(&g_484f68);
    function_1c590((s_shader_cache *)g_51f0f0, *(long *)(*(byte **)(g_485a80 + 0x5c) + 0x64), 0);
    function_1c710(g_51f0f0);
    return true;
}

color3f g_4857bc;
dword __cdecl pack_color3f(color3f const *color);

// @retail 0x276d0
bool __stdcall function_276d0(void *context)
{
    g_509400 = g_485ae0;
    function_14bc0((short)g_4858b8, 0, false);
    function_14f60(0, 3);
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_POINT);
    D3DDevice_SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_POINT);
    D3DDevice_SetTextureStageState(0, D3DTSS_MIPFILTER, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAXANISOTROPY, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MIPMAPLODBIAS, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAXMIPLEVEL, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_COLORSIGN, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_ALPHAKILL, 0);

    function_0222d0(D3DRS_COLORWRITEENABLE, 0x1010101);
    function_0222d0(D3DRS_ALPHABLENDENABLE, 1);
    function_0222d0(D3DRS_SRCBLEND, D3DBLEND_CONSTANTCOLOR);
    function_0222d0(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    function_0222d0(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    function_0222d0(D3DRS_BLENDCOLOR, 0xffffff);
    function_0222d0(D3DRS_ALPHATESTENABLE, 0);
    function_0222d0(D3DRS_CULLMODE, 0);
    function_0222d0(D3DRS_STENCILENABLE, 0);
    function_0222d0(D3DRS_ZENABLE, 0);
    function_0222d0(D3DRS_ZBIAS, 0);
    memset(&g_484f68, 0, sizeof(g_484f68));
    g_484f68.PSTextureModes = 1;
    g_484f68.PSCombinerCount = 0x11001;
    g_484f68.PSAlphaInputs[0] = 0x8200000;
    g_484f68.PSAlphaOutputs[0] = 0xc0;
    g_484f68.PSFinalCombinerConstant0 = pack_color3f(&g_4857bc);
    g_484f68.PSFinalCombinerInputsABCD = 0x1c010000;
    g_484f68.PSFinalCombinerInputsEFG = 0x1c00;
    function_1ccf0(&g_484f68);
    function_1c590((s_shader_cache *)g_51f0f0, *(long *)(*(byte **)(g_485a80 + 0x5c) + 0x64), 0);
    function_1c710(g_51f0f0);
    return true;
}

// @retail 0x29080
bool __stdcall function_29080(void *context)
{
    if (!g_4b99b0[0] && !g_4b99b0[1])
        return false;
    g_509400 = g_485ae0;
    function_14bc0(*(short *)(g_4b99b0 + 0x44), 0, false);
    function_14f60(0, g_4b99b0[1] ? *(short *)(g_4b99b0 + 0x38) : 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_POINT);
    D3DDevice_SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_POINT);
    D3DDevice_SetTextureStageState(0, D3DTSS_MIPFILTER, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAXANISOTROPY, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MIPMAPLODBIAS, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAXMIPLEVEL, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_COLORSIGN, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_ALPHAKILL, 0);

    if (g_4b99b0[1])
    {
        function_14f60(2, 0);
        D3DDevice_SetTextureStageState(2, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
        D3DDevice_SetTextureStageState(2, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
        D3DDevice_SetTextureStageState(2, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
        D3DDevice_SetTextureStageState(2, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
        D3DDevice_SetTextureStageState(2, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
        D3DDevice_SetTextureStageState(2, D3DTSS_MIPFILTER, 0);
        D3DDevice_SetTextureStageState(2, D3DTSS_MAXANISOTROPY, 0);
        D3DDevice_SetTextureStageState(2, D3DTSS_MIPMAPLODBIAS, 0);
        D3DDevice_SetTextureStageState(2, D3DTSS_MAXMIPLEVEL, 0);
        D3DDevice_SetTextureStageState(2, D3DTSS_COLORSIGN, 0);
        D3DDevice_SetTextureStageState(2, D3DTSS_ALPHAKILL, 0);

    }
    function_0222d0(D3DRS_COLORWRITEENABLE, 0x10101);
    function_0222d0(D3DRS_ALPHABLENDENABLE, g_4b99b0[0]);
    function_0222d0(D3DRS_SRCBLEND, D3DBLEND_ONE);
    function_0222d0(D3DRS_DESTBLEND, D3DBLEND_CONSTANTCOLOR);
    color3f blend = *(color3f *)(g_4b99b0 + 0x10);
    function_0222d0(D3DRS_BLENDCOLOR, pack_color3f(&blend));
    function_0222d0(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    function_0222d0(D3DRS_ALPHATESTENABLE, 0);
    function_0222d0(D3DRS_CULLMODE, 0);
    function_0222d0(D3DRS_STENCILENABLE, 0);
    function_0222d0(D3DRS_ZENABLE, 0);
    function_0222d0(D3DRS_ZBIAS, 0);
    color3f color = *(color3f *)(g_4b99b0 + 4);
    if (g_4b99b0[1])
    {
        memset(&g_484f68, 0, sizeof(g_484f68));
        g_484f68.PSTextureModes = 0x2621;
        g_484f68.PSDotMapping = 0x11;
        g_484f68.PSCombinerCount = 0x11001;
        g_484f68.PSFinalCombinerConstant0 = pack_color3f(&color);
        g_484f68.PSFinalCombinerInputsABCD = 0xa010000;
    }
    else
    {
        memset(&g_484f68, 0, sizeof(g_484f68));
        g_484f68.PSTextureModes = 1;
        g_484f68.PSCombinerCount = 0x11001;
        g_484f68.PSFinalCombinerConstant0 = pack_color3f(&color);
        g_484f68.PSFinalCombinerInputsABCD = 0x8010000;
    }
    g_484f68.PSFinalCombinerInputsEFG = 0;
    function_1ccf0(&g_484f68);
    function_1c590((s_shader_cache *)g_51f0f0, *(long *)(*(byte **)(g_485a80 + 0x5c) + 0x64), 0);
    function_1c710(g_51f0f0);
    return true;
}

// @retail 0x251b0
bool __stdcall function_251b0(void *context)
{
    function_0222d0(D3DRS_COLORWRITEENABLE, 0x10101);
    function_0222d0(D3DRS_ALPHABLENDENABLE, 0);
    function_0222d0(D3DRS_ALPHATESTENABLE, 0);
    function_0222d0(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    function_0222d0(D3DRS_ZWRITEENABLE, 0);
    function_0222d0(D3DRS_CULLMODE, 0);
    function_0222d0(D3DRS_STENCILENABLE, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_POINT);
    D3DDevice_SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_POINT);
    D3DDevice_SetTextureStageState(0, D3DTSS_MIPFILTER, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAXANISOTROPY, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MIPMAPLODBIAS, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_MAXMIPLEVEL, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_COLORSIGN, 0);
    D3DDevice_SetTextureStageState(0, D3DTSS_ALPHAKILL, 0);
    if (!context)
    {
        function_14bc0(1, 0, true);
        function_14f60(0, g_4858b8);
        function_14f60(1, 15);
        function_14f60(2, 15);
        D3DDevice_SetTextureStageState(1, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
        D3DDevice_SetTextureStageState(1, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
        D3DDevice_SetTextureStageState(1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
        D3DDevice_SetTextureStageState(1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
        D3DDevice_SetTextureStageState(1, D3DTSS_MIPFILTER, 0);
        D3DDevice_SetTextureStageState(1, D3DTSS_MAXANISOTROPY, 0);
        D3DDevice_SetTextureStageState(1, D3DTSS_MIPMAPLODBIAS, 0);
        D3DDevice_SetTextureStageState(1, D3DTSS_MAXMIPLEVEL, 0);
        D3DDevice_SetTextureStageState(1, D3DTSS_COLORSIGN, 0);
        D3DDevice_SetTextureStageState(1, D3DTSS_ALPHAKILL, 0);
        D3DDevice_SetTextureStageState(2, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
        D3DDevice_SetTextureStageState(2, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
        D3DDevice_SetTextureStageState(2, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
        D3DDevice_SetTextureStageState(2, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
        D3DDevice_SetTextureStageState(2, D3DTSS_MIPFILTER, 0);
        D3DDevice_SetTextureStageState(2, D3DTSS_MAXANISOTROPY, 0);
        D3DDevice_SetTextureStageState(2, D3DTSS_MIPMAPLODBIAS, 0);
        D3DDevice_SetTextureStageState(2, D3DTSS_MAXMIPLEVEL, 0);
        D3DDevice_SetTextureStageState(2, D3DTSS_COLORSIGN, 0);
        D3DDevice_SetTextureStageState(2, D3DTSS_ALPHAKILL, 0);
        memset(&g_484f68, 0, sizeof(g_484f68));
        g_484f68.PSTextureModes = 0x41e1;
        g_484f68.PSCombinerCount = 0x11002;
        g_484f68.PSConstant0[0] = 0xff0000;
        g_484f68.PSConstant1[0] = 0xff00;
        g_484f68.PSConstant1[1] = 0xff;
        g_484f68.PSRGBInputs[0] = 0x9010a02;
        g_484f68.PSRGBInputs[1] = 0xc200a02;
        g_484f68.PSRGBOutputs[0] = 0xc00;
        g_484f68.PSRGBOutputs[1] = 0xc00;
        g_484f68.PSFinalCombinerInputsABCD = 0xc;
    }
    else
    {
        function_14bc0((word)g_4858b8, 0, true);
        function_14f60(0, 1);
        memset(&g_484f68, 0, sizeof(g_484f68));
        g_484f68.PSTextureModes = 1;
        g_484f68.PSCombinerCount = 1;
        g_484f68.PSFinalCombinerInputsABCD = 8;
    }
    g_484f68.PSFinalCombinerInputsEFG = 0x1800;
    function_1ccf0(&g_484f68);
    function_1c590((s_shader_cache *)g_51f0f0, *(long *)(*(byte **)(g_485a80 + 0x5c) + 0x64), 0);
    function_1c710(g_51f0f0);
    return true;
}


// @retail 0x29370
bool __stdcall function_29370(void *context)
{
    if (!g_4b99b0[0] && !g_4b99b0[1]) return false;
    g_509400 = g_485ae0;
    function_14bc0(*(short *)(g_4b99b0 + 0x44), 0, false);
    for (short stage = 0; stage < 4; ++stage)
    {
        function_14f60(stage, *(short *)(g_4b99b0 + 0x38));
        D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
        D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
        D3DDevice_SetTextureStageState(stage, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
        D3DDevice_SetTextureStageState(stage, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
        D3DDevice_SetTextureStageState(stage, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
        D3DDevice_SetTextureStageState(stage, D3DTSS_MIPFILTER, 0);
        D3DDevice_SetTextureStageState(stage, D3DTSS_MAXANISOTROPY, 0);
        D3DDevice_SetTextureStageState(stage, D3DTSS_MIPMAPLODBIAS, 0);
        D3DDevice_SetTextureStageState(stage, D3DTSS_MAXMIPLEVEL, 0);
        D3DDevice_SetTextureStageState(stage, D3DTSS_COLORSIGN, 0);
        D3DDevice_SetTextureStageState(stage, D3DTSS_ALPHAKILL, 0);
    }
    function_0222d0(D3DRS_COLORWRITEENABLE, 0x10101);
    function_0222d0(D3DRS_ALPHATESTENABLE, 1);
    function_0222d0(D3DRS_SRCBLEND, D3DBLEND_ONE);
    function_0222d0(D3DRS_DESTBLEND, D3DBLEND_CONSTANTCOLOR);
    color3f blend = *(color3f *)(g_4b99b0 + 0x10);
    function_0222d0(D3DRS_BLENDCOLOR, pack_color3f(&blend));
    function_0222d0(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    function_0222d0(D3DRS_ZWRITEENABLE, 0);
    function_0222d0(D3DRS_ALPHABLENDENABLE, 0);
    function_0222d0(D3DRS_STENCILENABLE, 0);
    function_0222d0(D3DRS_ZENABLE, 0);
    function_0222d0(D3DRS_ZBIAS, 0);
    D3DPIXELSHADERDEF program;
    memset(&program, 0, sizeof(program));
    program.PSRGBOutputs[1] = 0x30d00;
    program.PSRGBOutputs[2] = 0x30d00;
    program.PSTextureModes = 0x8421;
    program.PSCombinerCount = 0x11003;
    program.PSRGBInputs[0] = 0x08200920;
    program.PSRGBOutputs[0] = 0x30c00;
    program.PSRGBInputs[1] = 0x0a200b20;
    program.PSRGBInputs[2] = 0x0c200d20;
    program.PSFinalCombinerInputsABCD = 0xd;
    g_484f68 = program;
    D3DDevice_SetPixelShaderProgram(&program);
    function_1c590((s_shader_cache *)g_51f0f0, *(long *)(*(byte **)(g_485a80 + 0x5c) + 0x64), 0);
    function_1c710(g_51f0f0);
    return true;
}
