#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include <math.h>
#include <string.h>

// @flags /O2 /arch:SSE /Gr

/* limits read by the range checks below (zero in the retail image; set at run time) */
real g_54e854;
real g_54e858;
real g_54e85c;
real g_54e860;

// @retail 0x170d70
void function_170d70(real_vector3d const *rotation, real_vector3d *a, real_vector3d *b)
{
	real_vector3d axis = *rotation;
	real angle = normalize_inline(&axis);

	if (angle != 0.f)
	{
		real s = (real)sin(angle);
		real c = (real)cos(angle);
		real one_minus_c = 1.f - c;

		real ka = (a->i * axis.i + axis.k * a->k + axis.j * a->j) * one_minus_c;
		real pa1 = axis.i * a->k - a->i * axis.k;
		real pa2 = a->i * axis.j - axis.i * a->j;
		a->i = a->i * c + ka * axis.i - (axis.k * a->j - axis.j * a->k) * s;
		a->j = ka * axis.j + c * a->j - pa1 * s;
		a->k = ka * axis.k + c * a->k - pa2 * s;

		real kb = (b->k * axis.k + b->j * axis.j + b->i * axis.i) * one_minus_c;
		real pb1 = b->k * axis.i - b->i * axis.k;
		real pb2 = b->i * axis.j - b->j * axis.i;
		b->i = b->i * c + kb * axis.i - (b->j * axis.k - b->k * axis.j) * s;
		b->j = b->j * c + kb * axis.j - pb1 * s;
		b->k = b->k * c + kb * axis.k - pb2 * s;
	}
}

static inline bool real_is_finite(real value)
{
	return (*(long *)&value & 0x7f800000) != 0x7f800000;
}

static inline bool real_in_world_range(real value)
{
	return real_is_finite(value) && value >= -50000.0f && value <= 50000.0f;
}

// @retail 0x172350
bool function_172350(real_point3d const *point)
{
	if (real_in_world_range(point->x) && real_in_world_range(point->y) && real_in_world_range(point->z))
		return true;
	return false;
}

// @retail 0x1723e0
bool function_1723e0(real value)
{
	if (real_is_finite(value) && value >= g_54e858 && value <= g_54e85c)
		return true;
	return false;
}

// @retail 0x172420
bool function_172420(real value)
{
	if (real_is_finite(value) && value >= g_45dbd8 && value <= g_54e860)
		return true;
	return false;
}

// @retail 0x172460
bool function_172460(real value)
{
	if (real_is_finite(value) && value >= g_45dbd8 && value <= 3600.0f)
		return true;
	return false;
}

/* a table of 512 short lists: a bit per list that is in use, the first value
   of each list and the next value after each value */
struct s_short_list_table
{
	dword used[16];
	short first[512];
	short next[512];
};

// @retail 0x173b00
void function_173b00(s_short_list_table *table, short list_index, short value)
{
	if (!(table->used[list_index >> 5] & (1 << (list_index & 0x1f))))
	{
		table->next[value] = NONE;
		table->first[list_index] = value;
		table->used[list_index >> 5] |= 1 << (list_index & 0x1f);
	}
	else
	{
		table->next[value] = table->first[list_index];
		table->first[list_index] = value;
	}
}

struct s_ring_buffer
{
	long unknown00;
	long index;
	short values[256];
};

// @retail 0x173b60
void function_173b60(s_ring_buffer *buffer, short value)
{
	long index = buffer->index++;

	if (buffer->index == 256)
		buffer->index = 0;
	buffer->values[index] = value;
}

struct s_flag_holder
{
	byte unknown00[0x16];
	byte flag16_0 : 1;
	byte flag16_1 : 1;
	byte flag16_2 : 1;
};

// @retail 0x173b80
long function_173b80(s_flag_holder *holder)
{
	return holder->flag16_2;
}

/* ---- the local players ---- */

static inline long local_player_next(long index)
{
	long i = (index == NONE) ? 0 : index + 1;
	long result = NONE;

	for (; i < 4; i++)
	{
		if (g_4e8c20->entries[i] != NONE)
		{
			result = i;
			break;
		}
	}
	return result;
}

// @retail 0x172750
bool function_172750(long mode, real_point3d const *point, real radius)
{
	real limit = radius * radius;

	for (long i = local_player_next(NONE); i != NONE; i = local_player_next(i))
	{
		s_player_state *player = NULL;
		real distance_squared;

		if (i != NONE && g_4686c4 != NONE)
			player = &g_4e9bd4[i].state;
		real dx = player->position.x - point->x;
		real dy = player->position.y - point->y;
		real dz = player->position.z - point->z;
		distance_squared = dx * dx + dy * dy + dz * dz;
		if (mode == 1)
			distance_squared = player->radius * player->radius * distance_squared;
		if (limit > distance_squared)
			return true;
	}
	return false;
}

/* ---- the rectangle pool ---- */

struct s_rect4
{
	real x0, x1, y0, y1;
};

struct s_pool_entry
{
	long key;
	short next;
	short unknown06;
	short value08;
	short unknown0a;
	s_rect4 rect;
	short count;
	short unknown1e;
	real *data;
	real unknown24;
	long unknown28;
	byte unknown2c[0x40];
};

struct s_pool
{
	long count;
	s_pool_entry entries[256];
	long data_used;
	real data[1];
};

struct s_pool_source
{
	byte unknown00[0xa0];
	s_rect4 rect;
	byte unknown_b0[0x198 - 0xb0];
	long count;
	real data[1];
};

// @retail 0x172bb0
short function_172bb0(s_pool *pool, s_pool_source *source, short value, long key)
{
	long count = pool->count++;
	short index = (short)count;
	s_pool_entry *entry = &pool->entries[index];

	entry->key = key;
	entry->next = NONE;
	entry->unknown06 = NONE;
	entry->unknown0a = NONE;
	entry->unknown28 = NONE;
	entry->value08 = value;
	entry->rect = source->rect;

	real *data = &pool->data[pool->data_used * 2];
	pool->data_used += source->count;
	entry->data = data;
	memcpy(data, source->data, source->count * 8);
	entry->count = (short)source->count;
	entry->unknown24 = 0.0f;
	memset(entry->unknown2c, 0, 0x40);
	return index;
}

// @retail 0x173350
short function_173350(s_pool *pool, short index, short value)
{
	while (index != NONE)
	{
		s_pool_entry *entry = &pool->entries[index];

		if (entry->unknown06 == value)
			break;
		index = entry->next;
	}
	return index;
}

/* ---- merging the rectangles ---- */

/* lists of pool entries: bit i of the first 16 words says whether list i
   has a head (heads[i]); the entries are chained through next[] */
struct s_pool_lists
{
	dword valid[16];
	short heads[512];
	short next[512];
};

static inline short list_head(s_pool_lists const *lists, short list)
{
	if (lists->valid[list >> 5] & (1 << (list & 31)))
		return lists->heads[list];
	return NONE;
}

static inline short list_next(s_pool_lists const *lists, short list, short index)
{
	short current = lists->heads[list];

	while (current != NONE && current != index)
		current = lists->next[current];
	return lists->next[index];
}

static inline real real_minimum(real a, real b)
{
	return (a > b) ? b : a;
}

static inline real real_maximum(real a, real b)
{
	return (a > b) ? a : b;
}

static inline void rect4_union(s_rect4 *rect, s_rect4 const *other)
{
	if (rect->x0 > other->x0)
		rect->x0 = other->x0;
	if (!(rect->x1 > other->x1))
		rect->x1 = other->x1;
	if (rect->y0 > other->y0)
		rect->y0 = other->y0;
	if (!(rect->y1 > other->y1))
		rect->y1 = other->y1;
}

static inline real rect4_area(s_rect4 const *rect)
{
	return (rect->y1 - rect->y0) * (rect->x1 - rect->x0);
}

// @retail 0x172c70
void function_172c70(s_pool_lists *lists, s_pool *pool)
{
	for (long list = 0; list < g_4e0348->list_count; list++)
	{
		long index = list_head(lists, (short)list);

		while (index != NONE)
		{
			s_pool_entry *entry = &pool->entries[index];
			long next = list_next(lists, (short)list, (short)index);

			while (next != NONE)
			{
				s_pool_entry *other = &pool->entries[next];
				s_rect4 merged = other->rect;

				rect4_union(&merged, &entry->rect);
				if ((rect4_area(&other->rect) + rect4_area(&entry->rect)) * 1.5f > rect4_area(&merged))
				{
					entry->unknown28 = next;
					other->unknown24 = real_minimum(entry->unknown24, other->unknown24);
					other->rect = merged;
					break;
				}
				next = list_next(lists, (short)list, (short)next);
			}
			index = list_next(lists, (short)list, (short)index);
		}
	}
}

/* ---- the view setup ---- */

struct s_rotation_matrix
{
	real m00, m01, m02;
	real m10, m11, m12;
	real m20, m21, m22;
};

struct s_view_setup
{
	byte unknown00[0x20];
	real_vector3d forward;
	real_vector3d up;
	real field_of_view;
	real aspect;
	real scale_x;
	real scale_y;
	real deviation;
	real vertical_field_of_view;
	real ratio;
};

static inline void rotation_matrix_from_axis_angle(s_rotation_matrix *m, real_vector3d const *axis, real angle)
{
	real s = (real)sin(angle);
	real c = (real)cos(angle);
	real i2 = axis->i * axis->i;
	real j2 = axis->j * axis->j;
	real k2 = axis->k * axis->k;
	real one_minus_c = 1.f - c;
	real tij = one_minus_c * axis->i * axis->j;
	real tik = one_minus_c * axis->k * axis->i;
	real tjk = one_minus_c * axis->k * axis->j;

	m->m00 = (1.f - i2) * c + i2;
	m->m01 = tij - s * axis->k;
	m->m10 = tij + s * axis->k;
	m->m11 = (1.f - j2) * c + j2;
	m->m20 = tik - s * axis->j;
	m->m02 = tik + s * axis->j;
	m->m22 = (1.f - k2) * c + k2;
	m->m12 = tjk - s * axis->i;
	m->m21 = tjk + s * axis->i;
}

static inline void rotation_matrix_transform(s_rotation_matrix const *m, real_vector3d const *in, real_vector3d *out)
{
	real_vector3d copy;

	if (in == out)
	{
		copy = *in;
		in = &copy;
	}
	out->i = in->i * m->m00 + m->m02 * in->k + m->m01 * in->j;
	out->j = in->k * m->m12 + m->m11 * in->j + in->i * m->m10;
	out->k = in->j * m->m21 + in->k * m->m22 + in->i * m->m20;
}

// @retail 0x171d90
void function_171d90(s_view_setup *view)
{
	real_vector3d original = view->forward;
	s_rotation_matrix m;

	view->ratio = view->field_of_view / g_54e854;
	view->vertical_field_of_view = (real)(atan2((double)(tan(view->field_of_view * 0.5f) / view->aspect), 1.0) * 2.0);

	real half = view->vertical_field_of_view * 0.5f;
	real angle_a = (real)atan(tan(half) * view->scale_x);
	real angle_b = (real)atan(tan(half) * view->scale_y);

	rotation_matrix_from_axis_angle(&m, g_4687b0, angle_a);
	rotation_matrix_transform(&m, &view->forward, &view->forward);
	rotation_matrix_transform(&m, &view->up, &view->up);

	real_vector3d axis;
	axis.i = view->forward.j * view->up.k - view->up.j * view->forward.k;
	axis.j = view->forward.k * view->up.i - view->up.k * view->forward.i;
	axis.k = view->up.j * view->forward.i - view->forward.j * view->up.i;
	rotation_matrix_from_axis_angle(&m, &axis, angle_b);
	rotation_matrix_transform(&m, &view->forward, &view->forward);
	rotation_matrix_transform(&m, &view->up, &view->up);

	real dot = view->forward.j * original.j + view->forward.k * original.k + view->forward.i * original.i;
	real limit = 0.0001f;
	if (!(dot > limit))
		dot = limit;
	real inv = 1.f / dot;
	real dz = original.k * inv - view->forward.k;
	real dy = original.j * inv - view->forward.j;
	real dx = inv * original.i - view->forward.i;
	view->deviation = (real)sqrt(dz * dz + dy * dy + dx * dx);
}
