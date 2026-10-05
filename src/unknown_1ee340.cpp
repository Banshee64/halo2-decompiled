#include "unknown_11c920.h"
#include "globals.h"
#include <xmmintrin.h>
#include <float.h>
#include "unknown_1eb350.h"
#include "havok_reference.h"

struct c_transformed_point
{
	__m128 value;
	void transform(const void *matrix, const __m128 *point);
};

void __cdecl function_2fe730(const void *points, long count, long stride, void *output);

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
	virtual void v3();
	virtual void get_bounds(const void *matrix, real expansion, void *output);
};

// @retail 0x1eded0
void c_vertex_shape::get_bounds(const void *matrix, real expansion, void *output)
{
	c_transformed_point points[8];
	for (long i = 0; i < vertex_count; i++)
		points[i].transform(matrix, &vertices()[i]);
	function_2fe730(points, vertex_count, sizeof(c_transformed_point), output);
}

// @retail 0x1edf40
void c_vertex_shape::v3()
{
	__asm int 3
	__assume(0);
}

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
	virtual long next_index(dword index);
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

// @retail 0x1ee4a0
long c_count_interface::next_index(dword index)
{
	if (index != NONE && index < (dword)(get_count() - 1))
		return index + 1;
	return NONE;
}

struct c_shape_owner : c_shape_library_base_a
{
	byte field_8[0x14 - 8];
	c_havok_reference_counted *object;
	virtual ~c_shape_owner();
};

c_shape_owner *g_51e9d4;

// @retail 0x1ee530 deleting c_shape_owner

// @retail 0x1ee560
c_shape_owner::~c_shape_owner()
{
	havok_reference_remove(object);
	object = 0;
	g_51e9d4 = 0;
}

#include "unknown_1efac0.h"

struct c_shape_global_owner : c_a
{
	virtual ~c_shape_global_owner();
	void operator delete(void *block)
	{
		g_480118->allocate((long)block, ((c_shape_global_owner *)block)->flags, 0x22);
	}
};

c_shape_global_owner *g_51e9d0;

// @retail 0x1eefe0 deleting
c_shape_global_owner::~c_shape_global_owner()
{
	g_51e9d0 = 0;
}
