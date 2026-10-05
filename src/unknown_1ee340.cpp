#include "unknown_11c920.h"
#include "globals.h"
#include <xmmintrin.h>
#include <float.h>

// @flags /O2 /arch:SSE /Gr

/* a shape holding an array of 16-byte aligned vertices starting at +0x20 */
struct c_vertex_shape
{
	byte unknown04[0x10];
	long vertex_count;
	byte unknown18[8];
	real vertices_raw[4];

	__m128 *vertices() { return (__m128 *)this->vertices_raw; }

	virtual void get_supporting_vertex(const __m128 *direction, __m128 *out);
	virtual void gather_vertices(const word *indices, long count, __m128 *out);
	virtual void get_first_vertex(__m128 *out);
	virtual void v3() {}
};

// @retail 0x1ee340
void c_vertex_shape::get_first_vertex(__m128 *out)
{
	*out = vertices()[0];
}

// @retail 0x1ee350
void c_vertex_shape::get_supporting_vertex(const __m128 *direction, __m128 *out)
{
	long best = 0;
	real best_dot = -FLT_MAX;
	for (long i = 0; i < vertex_count; i++)
	{
		__m128 p = _mm_mul_ps(vertices()[i], *direction);
		__m128 sum = _mm_add_ss(_mm_shuffle_ps(p, p, 0x55), p);
		sum = _mm_add_ss(_mm_shuffle_ps(p, p, 0xaa), sum);
		real dot;
		_mm_store_ss(&dot, sum);
		if (dot > best_dot)
		{
			best_dot = dot;
			best = i;
		}
	}
	*out = vertices()[best];
}

// @retail 0x1ee3d0
void c_vertex_shape::gather_vertices(const word *indices, long count, __m128 *out)
{
	for (long i = count - 1; i >= 0; i--)
	{
		out[i] = vertices()[indices[i]];
	}
}

struct s_tag_block_owner
{
	byte unknown00[0x5c];
	s_tag_block_entry *entries;
};

struct s_tag_instance_ref
{
	byte unknown00[8];
	s_tag_block_owner *data;
	byte unknown0c[4];
};

struct s_lookup_source
{
	byte unknown00[0x18];
	short index;
	short pad1a;
	long tag_index;
};

short g_54e898;

// @retail 0x1ee410
void function_1ee410(const s_lookup_source *source, short *result)
{
	if (source->tag_index != NONE)
	{
		s_tag_instance_ref *tags = (s_tag_instance_ref *)g_4e3b44;
		*result = tags[(word)source->tag_index].data->entries[source->index].value10;
	}
	else if (source->index != NONE)
	{
		*result = g_4e0348->entries[source->index].value08;
	}
	else
	{
		*result = g_54e898;
	}
}

struct s_count_entry
{
	dword unknown0;
	dword flags;
};

struct s_count_owner
{
	byte unknown00[0x28];
	long count;
	s_count_entry *entries;
};


struct c_count_interface
{
	byte unknown04[8];
	s_count_owner *owner;

	virtual void v0() {}
	virtual void v1() {}
	virtual void v2() {}
	virtual void v3() {}
	virtual void v4() {}
	virtual void v5() {}
	virtual void v6() {}
	virtual void v7() {}
	virtual void v8() {}
	virtual void v9() {}
	virtual long get_count();
	virtual long test_count();
};

// @retail 0x1ee470
long c_count_interface::get_count()
{
	s_count_owner *o = owner;
	long count = o->count;
	if (count > 0)
	{
		const s_count_entry *last = &o->entries[count - 1];
		if (last->flags & 0x10)
		{
			count--;
		}
	}
	return count;
}

// @retail 0x1ee490
long c_count_interface::test_count()
{
	return get_count() > 0 ? 0 : -1;
}
