// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_17B850.CPP: the contrails (entry 39 of the lifecycle table) */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "object_queries.h"

s_data_array *g_4ea940;
s_data_array *g_4ea944;

struct s_bsp3d;
extern s_bsp3d *g_4e033c;
long function_14a280(s_bsp3d *bsp, real_point3d *point, long index);
bool __stdcall function_bab40(long object_index, long name, real *value);
void function_17c0e0(long contrail_index, long count, bool flag);
void __stdcall function_17c540(long contrail_index, real dt);

/* a contrail (g_4ea944, 0x48 bytes) */
struct s_contrail_datum
{
	short salt;
	word flag0 : 1;
	word : 15;
	long tag_index;
	long object_index;
	short marker_index;
	byte unknown0e[2];
	long marker_name;
	real scale;
	short sequence_index;
	short frame_index;
	real unknown1c;
	real unknown20;
	real point_delay;
	real frame_time;
	real unknown2c;
	short point_counts[4];
	long point_indices[4];
};

/* a contrail point (g_4ea940, 0x38 bytes) */
struct s_contrail_point_datum
{
	short salt;
	byte unknown02[0x12];
	s_location location;
	real_point3d position;
	byte unknown28[0xc];
	long next_index;
};

/* the contrail tag ('cont') */
struct s_contrail_definition
{
	byte unknown00[2];
	byte flags02;
	byte flags03;
	real point_rate;
	byte unknown08[0x24 - 8];
	real unknown24;
	real unknown28;
	real frame_rate;
	byte unknown30[4];
	long bitmap_index;
	short sequence_first;
	short sequence_count;
};

struct s_contrail_bitmap_sequence
{
	byte unknown00[0x22];
	short frame_count;
	byte unknown24[0x3c - 0x24];
};

struct s_contrail_bitmap
{
	byte unknown00[0x3c];
	long sequence_count;
	s_contrail_bitmap_sequence *sequences;
};

struct s_contrail_object_marker
{
	byte unknown00[0x10];
	long name;
	byte unknown14[4];
};

struct s_contrail_object_definition
{
	byte unknown00[0x98];
	s_contrail_object_marker *markers;
};

struct s_contrail_object_header
{
	byte unknown00[8];
	long *object;
};

struct s_contrail_structure_leaf
{
	short cluster_index;
	byte unknown02[6];
};

struct s_contrail_structure_bsp
{
	byte unknown00[0x30];
	s_contrail_structure_leaf *leaves;
};

void contrails_dispose(void);
void function_17bf80(s_contrail_datum *contrail);
short function_17c040(long contrail_index, real dt);
void function_17c880(long contrail_index);

#define CONTRAIL(index) ((s_contrail_datum *)g_4ea944->data + ((index) & 0xffff))
#define CONTRAIL_POINT(index) ((s_contrail_point_datum *)g_4ea940->data + ((index) & 0xffff))
#define CONTRAIL_DEFINITION(index) ((s_contrail_definition *)g_4e3b44[(index) & 0xffff].bytes)

static inline void contrails_make_valid(s_data_array *data)
{
	data->valid = true;
	data_delete_all(data);
}

// @retail 0x17b7b0
void contrails_initialize(void)
{
	g_4ea944 = data_new_inlined("contrail", 0x100, sizeof(s_contrail_datum), 0, g_510c2c);
	g_4ea940 = data_new_inlined("contrail point", 0x400, sizeof(s_contrail_point_datum), 0, g_510c2c);
	if (!g_4ea944 || !g_4ea940)
		contrails_dispose();
}

// @retail 0x17b850
void contrails_dispose(void)
{
	if (g_4ea940)
	{
		g_4ea940 = 0;
	}
	if (g_4ea944)
	{
		g_4ea944 = 0;
	}
}

// @retail 0x17b870
void contrails_initialize_for_new_map(void)
{
	contrails_make_valid(g_4ea944);
	contrails_make_valid(g_4ea940);
}

// @retail 0x17b8a0
void contrails_dispose_from_old_map(void)
{
	g_4ea940->valid = false;
	g_4ea944->valid = false;
}

// @retail 0x17b8c0
void contrails_delete_all(void)
{
	s_data_array *array = g_4ea944;
	long index = NONE;

	for (;;)
	{
		index = data_next_absolute_index_inlined(array, index + 1);
		if (index == NONE)
			break;

		s_contrail_datum *contrail = (s_contrail_datum *)(array->data + array->size * index);
		long contrail_index = (contrail->salt << 16) | index;

		if (contrail->object_index != NONE)
		{
			function_17c880(contrail_index);
		}
		else
		{
			function_17c880(contrail_index);
			datum_delete(array, contrail_index);
		}
	}
}

// @retail 0x17b940
void contrails_update_locations(void)
{
	long contrail_index = data_datum_index(g_4ea944, data_next_absolute_index(g_4ea944, 0));

	while (contrail_index != NONE)
	{
		s_contrail_datum *contrail = CONTRAIL(contrail_index);

		for (long i = 0; i < 4; i++)
		{
			for (long point_index = contrail->point_indices[i]; point_index != NONE; )
			{
				s_contrail_point_datum *point = CONTRAIL_POINT(point_index);

				if (point->location.cluster_index != NONE)
				{
					if (g_4686c4 == NONE)
					{
						point->location.leaf_index = NONE;
						point->location.cluster_index = NONE;
					}
					else
					{
						long leaf_index = function_14a280(g_4e033c, &point->position, 0);

						point->location.leaf_index = leaf_index;
						point->location.cluster_index = leaf_index != NONE ? ((s_contrail_structure_bsp *)g_4e0348)->leaves[leaf_index].cluster_index : NONE;
					}
					point->location.bsp_index = g_4686c4;
				}
				point_index = point->next_index;
			}
		}
		contrail_index = data_datum_index(g_4ea944, data_find_index(g_4ea944, contrail_index == NONE ? 0 : (contrail_index & 0xffff) + 1));
	}
}

// @retail 0x17bab0
long contrail_new(long object_index, short marker_index, long tag_index)
{
	long contrail_index = NONE;

	if (tag_index != NONE && CONTRAIL_DEFINITION(tag_index)->bitmap_index != NONE)
	{
		contrail_index = datum_new(g_4ea944);
		if (contrail_index != NONE)
		{
			s_contrail_datum *contrail = CONTRAIL(contrail_index);
			long *object = ((s_contrail_object_header *)g_4e0300->data)[object_index & 0xffff].object;
			s_contrail_object_marker *marker = &((s_contrail_object_definition *)g_4e3b44[*object & 0xffff].bytes)->markers[marker_index];

			*(word *)((byte *)contrail + 2) = 0;
			contrail->tag_index = tag_index;
			contrail->object_index = object_index;
			contrail->marker_index = marker_index;
			contrail->marker_name = marker->name;
			contrail->sequence_index = NONE;
			function_17bf80(contrail);
			contrail->unknown1c = 0.0f;
			contrail->unknown20 = 0.0f;
			for (long i = 0; i < 4; i++)
			{
				contrail->point_counts[i] = 0;
				contrail->point_indices[i] = NONE;
			}
			if (function_bab40(contrail->object_index, contrail->marker_name, &contrail->scale))
			{
				contrail->flag0 = true;
				function_17c0e0(contrail_index, 1, true);
			}
		}
	}
	return contrail_index;
}

// @retail 0x17bbd0
void contrail_update(long contrail_index, bool detach, real dt)
{
	s_contrail_datum *contrail = CONTRAIL(contrail_index);

	if (detach)
	{
		contrail->flag0 = false;
		contrail->object_index = NONE;
	}
	if (contrail->flag0)
	{
		short count = function_17c040(contrail_index, dt);

		function_17c0e0(contrail_index, count < 1 ? 1 : count, false);
	}
	contrail->unknown2c += dt;
}

// @retail 0x17bc40
void contrails_update(real dt)
{
	long contrail_index = data_datum_index(g_4ea944, data_next_absolute_index(g_4ea944, 0));

	while (contrail_index != NONE)
	{
		s_contrail_datum *contrail = CONTRAIL(contrail_index);
		real elapsed = dt - contrail->unknown2c;
		s_contrail_definition *definition = CONTRAIL_DEFINITION(contrail->tag_index);

		contrail->unknown2c = 0.0f;
		if (contrail->object_index != NONE)
		{
			bool active = function_bab40(contrail->object_index, contrail->marker_name, &contrail->scale);

			if (active != contrail->flag0)
			{
				real scale = contrail->scale;

				contrail->scale = 0.0f;
				function_17c0e0(contrail_index, 1, true);
				contrail->scale = scale;
			}
			if (active)
			{
				contrail->flag0 = true;
				function_17c0e0(contrail_index, function_17c040(contrail_index, dt), true);
			}
			else
			{
				contrail->flag0 = false;
			}
		}
		if (elapsed > 0.0f && (definition->unknown24 > 0.0f || definition->unknown28 > 0.0f))
		{
			real remaining = elapsed;
			real rate = definition->frame_rate;

			if (rate > 0.0f)
			{
				if (definition->flags02 & 0x20)
					rate = contrail->scale * rate;
				for (;;)
				{
					real step = 1.0f / rate - contrail->frame_time;

					if (!(remaining >= step))
					{
						contrail->frame_time += remaining;
						break;
					}
					function_17bf80(contrail);
					remaining -= step;
					if (!(remaining > 0.0f))
						break;
				}
			}
			else
			{
				contrail->frame_time += elapsed;
			}

			real a = definition->unknown24;
			if (definition->flags03 & 1)
				a = contrail->scale * a;
			contrail->unknown1c -= a * elapsed;

			real b = definition->unknown28;
			if (definition->flags03 & 2)
				b = contrail->scale * b;
			contrail->unknown20 += b * elapsed;
		}
		function_17c540(contrail_index, dt);

		short i;
		for (i = 0; i < 4; i++)
		{
			if (contrail->point_indices[i] != NONE)
				break;
		}
		if (i == 4 && contrail->object_index == NONE)
		{
			function_17c880(contrail_index);
			datum_delete(g_4ea944, contrail_index);
		}
		contrail_index = data_datum_index(g_4ea944, data_find_index(g_4ea944, contrail_index == NONE ? 0 : (contrail_index & 0xffff) + 1));
	}
}

// @retail 0x17bef0
real function_17bef0(dword flags, real lower, real scale, real upper, byte bit)
{
	real base = lower;

	if (flags & (1 << bit))
		base = scale * lower;

	real range = upper - lower;

	if (flags & (1 << (bit + 1)))
		range = range * scale;
	return _real_random(&g_4e7408->seed, __FILE__, __LINE__) * range + base;
}

// @retail 0x17bf80
void function_17bf80(s_contrail_datum *contrail)
{
	s_contrail_definition *definition = CONTRAIL_DEFINITION(contrail->tag_index);

	contrail->frame_time = 0.0f;
	if (definition->bitmap_index != NONE)
	{
		s_contrail_bitmap *bitmap = (s_contrail_bitmap *)g_4e3b44[definition->bitmap_index & 0xffff].bytes;
		short sequence_index;
		short frame_index;

		contrail->frame_index++;
		sequence_index = contrail->sequence_index;
		frame_index = contrail->frame_index;
		if (sequence_index < 0 || sequence_index >= bitmap->sequence_count || frame_index < 0 || frame_index >= bitmap->sequences[sequence_index].frame_count)
		{
			short first = definition->sequence_first;
			short last = first + definition->sequence_count;

			contrail->sequence_index = (short)(first + (((last - first) * random_next(&g_4e7408->seed)) >> 16));
			contrail->frame_index = 0;
		}
	}
	else
	{
		contrail->frame_index = 0;
		contrail->sequence_index = 0;
	}
}

// @retail 0x17c040
short function_17c040(long contrail_index, real dt)
{
	s_contrail_datum *contrail = CONTRAIL(contrail_index);
	s_contrail_definition *definition = CONTRAIL_DEFINITION(contrail->tag_index);
	real rate = definition->point_rate;
	short count = 0;

	if (definition->flags02 & 1)
		rate = contrail->scale * rate;

	real interval = 1.0f / rate;

	if (dt != 0.0f)
	{
		while (dt >= contrail->point_delay)
		{
			dt -= contrail->point_delay;
			count++;
			contrail->point_delay = interval;
			if (dt == 0.0f)
				return count;
		}
		contrail->point_delay -= dt;
	}
	return count;
}

// @retail 0x17c880
void function_17c880(long contrail_index)
{
	s_contrail_datum *contrail = CONTRAIL(contrail_index);
	s_data_array *points = g_4ea940;

	for (long i = 0; i < 4; i++)
	{
		long point_index = contrail->point_indices[i];

		while (point_index != NONE)
		{
			long next_index = ((s_contrail_point_datum *)points->data + (point_index & 0xffff))->next_index;

			datum_delete(points, point_index);
			point_index = next_index;
		}
		contrail->point_counts[i] = 0;
		contrail->point_indices[i] = NONE;
	}
}
