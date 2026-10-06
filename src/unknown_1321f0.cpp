// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1321F0.CPP */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "globals.h"
#include "object_queries.h"
#include <math.h>
#include <string.h>
#include <xtl.h>

#define k_pi 3.14159265359f

struct s_1321f0
{
	byte unknown000[0x21c];
	dword flags[1];
};

// @retail 0x1321f0
bool function_1321f0(long index, s_1321f0 const *data)
{
	return (data->flags[index >> 5] & (1 << (index & 31))) != 0;
}

struct s_sort_entry
{
	short value;
	byte unknown02[0x1a - 2];
};

struct s_sort_context
{
	byte unknown000[0xa6c];
	short count;
	s_sort_entry entries[128];
	byte unknown176e[2];
	long order[0x200];
};

// @retail 0x134950
bool __stdcall function_134950(long a, long b, void const *context)
{
	s_sort_context const *data = (s_sort_context const *)context;
	return data->entries[a].value > data->entries[b].value;
}

struct s_132d60
{
	s_sort_context *context;
	byte unknown04[4];
	short cluster_index;
	byte unknown0a[0x2a68 - 0xa];
	point3f point;
	real radius;
};

extern long g_4e7414;
extern bool g_4e7411;
short __stdcall function_14a5b0(short cluster_index, point3f const *point, real radius, long maximum_count, short *clusters);
void sort_4byte(long *elements, unsigned long count, void *unused, bool (__stdcall *compare)(long, long, const void *), const void *context);

// @retail 0x132d60
void function_132d60(s_132d60 *data)
{
	real radius = data->radius;
	short clusters[0x200];
	short count = 0;

	if (data->cluster_index != NONE)
	{
		if (radius > 0.0f)
		{
			g_4e7414++;
			g_4e7411 = true;
			count = function_14a5b0(data->cluster_index, &data->point, radius, 0x200, clusters);
			g_4e7411 = false;
			if (count > 0x200)
			{
				count = 0x200;
			}
		}
		else
		{
			count = 1;
			clusters[0] = data->cluster_index;
		}
	}

	data->context->count = count;
	for (long i = 0; i < data->context->count; i++)
	{
		s_sort_entry *entry = &data->context->entries[i];
		entry->value = clusters[i];
		*(short *)&entry->unknown02[0] = 0;
		data->context->order[i] = i;
	}
	sort_4byte(data->context->order, data->context->count, &radius, function_134950, data->context);
}

/* ---- bit vector pools ---- */

/* the entry list of unknown_163110.cpp (its constructor and swap are there) */
class c_entry_list
{
public:
	c_entry_list(long maximum_count);
	~c_entry_list()
	{
		delete[] shorts_b;
		shorts_b = NULL;
		delete[] longs_a;
		longs_a = NULL;
		delete[] longs_c;
		longs_c = NULL;
		delete[] shorts_d;
		shorts_d = NULL;
	}
	void swap(short index0, short index1);
	bool add(long a, short b, long c, short d);
	short find(long a);

	long maximum_count;
	short count;
	short *shorts_b;
	long *longs_a;
	long *longs_c;
	short *shorts_d;
};

struct s_bit_vector_pool_sizes
{
	short unknown0;
	short list_sizes[4];
	short record_count;
};

struct s_bit_vector_pool
{
	void *context;
	s_location location;
	c_entry_list *lists[4];
	long indices[0x80];
	dword flags[0x10];
	dword pool[0x200];
	word pool_used;
	word entry_count;
	dword entries[0x200][4];
	dword flags2a60;
	short frustum_count;
	byte unknown2a66[2];
	point3f center;
	real radius;
	long mode;
	long query_type;
	byte unknown2a80[4];
	real plane_offset;
	plane3f plane;
	long counters[13];
	byte *records;
	void *filter;
	s_bit_vector_pool_sizes sizes;
};

/* the records are 0x9c bytes each */
#define k_bit_vector_pool_record_size 0x9c

struct s_frustum_set_view;
bool function_165010(s_frustum_set_view const *set, long section_index, point3f const *center, real radius, bool *contained);
long function_461c0(point3f const *a, point3f const *b, real radius);
bool function_c3110(long index);
bool function_c3140(long index);
extern s_record_pool *g_4e030c;
extern long g_4e0308;

struct s_132a30_light
{
	byte unknown00[0xc];
	long stamp;
	byte unknown10[8];
	point3f center;
	real radius;
	byte unknown28[0x110 - 0x28];
};

// @retail 0x132a30
void function_132a30(s_bit_vector_pool *data, long index, long section_index, bool visible)
{
	if (function_c3140(index))
	{
		s_132a30_light *light = &((s_132a30_light *)g_4e030c->data)[index & 0xffff];
		if (light->stamp != g_4e0308)
		{
			if (!visible)
			{
				point3f center = light->center;
				real radius = light->radius;
				visible = false;
				char intersects = true;
				if (data->query_type == 0)
					intersects = function_165010((s_frustum_set_view *)data->context, section_index, &center, radius, &visible);
				else if (data->query_type == 1)
					intersects = (char)function_461c0(&data->center, &center, data->radius + radius);
				if (!(char)intersects && (char)data->flags2a60 >= 0)
					return;
			}
			if (data->lists[1]->add(index, 0, 0, NONE))
				function_c3110(index);
		}
	}
}

// @retail 0x132e60
bool function_132e60(s_bit_vector_pool *data, point3f const *center, real radius,
	c_entry_list *list, long index, short section, bool visible, bool *contained, bool *first, bool *second)
{
	bool result = true;
	*first = true;
	*second = true;
	*contained = false;
	if (data->query_type == 0)
	{
		if (!visible)
			result = function_165010((s_frustum_set_view *)data->context, section, center, radius, contained);
		if (result && !*contained && data->mode == 0 && radius <= 2.5f)
			*contained = true;
	}
	else if (data->query_type == 1)
	{
		vector3f delta;
		vector3d_from_points3d(&data->center, center, &delta);
		real sum = radius + data->radius;
		if (!(sum * sum >= delta.k * delta.k + delta.i * delta.i + delta.j * delta.j))
			return false;
		double x = (double)data->center.x - center->x;
		double y = (double)data->center.y - center->y;
		double z = (double)data->center.z - center->z;
		*contained = sqrt(z * z + y * y + x * x) + radius <= data->radius;
	}
	if (result && list && (data->flags2a60 & 1))
	{
		bool found = list->find(index) != NONE;
		*second = found;
		*first = found;
	}
	return result;
}

struct s_132b80_frustum
{
	byte unknown00[0x74];
	plane3f plane;
	byte unknown84[0x1bc - 0x84];
};

void function_11bed0(s_location *location, point3f const *point);

// @retail 0x132b80
long function_132b80(s_bit_vector_pool *data, long mode, s_132b80_frustum const *frusta,
	short cluster, void *filter, point3f const *center, real radius, bool use_plane,
	real plane_offset, long count, dword flags)
{
	data->location.cluster_index = cluster;
	data->location.bsp_index = g_4686c4;
	data->location.leaf_index = NONE;
	data->mode = mode;
	data->flags2a60 = flags;
	data->frustum_count = (short)count;
	data->center = *center;
	data->radius = radius;
	data->plane_offset = plane_offset;
	if (use_plane)
	{
		data->flags2a60 |= 0x200;
		data->plane = frusta->plane;
		data->plane.d += plane_offset;
	}
	else
		data->flags2a60 &= ~0x200;
	void *destination = (byte *)data->context + 4;
	if (frusta != destination)
		memcpy(destination, frusta, count * sizeof(s_132b80_frustum));
	*(short *)data->context = (short)count;
	if (cluster == NONE)
		function_11bed0(&data->location, center);
	data->filter = filter;
	if (filter)
	{
		data->flags2a60 |= 1;
		for (long i = 0; i < 13; i++)
			data->counters[i] = 0;
	}
	if (data->mode == 0 && (char)data->flags2a60 < 0)
		data->query_type = 2;
	else if (count && (radius >= 3.0f || (data->flags2a60 & 0x100)))
		data->query_type = 0;
	else
		data->query_type = 1;
	return data->query_type;
}

/* takes enough dwords from the pool for a bit vector of this many bits */
// @retail 0x1332b0
dword *function_1332b0(long bit_count, s_bit_vector_pool *data)
{
	dword *result = NULL;
	if (bit_count > 0)
	{
		word used = data->pool_used;
		long count = (bit_count + 31) >> 5;
		result = data->pool;
		if (used + count < 0x200)
		{
			result = &data->pool[used];
			used += count;
			data->pool_used = used;
		}
	}
	return result;
}

// @retail 0x134300
long function_134300(s_bit_vector_pool *data, short bit)
{
	long index = data->entry_count++;
	if (index >= 0 && index < 0x200)
	{
		data->entries[index][3] = 0;
		data->entries[index][2] = 0;
		data->entries[index][1] = 0;
		data->entries[index][0] = 0;
		data->entries[(short)index][bit >> 5] |= 1 << (bit & 31);
		return index;
	}
	data->entry_count = 0x200;
	return 0;
}

// @retail 0x1348e0
void function_1348e0(s_bit_vector_pool *data)
{
	data->lists[0]->count = 0;
	data->lists[1]->count = 0;
	data->lists[2]->count = 0;
	data->lists[3]->count = 0;
	memset(data->flags, 0, sizeof(data->flags));
	memset(data->pool, 0, data->pool_used * sizeof(dword));
	data->pool_used = 0;
	data->entry_count = 0;
	for (long i = 0; i < 0x80; i++)
	{
		data->indices[i] = NONE;
	}
}

/* a bit field from seven flags */
// @retail 0x132b10
dword function_132b10(bool b, bool d, bool c, bool f, bool g, bool e, bool a)
{
	return (b ? 0x4000 : 0) | (c ? 0x2000 : 0) | (d ? 0x1000 : 0) | (e ? 0x800 : 0) | (f ? 0x400 : 0) | (g ? 0x200 : 0) | (a ? 0x8000 : 0);
}

/* a 16 bit real: sign, ten bits of mantissa, then five of exponent */
// @retail 0x135880
real function_135880(word value)
{
	dword sign = value >> 15;
	dword mantissa = (value >> 5) & 0x3ff;
	dword exponent = value & 0x1f;
	dword bits = (sign << 31) | (mantissa << 13) | ((exponent + 0x70) << 23);
	return *(real *)&bits;
}
// @retail 0x134980
void function_134980(s_bit_vector_pool *data, s_bit_vector_pool_sizes const *sizes, void *context)
{
	data->context = context;
	data->sizes = *sizes;
	data->lists[0] = new c_entry_list(data->sizes.list_sizes[0]);
	data->lists[2] = new c_entry_list(data->sizes.list_sizes[2]);
	data->lists[3] = new c_entry_list(data->sizes.list_sizes[3]);
	data->lists[1] = NULL;
	if (data->sizes.list_sizes[1])
	{
		data->lists[1] = new c_entry_list(data->sizes.list_sizes[1]);
	}
	data->pool_used = 0;
	data->entry_count = 0;
	data->records = NULL;
	if (data->sizes.record_count)
	{
		data->records = (byte *)operator new(data->sizes.record_count * k_bit_vector_pool_record_size);
	}
}

// @retail 0x134b20
void function_134b20(s_bit_vector_pool *data)
{
	delete data->lists[0];
	data->lists[0] = NULL;
	delete data->lists[1];
	data->lists[1] = NULL;
	delete data->lists[2];
	data->lists[2] = NULL;
	delete data->lists[3];
	data->lists[3] = NULL;
	operator delete(data->records);
	data->records = NULL;
}

/* the two bit vector pools, each with a scratch buffer carved from one
   virtual allocation (ai.cpp borrows the same buffer) */
extern byte *g_510c44;
s_bit_vector_pool g_547f88;
s_bit_vector_pool g_54aa68;

// @retail 0x131ff0
void function_131ff0(void)
{
	s_bit_vector_pool_sizes sizes0;
	s_bit_vector_pool_sizes sizes1;
	byte *buffer;

	sizes0.unknown0 = 1;
	sizes0.list_sizes[0] = 0x80;
	sizes0.list_sizes[1] = 0x40;
	sizes0.list_sizes[2] = 0x100;
	sizes0.list_sizes[3] = 0x180;
	sizes0.record_count = 0;
	sizes1.unknown0 = 6;
	sizes1.list_sizes[0] = 0x40;
	sizes1.list_sizes[1] = 1;
	sizes1.list_sizes[2] = 0x80;
	sizes1.list_sizes[3] = 0x60;
	sizes1.record_count = 0x100;

	buffer = (byte *)VirtualAlloc(NULL, 0x452e8, MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
	if (!buffer)
	{
		GetLastError();
	}
	g_510c44 = buffer;

	function_134980(&g_547f88, &sizes0, g_510c44);
	function_134980(&g_54aa68, &sizes1, g_510c44 + 0x22974);
}

// @retail 0x1320b0
void function_1320b0(void)
{
	function_134b20(&g_547f88);
	function_134b20(&g_54aa68);

	if (g_510c44)
	{
		if (!VirtualFree(g_510c44, 0, MEM_RELEASE))
		{
			GetLastError();
		}
		g_510c44 = NULL;
	}
}

struct s_134240_object
{
	long definition_index;
};

struct s_134240_object_header
{
	byte unknown00[8];
	s_134240_object *object;
};

/* moves the objects whose model has nodes to the front of the third list */
// @retail 0x134240
void function_134240(s_bit_vector_pool *data)
{
	long kept = 0;
	for (long i = 0; i < data->lists[2]->count; i++)
	{
		long object_index = data->lists[2]->longs_a[(short)i];
		if (object_index == NONE)
		{
			continue;
		}
		s_134240_object *object = ((s_134240_object_header *)g_4e0300->data)[object_index & 0xffff].object;
		if (!object || object->definition_index == NONE)
		{
			continue;
		}
		byte *definition = g_4e3b44[object->definition_index & 0xffff].bytes;
		if (!definition || *(long *)(definition + 0x38) == NONE)
		{
			continue;
		}
		byte *model = g_4e3b44[*(long *)(definition + 0x38) & 0xffff].bytes;
		if (!model || *(long *)(model + 4) == NONE)
		{
			continue;
		}
		byte *render_model = g_4e3b44[*(long *)(model + 4) & 0xffff].bytes;
		if (*(long *)(render_model + 0x74) > 0)
		{
			if (i != kept)
			{
				data->lists[2]->swap((short)i, (short)kept);
			}
			kept++;
		}
	}
}

static inline real transition_cosine(real x)
{
	real t;
	if (0.0f > x)
	{
		t = 0.0f;
	}
	else if (x > 1.0f)
	{
		t = 1.0f;
	}
	else
	{
		t = x;
	}
	return (real)(0.5f - cos(t * k_pi) * 0.5f);
}

// @retail 0x134c50
real function_134c50(real x)
{
	return 0.0f > transition_cosine(x) ? 0.0f : (transition_cosine(x) > 1.0f ? 1.0f : transition_cosine(x));
}

/* which sides of the pool's plane a sphere touches (both when the plane is
   off or the sphere crosses it) */
// @retail 0x132fd0
void function_132fd0(s_bit_vector_pool const *data, point3f const *center, bool *behind, bool *in_front, real radius)
{
	if (data->flags2a60 & 0x200)
	{
		real distance = plane_distance_to_point(&data->plane, center);

		*in_front = false;
		*behind = false;
		if (0.0f > distance)
		{
			*behind = true;
		}
		if (distance > 0.0f)
		{
			*in_front = true;
		}
		if ((real)fabs(distance) <= radius)
		{
			*in_front = true;
			*behind = true;
		}
	}
	else
	{
		*in_front = true;
		*behind = true;
	}
}

long g_4b9ef0;
long g_4b9f8c;
real g_4b9ffc;
bool g_4ba004;

/* the render flags of an entry from its own flags and the pool's */
// @retail 0x1332f0
dword function_1332f0(s_bit_vector_pool const *data, word flags)
{
	dword result = ((((flags >> 2) & 0x2c00) | (flags & 0x4000)) >> 9) | ((flags & 0x200) << 5);

	if ((char)data->flags2a60 < 0)
	{
		result |= 1;
	}
	if (g_4b9ef0 == 3)
	{
		long tag_index = g_4b9f8c;

		if (tag_index != NONE && g_4b9ffc == 0.0f && !(flags & 0x400) && !(*g_4e3b44[tag_index & 0xffff].bytes & 2))
		{
			result |= 0x100;
		}
		if (!g_4ba004 || !(flags & 0x400))
		{
			result |= 0x80;
		}
	}

	return result;
}
