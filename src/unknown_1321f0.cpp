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
	short find(long arg_1);

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
	s_location field_4;
	c_entry_list *lists[4];
	long indices[0x80];
	dword flags[0x10];
	dword pool[0x200];
	word pool_used;
	word entry_count;
	dword entries[0x200][4];
	dword flags2a60;
	short field_2a64;
	byte field_2a66[2];
	point3f field_2a68;
	real field_2a74;
	long field_2a78;
	long field_2a7c;
	long field_2a80;
	real field_2a84;
	plane3f plane;
	dword field_2a98[13];
	byte *records;
	c_entry_list *field_2ad0;
	s_bit_vector_pool_sizes sizes;
};

/* the records are 0x9c bytes each */
#define k_bit_vector_pool_record_size 0x9c

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
inline real function_135880(word value)
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

struct s_frustum_set_view;
bool function_165010(s_frustum_set_view const *arg_1, long arg_2, point3f const *arg_3, real arg_4, bool *arg_5);
void function_11bed0(s_location *arg_1, point3f const *arg_2);

PRIVATE __forceinline real function_132e61(point3f const *arg_1, point3f const *arg_2)
{
	vector3f local_1;
	vector3d_from_points3d(arg_1, arg_2, &local_1);
	return (real)sqrt(local_1.i * local_1.i + local_1.j * local_1.j + local_1.k * local_1.k);
}

// @retail 0x132e60
bool function_132e60(s_bit_vector_pool *arg_1, real arg_2, c_entry_list *arg_3, long arg_4, short arg_5, bool arg_6, bool *arg_7, bool *arg_8, bool *arg_9, point3f const *arg_10)
{
	bool local_1 = true;
	*arg_8 = true;
	*arg_9 = true;
	*arg_7 = false;
	if (arg_1->field_2a7c == 0)
	{
		if (!arg_6)
			local_1 = function_165010((s_frustum_set_view const *)arg_1->context, arg_5, arg_10, arg_2, arg_7);
		else
			local_1 = true;
		if (local_1 && !*arg_7 && arg_1->field_2a78 == 0 && arg_2 <= 2.5f)
		{
			*arg_7 = true;
		}
	}
	else if (arg_1->field_2a7c == 1)
	{
		real local_3 = arg_2 + arg_1->field_2a74;
		vector3f local_2;
		vector3d_from_points3d(&arg_1->field_2a68, arg_10, &local_2);
		if (local_2.j * local_2.j + local_2.i * local_2.i + local_2.k * local_2.k <= local_3 * local_3)
		{
			local_1 = true;
			*arg_7 = function_132e61(arg_10, &arg_1->field_2a68) + arg_2 <= arg_1->field_2a74;
		}
		else
		{
			return false;
		}
	}
	if (local_1 && arg_3 && (arg_1->flags2a60 & 1))
	{
		*arg_8 = *arg_9 = arg_3->find(arg_4) != NONE;
	}
	return local_1;
}

// @retail 0x132b80
long function_132b80(s_bit_vector_pool *arg_1, long arg_2, void const *arg_3, long arg_4, c_entry_list *arg_5, point3f const *arg_6, real arg_7, bool arg_8, real arg_9, dword arg_11, long arg_10)
{
	(void)&arg_1;
	long const *local_2 = &arg_4;
	arg_1->field_4.cluster_index = (short)*local_2;
	arg_1->field_4.bsp_index = g_4686c4;
	arg_1->field_4.leaf_index = NONE;
	arg_1->field_2a78 = arg_2;
	arg_1->flags2a60 = arg_11;
	arg_1->field_2a64 = (short)arg_10;
	arg_1->field_2a68 = *arg_6;
	arg_1->field_2a74 = arg_7;
	arg_1->field_2a84 = arg_9;
	if (arg_8)
	{
		arg_1->flags2a60 |= 0x200;
		arg_1->plane = *(plane3f const *)((byte const *)arg_3 + 0x74);
		arg_1->plane.d += arg_9;
	}
	else
	{
		arg_1->flags2a60 &= ~0x200;
	}
	void *local_1 = (byte *)arg_1->context + 4;
	if (arg_3 != local_1)
	{
		memcpy(local_1, arg_3, arg_10 * 0x1bc);
	}
	*(short *)arg_1->context = (short)arg_10;
	if (*local_2 == NONE)
	{
		function_11bed0(&arg_1->field_4, arg_6);
	}
	arg_1->field_2ad0 = arg_5;
	if (arg_5)
	{
		arg_1->flags2a60 |= 1;
		arg_1->field_2a98[0] = 0;
		arg_1->field_2a98[1] = 0;
		arg_1->field_2a98[2] = 0;
		arg_1->field_2a98[3] = 0;
		arg_1->field_2a98[4] = 0;
		arg_1->field_2a98[5] = 0;
		arg_1->field_2a98[6] = 0;
		arg_1->field_2a98[7] = 0;
		arg_1->field_2a98[8] = 0;
		arg_1->field_2a98[9] = 0;
		arg_1->field_2a98[10] = 0;
		arg_1->field_2a98[11] = 0;
		arg_1->field_2a98[12] = 0;
	}
	if (arg_1->field_2a78 == 0 && (char)arg_1->flags2a60 < 0)
	{
		arg_1->field_2a7c = 2;
	}
	else if (arg_10 != 0 && (arg_7 >= 3.0f || (arg_1->flags2a60 & 0x100)))
	{
		arg_1->field_2a7c = 0;
	}
	else
	{
		arg_1->field_2a7c = 1;
	}
	return arg_1->field_2a7c;
}

extern long g_4e7c1c;
long g_4e7c20[0x400];
bool function_16e210(long cluster_index, long value);

struct s_132780
{
	long field_0;
	long field_4[6];
	long field_1c[6];
	byte *field_34;
};

class c_1648d0
{
public:
	bool function_1648d0(void const *arg_1, void const *arg_2, void *arg_3, bool arg_4) const;
};

// @retail 0x132780
bool function_132780(s_sort_context const *arg_1, s_sort_context const *arg_2, long arg_3, long arg_4, long arg_5, long arg_6, long arg_7, s_132780 *arg_8)
{
	(void)&arg_1;
	(void)&arg_2;
	(void)&arg_3;
	(void)&arg_4;
	(void)&arg_5;
	s_sort_entry const *local_1 = &arg_1->entries[arg_1->order[arg_6]];
	s_sort_entry const *local_2 = &arg_2->entries[arg_2->order[arg_3]];
	bool local_3 = false;
	for (long local_4 = 0; local_4 < ((short const *)local_2)[arg_7 + 1]; local_4++)
	{
		byte const *local_5 = (byte const *)arg_2 + 0x1974 + (((short const *)local_2)[arg_7 + 7] + local_4) * 0x108;
		for (long local_6 = 0; local_6 < ((short const *)local_1)[arg_4 + 1]; local_6++)
		{
			byte const *local_7 = (byte const *)arg_1 + 0x1974 + (((short const *)local_1)[7] + local_6) * 0x108;
			if (arg_8->field_0 >= arg_5)
				return false;
			byte *local_8 = arg_8->field_34 + arg_8->field_0 * 0x9c;
			if (((c_1648d0 const *)local_7)->function_1648d0((byte const *)arg_2 + 4 + *(long const *)local_5 * 0x1bc, local_5, local_8, false))
			{
				long local_9 = arg_8->field_0++;
				*(long *)(local_8 + 0x94) = ((short const *)local_2)[arg_7 + 7] + local_4;
				*(long *)(local_8 + 0x98) = arg_7;
				if (arg_8->field_1c[arg_7] == NONE)
					arg_8->field_1c[arg_7] = local_9;
				arg_8->field_4[arg_7]++;
				local_3 = true;
			}
		}
	}
	return local_3;
}

PRIVATE __forceinline bool function_134391(c_entry_list *arg_1, long arg_2, word arg_3, short arg_4)
{
	if ((word)arg_1->count < arg_1->maximum_count - 1)
	{
		arg_1->longs_a[(word)arg_1->count] = arg_2;
		arg_1->shorts_b[(word)arg_1->count] = arg_3;
		arg_1->longs_c[(word)arg_1->count] = 0;
		arg_1->shorts_d[(word)arg_1->count] = arg_4;
		arg_1->count++;
		return true;
	}
	return false;
}

// @retail 0x134390
void function_134390(s_bit_vector_pool *arg_1)
{
	(void)&arg_1;
	if (arg_1->flags2a60 & 0x20)
		return;
	for (short local_1 = 0; local_1 < ((s_sort_context *)arg_1->context)->count; local_1++)
	{
		s_sort_entry *local_2 = &((s_sort_context *)arg_1->context)->entries[(short)local_1];
		byte *local_3 = *(byte **)((byte *)g_4e0348 + 0xa0) + local_2->value * 0xb0;
		arg_1->flags[local_2->value >> 5] |= 1 << (local_2->value & 31);
		((byte *)arg_1->indices)[local_2->value] = (byte)local_1;
		if (*(word *)(local_3 + 0x24) > 0)
		{
			byte *local_4 = *(byte **)((byte *)g_4e0348 + 0xa0) + local_2->value * 0xb0;
			point3f local_5;
			local_5.x = (*(real *)(local_4 + 0x54) + *(real *)(local_4 + 0x58)) * 0.5f;
			local_5.y = (*(real *)(local_4 + 0x5c) + *(real *)(local_4 + 0x60)) * 0.5f;
			local_5.z = (*(real *)(local_4 + 0x64) + *(real *)(local_4 + 0x68)) * 0.5f;
			real local_6 = *(real *)(local_4 + 0x58) - local_5.x;
			real local_7 = *(real *)(local_4 + 0x60) - local_5.y;
			real local_8 = *(real *)(local_4 + 0x68) - local_5.z;
			real local_9 = (real)sqrt(local_8 * local_8 + local_7 * local_7 + local_6 * local_6);
			bool local_10, local_11;
			function_132fd0(arg_1, &local_5, &local_10, &local_11, local_9);
			if (arg_1->field_2ad0 && (arg_1->flags2a60 & 1))
			{
				c_entry_list *local_12 = *(c_entry_list **)((byte *)arg_1->field_2ad0 + 0xc);
				short local_13 = NONE;
				for (long local_14 = 0; local_14 < (word)local_12->count; local_14++)
					if (local_12->longs_a[local_14] == local_2->value)
					{
						local_13 = (short)local_14;
						break;
					}
				if (local_13 == NONE)
					continue;
			}
			dword local_15 = function_132b10(local_11, true, true, function_16e210(local_2->value, g_4b9f8c), false, false, local_10);
			function_134391(arg_1->lists[0], local_2->value, (word)local_15, NONE);
		}
		for (long local_16 = 0; local_16 < *(long *)(local_3 + 0x98); local_16++)
		{
			short local_17 = (*(short **)(local_3 + 0x9c))[local_16];
			long local_18 = (word)local_17;
			byte *local_19 = *(byte **)((byte *)g_4e0348 + 0x144) + local_18 * 0x58;
			byte *local_20 = *(byte **)((byte *)g_4e0348 + 0x13c) + *(short *)(local_19 + 0x34) * 0xc8;
			if (g_4e7c20[local_17] != g_4e7c1c)
			{
				c_entry_list *local_21 = arg_1->field_2ad0;
				if (local_21)
					local_21 = *(c_entry_list **)((byte *)local_21 + 0x18);
				if (*(word *)(local_20 + 0x24) > 0)
				{
					bool local_22, local_23, local_24;
					point3f const *local_25 = (point3f const *)(local_19 + 0x3c);
					if (function_132e60(arg_1, *(real *)(local_19 + 0x48), local_21, local_18, (short)local_1, false, &local_22, &local_23, &local_24, local_25) || (char)arg_1->flags2a60 < 0)
					{
						short local_26 = NONE;
						bool local_27, local_28;
						function_132fd0(arg_1, local_25, &local_27, &local_28, *(real *)(local_19 + 0x48));
						dword local_29 = function_132b10(local_28, local_23, local_24, function_16e210(local_2->value, g_4b9f8c), false, local_22, local_27);
						if (!(local_29 & 0x800))
							local_26 = (short)function_134300(arg_1, (short)local_1);
						if (function_134391(arg_1->lists[3], local_18, (word)local_29, local_26))
							g_4e7c20[local_17] = g_4e7c1c;
					}
				}
			}
			else
			{
				c_entry_list *local_30 = arg_1->lists[3];
				long local_31 = NONE;
				bool local_32 = false;
				for (long local_33 = 0; local_33 < local_30->count && !local_32; local_33++)
				{
					local_32 = local_30->longs_a[(short)local_33] == local_18;
					local_31 = local_33;
				}
				if (!(local_30->shorts_b[(short)local_31] & 0x800))
					arg_1->entries[local_30->shorts_d[(short)local_31]][(short)local_1 >> 5] |= 1 << ((short)local_1 & 31);
				if (function_16e210(local_2->value, g_4b9f8c))
					arg_1->lists[3]->shorts_b[(short)local_31] |= 0x400;
			}
		}
	}
}
