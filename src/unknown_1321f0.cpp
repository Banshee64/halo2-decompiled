// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1321F0.CPP */

#include "cseries.h"
#include "real_math.h"
#include <string.h>

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
	real_point3d point;
	real radius;
};

extern long g_4e7414;
extern bool g_4e7411;
short __stdcall function_14a5b0(short cluster_index, real_point3d const *point, real radius, long maximum_count, short *clusters);
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

struct s_bit_vector_owner
{
	byte unknown00[4];
	short count;
};

struct s_bit_vector_pool
{
	byte unknown000[0xc];
	s_bit_vector_owner *owners[4];
	long indices[0x80];
	dword flags[0x10];
	dword pool[0x200];
	word pool_used;
	word entry_count;
	dword entries[0x200][4];
};

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
	data->owners[0]->count = 0;
	data->owners[1]->count = 0;
	data->owners[2]->count = 0;
	data->owners[3]->count = 0;
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