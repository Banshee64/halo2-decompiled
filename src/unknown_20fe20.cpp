// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_20FE20.CPP: object tables of game speed (a16 pad) */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "data_array.h"
#include <math.h>

/* ---- shared views ---- */
struct s_header_view
{
	byte unknown00[8];
	byte *object;
};

#define OBJECT_FROM_INDEX(index) (((s_header_view *)g_4e0300->data)[(index) & 0xffff].object)

/* defined in src/unknown_210db0.cpp */
struct s_unknown_210db0
{
	byte unknown00[0x40];
};

extern s_unknown_210db0 *g_4f93a4;

/* the object iterator of src/unknown_0bad50.cpp */
struct s_object;
struct s_object_iterator
{
	dword type_mask;
	byte flags;
	byte unknown05;
	short index;
	long object_index;
	long signature;
};

s_object *function_baeb0(s_object_iterator *iterator);
void function_bae80(s_object_iterator *iterator, dword type_mask, byte flags);

/* defined in src/unknown_1efac0.cpp */
struct s_tag_ref_data;
struct s_lookup
{
	long handle;
	s_tag_ref_data *tag_a;
	s_tag_ref_data *tag_b;
	void *pointer_a;
	void *pointer_b;

	bool initialize(long handle);
};

/* ---- 0x4f9398: a pool of 0x54 byte nodes linked from an object ---- */
struct s_node
{
	byte unknown00[4];
	long object_index;
	byte unknown08[0x40];
	byte flag;
	byte unknown49[7];
	long next;
};

struct s_node_owner
{
	byte unknown00[0x7d0];
	long first;
	short count;
};

struct s_node_link
{
	byte unknown00[0x1c];
	long node_index;
};

struct s_node_object
{
	byte unknown00[0x342];
	short link_offset;
};

s_data_array *g_4f9398;

#define NODE(index) ((s_node *)(g_4f9398->data + ((index) & 0xffff) * sizeof(s_node)))

// @retail 0x20fe20
void function_20fe20(s_node_owner *owner)
{
	long *link = &owner->first;

	while (*link != NONE)
	{
		long node_index = *link;
		s_node *node = NODE(node_index);

		if (node->flag)
		{
			long next = node->next;

			if (NODE(node_index)->object_index != NONE)
			{
				s_node_object *object = (s_node_object *)OBJECT_FROM_INDEX(NODE(node_index)->object_index);
				s_node_link *object_link = (s_node_link *)((byte *)object + object->link_offset);

				if (object_link->node_index == node_index)
				{
					object_link->node_index = NONE;
				}
			}

			datum_delete(g_4f9398, node_index);
			*link = next;
			owner->count--;
		}
		else
		{
			link = &node->next;
		}
	}
}

/* ---- the tables of the match globals (0x4e0348) ---- */
struct s_flags8
{
	byte flags;
	byte unknown01[7];
};

struct s_range
{
	dword unknown00;
	long lower;
	long upper;
	short value;
	short unknown0e;
};

struct s_pair_item
{
	short value;
	byte byte2;
	byte byte3;
};

struct s_pair_entry
{
	dword flag0 : 1;
	dword unknown : 31;
	long lower;
	long upper;
	long range_count;
	s_range *ranges;
	long item_count;
	s_pair_item *items;
};

struct s_pair_table
{
	long count;
	s_flags8 *flags;
	byte unknown08[0x28];
	long entry_count;
	s_pair_entry *entries;
};

struct s_unknown_4e0348
{
	byte unknown00[0xc4];
	long count;
	s_pair_table *table;
};

struct s_object_with_pair
{
	byte unknown00[0xaa];
	byte type;
	byte unknownab[0x130 - 0xab];
	short value130;
	byte unknown132[0x1d4 - 0x132];
	short value1d4;
};

struct s_output_entry
{
	long object_index;
	short unknown04;
	short index;
	byte unknown08[2];
	byte byte0a;
	byte byte0b;
	byte unknown0c[4];
};

s_output_entry *g_4f93a0;

// @retail 0x2101d0
bool function_2101d0(s_pair_table *table, long object_index)
{
	s_object_with_pair *object = (s_object_with_pair *)OBJECT_FROM_INDEX(object_index);
	short value = NONE;

	if (object->type == 6)
	{
		value = object->value130;
	}
	else if (object->type == 7)
	{
		value = object->value1d4;
	}

	if (value >= 0)
	{
		if (value < table->entry_count)
		{
			s_pair_entry *entry = &table->entries[value];

			if (TEST_FIELD_BIT(entry->flag0))
			{
				short i = 0;

				if (entry->item_count > 0)
				{
					do
					{
						s_pair_item *item = &entry->items[i];
						s_output_entry *output = &g_4f93a0[item->value];

						output->index = i;
						output->object_index = object_index;
						output->byte0a = item->byte2;
						output->byte0b = item->byte3;
						i++;
					}
					while (i < entry->item_count);

					return false;
				}
			}
		}

		return false;
	}

	return false;
}

// @retail 0x210310
short function_210310(long a, long object_index)
{
	short result = NONE;
	s_unknown_4e0348 *globals = (s_unknown_4e0348 *)g_4e0348;

	if (globals->count > 0 && globals->table)
	{
		s_pair_table *table = globals->table;
		s_object_with_pair *object = (s_object_with_pair *)OBJECT_FROM_INDEX(object_index);
		short value = NONE;

		if (object->type == 6)
		{
			value = object->value130;
		}
		else if (object->type == 7)
		{
			value = object->value1d4;
		}

		if (value >= 0 && value < table->entry_count)
		{
			s_pair_entry *entry = &table->entries[value];

			if (TEST_FIELD_BIT(entry->flag0) && a != NONE)
			{
				s_lookup lookup;

				if (lookup.initialize(object_index))
				{
					short i = (short)(a & 0xff);

					if (i >= 0 && i < entry->item_count)
					{
						result = entry->items[i].value;
					}
				}
			}
		}
	}

	return result;
}
// @retail 0x2108a0
short function_2108a0(long index)
{
	short result = NONE;
	s_unknown_4e0348 *globals = (s_unknown_4e0348 *)g_4e0348;

	if (globals->count > 0 && globals->table)
	{
		s_pair_table *table = globals->table;

		if (index >= 0 && index < table->count)
		{
			if (table->flags[index].flags & 4)
			{
				short i;

				for (i = 0; i < table->entry_count; i++)
				{
					s_pair_entry *entry = &table->entries[i];

					if (TEST_FIELD_BIT(entry->flag0) && index >= entry->lower && index <= entry->upper)
					{
						short j;

						for (j = 0; j < entry->range_count; j++)
						{
							s_range *range = &entry->ranges[j];

							if (index >= range->lower && index <= range->upper)
							{
								short value = range->value;

								if (value >= 0 && value < entry->item_count)
								{
									result = entry->items[value].value;
								}
								break;
							}
						}
						break;
					}
				}
			}
		}
	}

	return result;
}
/* ---- the axis permutation tables ---- */
static const short g_440b94[6][3] =
{
	{ 2, 1, 0 }, { 1, 2, 0 }, { 0, 2, 1 }, { 2, 0, 1 }, { 1, 0, 2 }, { 0, 1, 2 }
};

static const short g_440bb8[6][3] =
{
	{ 2, 1, 0 }, { 2, 0, 1 }, { 0, 2, 1 }, { 1, 2, 0 }, { 1, 0, 2 }, { 0, 1, 2 }
};

// @retail 0x210d10
void function_210d10(short a, byte b, real *in, real_point3d *out)
{
	long index = a * 2 + b;
	real z = in[g_440b94[index][2]];
	real y = in[g_440b94[index][1]];

	out->x = in[g_440b94[index][0]];
	out->y = y;
	out->z = z;
}

// @retail 0x210d60
void function_210d60(short a, byte b, real *in, real_point3d *out)
{
	long index = a * 2 + b;
	real z = in[g_440bb8[index][2]];
	real y = in[g_440bb8[index][1]];

	out->x = in[g_440bb8[index][0]];
	out->y = y;
	out->z = z;
}

/* ---- the node bits ---- */
struct s_match_node
{
	byte unknown00[0x18];
	byte flags;
	byte unknown19[0x24 - 0x19];
};

struct s_match_nodes
{
	byte unknown00[0x5c];
	long count;
	s_match_node *nodes;
};

// @retail 0x210e20
void function_210e20(void)
{
	s_match_nodes *globals = (s_match_nodes *)g_4e0348;
	long i;

	for (i = 0; i < globals->count; i++)
	{
		if (globals->nodes[i].flags & 8)
		{
			((dword *)g_4f93a4)[i >> 5] &= ~(1 << (i & 0x1f));
		}
	}
}

struct s_object_with_values
{
	byte unknown00[0x1da];
	short value1da;
	short value1dc;
};

// @retail 0x210e80
void function_210e80(void)
{
	struct
	{
		s_object *object;
		s_object_iterator iterator;
	} state;

	function_bae80(&state.iterator, 0x80, 0);

	while ((state.object = function_baeb0(&state.iterator)) != 0)
	{
		s_object_with_values *object = (s_object_with_values *)OBJECT_FROM_INDEX(state.iterator.object_index);

		object->value1da = NONE;
		object->value1dc = 0;
	}
}
/* ---- the particle direction cells and the noise table (0x4f93bc) ---- */

/* defined in src/unknown_146240.cpp */
struct s_random_globals
{
	dword unknown0;
	dword seed;
};

extern s_random_globals *g_4e7408;
extern real_vector3d g_4417f0[1026];

PRIVATE short random_range(dword *seed, short range)
{
	*seed = *seed * 0x19660d + 0x3c6ef35f;
	return (short)(((*seed >> 16) * range) >> 16);
}

struct s_cell
{
	byte active;
	byte unknown01[3];
	real value4;
	real value8;
	real valuec;
	real interpolation;
	real_vector3d direction;
};

struct s_cell_source
{
	byte unknown00[0x50];
	long tag_index;
	real_vector3d direction;
	real scale;
	byte unknown64[0x88 - 0x64];
};

struct s_cell_tag
{
	real value0;
	real value4;
	real value8;
	real valuec;
};

struct s_cell_globals
{
	byte unknown00[0x84];
	long count;
	s_cell_source *sources;
};

long g_4fa0c0;
short g_4f9cbc;
s_cell g_4f9cc0[8];
real_vector3d g_4f93bc[24][8];

PRIVATE real random_step(void)
{
	return random_range(&g_4e7408->seed, 2) ? 0.01f : -0.01f;
}

PRIVATE real pin_real(real x, real lower, real upper)
{
	if (x < lower)
	{
		x = lower;
	}
	else if (x > upper)
	{
		x = upper;
	}

	return x;
}

// @retail 0x2118c0
void function_2118c0(void)
{
	s_cell_globals *globals = (s_cell_globals *)g_4e0348;
	short i;

	g_4fa0c0++;

	for (i = 0; i < globals->count; i++)
	{
		s_cell_source *source = &globals->sources[i];
		s_cell *cell = &g_4f9cc0[i];

		if (source->tag_index != NONE)
		{
			s_cell_tag *tag = (s_cell_tag *)g_4e3b44[source->tag_index & 0xffff].bytes;
			real azimuth;
			real elevation;
			real scale;
			long j;

			cell->value4 += random_step();
			cell->value4 = pin_real(cell->value4, 0.0f, 1.0f);

			cell->valuec += random_step();
			cell->valuec = pin_real(cell->valuec, -1.0f, 1.0f);

			cell->value8 += random_step();
			cell->value8 = pin_real(cell->value8, -1.0f, 1.0f);

			cell->interpolation = (tag->value4 - tag->value0) * cell->value4 + tag->value0;

			azimuth = (real)atan2(source->direction.j, source->direction.i);
			elevation = (real)atan2(source->direction.k, sqrt(source->direction.j * source->direction.j + source->direction.i * source->direction.i));
			elevation += tag->valuec * cell->valuec * 0.5f;
			azimuth += cell->value8 * tag->value8 * 0.5f;

			cell->direction.i = (real)cos(azimuth) * (real)cos(elevation);
			cell->direction.j = (real)sin(azimuth) * (real)cos(elevation);
			cell->direction.k = (real)sin(elevation);

			scale = source->scale * cell->interpolation;
			for (j = 0; j < 3; j++)
			{
				cell->direction.n[j] *= scale;
			}

			cell->active = true;
		}
		else
		{
			cell->active = false;
		}
	}

	g_4f9cbc = (short)globals->count;
}
// @retail 0x211cc0
void function_211cc0(void)
{
	dword *seed = &g_4e7408->unknown0;
	short k = 0;
	short m;
	short b;

	do
	{
		b = 0;

		do
		{
			*seed = *seed * 0x19660d + 0x3c6ef35f;
			g_4f93bc[b * 8 + k][0] = g_4417f0[(short)(((*seed >> 16) * 0x402) >> 16)];
			b++;
		}
		while (b < 3);

		k++;
	}
	while (k < 8);

	k = 0;

	do
	{
		m = 1;

		do
		{
			real previous = (real)(k - 1);
			real t = (real)m * 0.125f + (real)k;
			real ta = t - (previous + 2.0f);
			real tb = t - (previous + 1.0f);
			real tc = t - previous;

			b = 0;

			do
			{
				real_vector3d *p3 = &g_4f93bc[b * 8 + ((k + 2) & 7)][0];
				real_vector3d *p2 = &g_4f93bc[b * 8 + ((k + 1) & 7)][0];
				real_vector3d *p1 = &g_4f93bc[b * 8 + k][0];
				real_vector3d *p0 = &g_4f93bc[b * 8 + ((k - 1) & 7)][0];
				long i;

				for (i = 0; i < 3; i++)
				{
					real s0 = p3->n[i] - p2->n[i];
					real s1 = p2->n[i] - p1->n[i];
					real s2 = p1->n[i] - p0->n[i];
					real d2 = s1 - s2;
					real d3 = (s0 - s1) - d2;

					g_4f93bc[b * 8 + k][m].n[i] = ((d3 * ta * (1.0f / 3.0f) + d2) * tb * 0.5f + s2) * tc + p0->n[i];
				}

				b++;
			}
			while (b < 3);

			m++;
		}
		while (m < 8);

		k++;
	}
	while (k < 8);
}

// @retail 0x211fd0
void function_211fd0(real_vector3d *out, real const *position, real time, real scale)
{
	real frequencies[3] = { 0.1f, 0.2f, 0.07f };
	short i = 0;

	*out = *g_4687a4;
	scale *= 1.0f / 3.0f;

	do
	{
		real value;
		short index;
		real_vector3d *noise;

		value = ((real)g_4fa0c0 * frequencies[i] * time + position[i]) * 8.0f;
		*(dword *)&value &= 0x7fffffff;
		value += 8388608.0f;
		index = *(byte *)&value;
		index &= 0x3f;
		noise = &g_4f93bc[i * 8][index];

		out->i += noise->i;
		out->j += noise->j;
		out->k += noise->k;
		i++;
	}
	while (i < 3);

	out->i *= scale;
	out->j *= scale;
	out->k *= scale;
}
// @retail 0x212100
void function_212100(real_point3d const *p3, real_point3d const *p2, real_point3d const *p1, real_point3d const *p0, real_point3d *out, real a, real b, real c)
{
	real two_b = b * 2.0f;
	real three_b = b * 3.0f;
	real inverse_b = 1.0f / b;
	real t0 = c - a;
	real t1 = c - (a + b);
	real t2 = c - (two_b + a);
	long i;

	for (i = 0; i < 3; i++)
	{
		real s0 = p3->n[i] - p2->n[i];
		real s1 = p2->n[i] - p1->n[i];
		real s2 = p1->n[i] - p0->n[i];
		real d2 = s1 - s2;
		real d3 = (s0 - s1) - d2;

		out->n[i] = ((d3 * t2 / three_b + d2) * t1 / two_b + s2) * inverse_b * t0 + p0->n[i];
	}
}