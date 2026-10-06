#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
#include "object_queries.h"
#include <math.h>
#include <string.h>
#include <float.h>

// @flags /O2 /arch:SSE /Gr

/* limits read by the range checks below (zero in the retail image; set at run time) */
real g_54e854;
real g_54e858;
real g_54e85c;
real g_54e860;

extern real g_4e9bd0;
short g_468d3c[6] = { 3, 3, 2, 1, 1, 3 };
short g_468d30[6] = { 3, 3, 2, 1, 1, 6 };
real const g_4449c0[6] = { 1500.0f, 1500.0f, 1500.0f, 100000.0f, 100000.0f, 100000.0f };

struct s_motion_channels_1701f0
{
	byte unknown00[8];
	dword flags;
	real target[16];
	byte unknown4c[0x94 - 0x4c];
	byte channel_flags[6];
	byte unknown9a[2];
	real times[6];
	byte unknownb4[0x10c - 0xb4];
	real current[16];
	byte unknown14c[0x180 - 0x14c];
	real first_derivative[13];
	real second_derivative[13];
	real fifth[13];
	real fourth[13];
	real third[13];
	real second[13];
	real first[13];
	real constant[13];
	real delta[13];
	byte unknown354[4];
};

// @retail 0x1701f0
void function_1701f0(long player_index)
{
	s_motion_channels_1701f0 *state = (s_motion_channels_1701f0 *)g_4e9bd4 + player_index;
	real *out = state->second_derivative;
	real *fifth = state->fifth;
	real *fourth = state->fourth;
	real *third = state->third;
	real *second = state->second;
	real *time = state->times;
	for (long group = 0; group < 6; ++group, ++time)
	{
		real t = *time - g_4e9bd0;
		if (t > 0.0f)
		{
			real t2 = t * t;
			real t3 = t2 * t;
			for (short i = 0; i < g_468d3c[group]; ++i)
			{
				out[i] = fifth[i] * t3 * 20.0f + fourth[i] * t2 * 12.0f + third[i] * t * 6.0f + second[i] * 2.0f;
				if (out[i] > g_4449c0[group] || 0.0f - g_4449c0[group] > out[i])
				{
					for (long other = 0; other < 6; ++other)
						if (other != group && state->times[other] == *time)
							state->times[other] = 0.0f;
					*time = 0.0f;
				}
			}
		}
		else
			memset(out, 0, g_468d3c[group] * sizeof(real));
		long count = g_468d3c[group];
		out += count;
		fifth += count;
		fourth += count;
		third += count;
		second += count;
	}
}

// @retail 0x1703f0
void function_1703f0(long player_index)
{
	real inverse_dt = (real)(1.0 / g_4e9bd0);
	s_motion_channels_1701f0 *state = (s_motion_channels_1701f0 *)g_4e9bd4 + player_index;
	real *out = state->first_derivative;
	real *fifth = state->fifth;
	real *fourth = state->fourth;
	real *third = state->third;
	real *second = state->second;
	real *first = state->first;
	real *delta = state->delta;
	for (long group = 0; group < 6; ++group)
	{
		real t = state->times[group] - g_4e9bd0;
		if (t > 0.0f)
		{
			real t2 = t * t;
			real t3 = t2 * t;
			real t4 = t3 * t;
			for (short i = 0; i < g_468d3c[group]; ++i)
				out[i] = fifth[i] * t4 * 5.0f + fourth[i] * t3 * 4.0f + third[i] * t2 * 3.0f + second[i] * t * 2.0f + first[i];
		}
		else if ((state->flags & 1) && ((state->channel_flags[group] & 2) || (state->flags & 8)))
			memset(out, 0, g_468d3c[group] * sizeof(real));
		else if (state->flags & 1)
		{
			for (short i = 0; i < g_468d3c[group]; ++i)
				out[i] = 0.0f - delta[i] * inverse_dt;
		}
		long count = g_468d3c[group];
		out += count;
		fifth += count;
		fourth += count;
		third += count;
		second += count;
		first += count;
		delta += count;
	}
}

vector3f *matrix4x3_rotation_between(transform4x3f const *a, transform4x3f const *b, vector3f *out);

PRIVATE inline void placement_basis_170c00(vector3f const *forward, vector3f const *up, transform4x3f *matrix)
{
	matrix->scale = 1.0f;
	matrix->forward = *forward;
	matrix->left.i = up->j * forward->k - forward->j * up->k;
	matrix->left.j = forward->i * up->k - up->i * forward->k;
	matrix->left.k = forward->j * up->i - forward->i * up->j;
	matrix->up = *up;
	matrix->position.x = matrix->position.y = matrix->position.z = 0.0f;
}

// @retail 0x170c00
vector3f *function_170c00(vector3f const *first_forward, vector3f const *first_up, vector3f const *second_forward, vector3f const *second_up, vector3f *out)
{
	transform4x3f first, second;
	placement_basis_170c00(first_forward, first_up, &first);
	placement_basis_170c00(second_forward, second_up, &second);
	return matrix4x3_rotation_between(&first, &second, out);
}

// @retail 0x170bb0
void function_170bb0(real const *first, real const *second, real *out)
{
	long count = 10;
	do
	{
		*out++ = *first++ - *second++;
	} while (--count);
	count = 1;
	do
	{
		function_170c00((vector3f const *)first, (vector3f const *)(first + 3),
			(vector3f const *)second, (vector3f const *)(second + 3), (vector3f *)out);
		first += 6;
		second += 6;
		out += 3;
	} while (--count);
}

// @retail 0x170d70
void function_170d70(vector3f const *rotation, vector3f *a, vector3f *b)
{
	vector3f axis = *rotation;
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

PRIVATE inline bool motion_near_zero_170630(real value)
{
	return real_is_finite(value) && fabs(value) < 0.001f;
}

PRIVATE inline void motion_normalize_170630(vector3f *vector)
{
	real magnitude = (real)sqrt(vector->i * vector->i + vector->j * vector->j + vector->k * vector->k);
	if (!(fabs(magnitude) < 0.0001f))
	{
		real inverse = 1.0f / magnitude;
		vector->i *= inverse;
		vector->j *= inverse;
		vector->k *= inverse;
	}
}

// @retail 0x170630
void function_170630(long player_index)
{
	s_motion_channels_1701f0 *state = (s_motion_channels_1701f0 *)g_4e9bd4 + player_index;
	real values[13];
	real *value = values;
	real *current = state->current;
	real *target = state->target;
	real *velocity = state->first_derivative;
	real *fifth = state->fifth;
	real *fourth = state->fourth;
	real *third = state->third;
	real *second = state->second;
	real *first = state->first;
	real *constant = state->constant;
	for (short group = 0; group < 6; ++group)
	{
		real t = state->times[group] - g_4e9bd0;
		if (!(t > 0.0f) && (state->flags & 1))
		{
			for (short i = 0; i < g_468d30[group]; ++i)
				current[i] = target[i];
		}
		else
		{
			if (t > 0.0f)
			{
				real t2 = t * t;
				real t3 = t2 * t;
				real t4 = t3 * t;
				real t5 = t4 * t;
				for (short i = 0; i < g_468d3c[group]; ++i)
					value[i] = fifth[i] * t5 + fourth[i] * t4 + third[i] * t3 + second[i] * t2 + first[i] * t + constant[i];
			}
			else
			{
				for (short i = 0; i < g_468d3c[group]; ++i)
					value[i] = 0.0f - velocity[i] * g_4e9bd0;
			}
			if (group < 5)
			{
				for (short i = 0; i < g_468d3c[group]; ++i)
					current[i] = value[i] + current[i];
			}
			else
				function_170d70((vector3f const *)value, (vector3f *)current, (vector3f *)(current + 3));
		}
		current += g_468d30[group];
		target += g_468d30[group];
		long count = g_468d3c[group];
		value += count;
		velocity += count;
		fifth += count;
		fourth += count;
		third += count;
		second += count;
		first += count;
		constant += count;
	}
	vector3f *forward = (vector3f *)(state->current + 10);
	vector3f *up = (vector3f *)(state->current + 13);
	if (!(motion_near_zero_170630(length_sq3f(forward) - 1.0f) &&
		motion_near_zero_170630(length_sq3f(up) - 1.0f) &&
		motion_near_zero_170630(up->k * forward->k + forward->i * up->i + up->j * forward->j)))
	{
		vector3f left;
		left.i = up->j * forward->k - up->k * forward->j;
		left.j = up->k * forward->i - forward->k * up->i;
		left.k = forward->j * up->i - up->j * forward->i;
		up->i = forward->j * left.k - left.j * forward->k;
		up->j = left.i * forward->k - forward->i * left.k;
		up->k = forward->i * left.j - forward->j * left.i;
		motion_normalize_170630(forward);
		motion_normalize_170630(up);
	}
}

// @retail 0x172350
bool function_172350(point3f const *point)
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

bool function_a74c0(vector3f const *forward, vector3f const *up);
bool function_a7570(vector3f const *vector);

struct s_checked_placement_1724a0
{
	dword flags;
	point3f position;
	point3f target;
	byte unknown1c[8];
	real scale;
	real radius;
	vector3f forward;
	vector3f up;
	vector3f velocity;
	byte unknown50[0x38];
	real duration;
};

// @retail 0x1724a0
bool function_1724a0(s_checked_placement_1724a0 const *placement)
{
	if (placement && (!(placement->flags & 1) ||
		(function_a74c0(&placement->forward, &placement->up) &&
		function_172350(&placement->position) && function_172350(&placement->target) &&
		function_a7570(&placement->velocity) && function_172420(placement->scale) &&
		function_1723e0(placement->radius) && function_172460(placement->duration))))
		return true;
	return false;
}

struct s_observer_command;
struct s_16f460;
void function_16f460(s_16f460 *state);

PRIVATE inline real placement_clamp_172520(real value, real minimum, real maximum)
{
	return value < minimum ? minimum : value > maximum ? maximum : value;
}

// @retail 0x172520
void function_172520(s_observer_command *command)
{
	s_checked_placement_1724a0 *placement = (s_checked_placement_1724a0 *)command;
	if (!function_1724a0(placement))
	{
		if (!function_a74c0(&placement->forward, &placement->up))
		{
			placement->forward = *g_4687a8;
			placement->up = *g_4687b0;
		}
		placement->position.x = placement_clamp_172520(placement->position.x, -50000.0f, 50000.0f);
		placement->position.y = placement_clamp_172520(placement->position.y, -50000.0f, 50000.0f);
		placement->position.z = placement_clamp_172520(placement->position.z, -50000.0f, 50000.0f);
		placement->target.x = placement_clamp_172520(placement->target.x, -50000.0f, 50000.0f);
		placement->target.y = placement_clamp_172520(placement->target.y, -50000.0f, 50000.0f);
		placement->target.z = placement_clamp_172520(placement->target.z, -50000.0f, 50000.0f);
		if (!function_a7570(&placement->velocity))
			placement->velocity = *g_4687a4;
		placement->radius = placement_clamp_172520(placement->radius, g_54e858, g_54e85c);
		placement->scale = placement_clamp_172520(placement->scale, 0.0f, g_54e860);
		if (!function_1724a0(placement))
			function_16f460((s_16f460 *)command);
	}
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
bool function_172750(long mode, point3f const *point, real radius)
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
	point3f position;
	s_location location;
	byte unknown14[0x20 - 0x14];
	vector3f forward;
	vector3f up;
	real field_of_view;
	real aspect;
	real scale_x;
	real scale_y;
	real deviation;
	real vertical_field_of_view;
	real ratio;
};

static inline void rotation_matrix_from_axis_angle(s_rotation_matrix *m, vector3f const *axis, real angle)
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

static inline void rotation_matrix_transform(s_rotation_matrix const *m, vector3f const *in, vector3f *out)
{
	vector3f copy;

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
	vector3f original = view->forward;
	s_rotation_matrix m;

	view->ratio = view->field_of_view / g_54e854;
	view->vertical_field_of_view = (real)(atan2((double)(tan(view->field_of_view * 0.5f) / view->aspect), 1.0) * 2.0);

	real half = view->vertical_field_of_view * 0.5f;
	real angle_a = (real)atan(tan(half) * view->scale_x);
	real angle_b = (real)atan(tan(half) * view->scale_y);

	rotation_matrix_from_axis_angle(&m, g_4687b0, angle_a);
	rotation_matrix_transform(&m, &view->forward, &view->forward);
	rotation_matrix_transform(&m, &view->up, &view->up);

	vector3f axis;
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

void function_11bed0(s_location *location, point3f const *point);

// @retail 0x1726d0
void function_1726d0(s_view_setup *view, point3f const *position, vector3f const *forward, vector3f const *up)
{
	memset(view, 0, sizeof(*view));
	view->position = *position;
	view->forward = *forward;
	view->up = *up;
	view->field_of_view = g_54e854;
	view->aspect = 1.3333334f;
	function_11bed0(&view->location, position);
	function_171d90(view);
}

struct s_polygon_173910
{
	long field_0;
	long plane_index;
	byte unknown08[0x14];
	long count;
	point3f const *points;
};

struct s_polygon_context_173910
{
	long field_0;
	struct s_planes
	{
		byte unknown00[0xc];
		plane3f const *planes;
	} *geometry;
	struct s_origin
	{
		byte unknown00[0x60];
		point3f position;
	} *origin;
};

short function_120850(vector3f const *v);
bool function_23a220(point2f const *point, short count, point2f const *points, real epsilon);

// @retail 0x173910
bool function_173910(s_polygon_context_173910 const *context, s_polygon_173910 const *polygon)
{
	point2f points[128];
	plane3f const *plane = &context->geometry->planes[polygon->plane_index];
	long axis = function_120850(&plane->n);
	bool positive = ((real const *)plane)[axis] > 0.0f;
	point3f const *origin = &context->origin->position;
	real distance = 0.0f - plane_distance_to_point(plane, origin);
	point3f projected;
	projected.x = plane->n.i * distance + origin->x;
	projected.y = distance * plane->n.j + origin->y;
	projected.z = plane->n.k * distance + origin->z;
	short const *axes = g_440b94[axis * 2 + positive];
	point2f point;
	point.x = ((real const *)&projected)[axes[0]];
	point.y = ((real const *)&projected)[axes[1]];
	for (long i = 0; i < polygon->count; ++i)
	{
		points[i].x = ((real const *)&polygon->points[i])[axes[0]];
		points[i].y = ((real const *)&polygon->points[i])[axes[1]];
	}
	return function_23a220(&point, (short)polygon->count, points, 0.0001f) != false;
}

// @retail 0x173890
long function_173890(s_polygon_context_173910 const *context, s_polygon_173910 const *polygon, long state, bool *inside)
{
	plane3f const *plane = &context->geometry->planes[polygon->plane_index];
	point3f const *origin = &context->origin->position;
	real distance = origin->x * plane->i + plane->j * origin->y + plane->k * origin->z - plane->d;
	if (fabs(distance) > 0.015625)
	{
		*inside = false;
		if (state <= 0)
		{
		zero:
			return 0;
		}
		if (state >= 3)
		{
			if (distance < 0.0f)
				return 1;
			return 2;
		}
	}
	else
	{
		*inside = function_173910(context, polygon);
		if (!*inside && state <= 0)
			goto zero;
	}
	return 3;
}

struct s_projected_polygon_173520
{
	long field_0;
	byte state;
	byte unknown05;
	short field_6;
	short field_8;
	short unknown0a;
	real distance;
	box2f bounds;
	short count;
	short unknown22;
	point2f *points;
};

struct s_polygon_cache_173520
{
	struct s_geometry
	{
		byte unknown00[0x60];
		s_polygon_173910 *polygons;
	} *geometry;
	s_polygon_context_173910::s_planes *planes;
	byte *view;
	plane3f field_c_8;
	dword visited[16];
	s_projected_polygon_173520 polygons[512];
	long point_count;
	point2f points[5120];
};

box2f g_5476cc;
box2f *g_4687dc = &g_5476cc;
int __stdcall function_1429d0(transform4x3f const *matrix, long count, point3f const *source, point3f *destination);
long function_11fc80(plane3f const *plane, bool keep_inside, real tolerance, long point_count, point3f const *points, long maximum_count, point3f *out);

// @retail 0x173520
s_projected_polygon_173520 *function_173520(long polygon_index, s_polygon_cache_173520 *cache)
{
	point3f transformed[64];
	point3f clipped[64];
	s_projected_polygon_173520 *result = &cache->polygons[polygon_index];
	dword bit = 1 << (polygon_index & 31);
	long word_index = polygon_index >> 5;
	if (!(cache->visited[word_index] & bit))
	{
		if (5120 - cache->point_count < 64)
			return 0;
		s_polygon_173910 const *polygon = &cache->geometry->polygons[polygon_index];
		function_1429d0((transform4x3f const *)(cache->view + 4), polygon->count, polygon->points, transformed);
		long count = function_11fc80(&cache->field_c_8, true, 0.0078125f, polygon->count, transformed, 64, clipped);
		bool inside;
		result->state = (byte)function_173890((s_polygon_context_173910 const *)cache, polygon, count, &inside);
		result->field_6 = ((short const *)polygon)[0];
		result->field_8 = ((short const *)polygon)[1];
		result->distance = FLT_MAX;
		result->bounds = *g_4687dc;
		result->points = cache->points + cache->point_count;
		cache->point_count += 64;
		result->count = (short)count;
		result->field_0 = *(long const *)((byte const *)polygon + 0x18);
		if (result->state)
		{
			long index = result->state == 1 ? count - 1 : 0;
			long step = result->state == 1 ? -1 : 1;
			for (long i = 0; i < count; ++i, index += step)
			{
				point2f *point = &result->points[index];
				real inverse = -1.0f / clipped[i].z;
				point->x = clipped[i].x * inverse;
				point->y = clipped[i].y * inverse;
				if (result->bounds.x0 > point->x)
					result->bounds.x0 = point->x;
				if (point->x > result->bounds.x1)
					result->bounds.x1 = point->x;
				if (result->bounds.y0 > point->y)
					result->bounds.y0 = point->y;
				if (point->y > result->bounds.y1)
					result->bounds.y1 = point->y;
				real distance = 0.0f - clipped[i].z;
				result->distance = result->distance > distance ? distance : result->distance;
			}
			if (result->state == 3)
			{
				if (inside)
				{
					result->distance = (real)(result->distance > 0.015625 ? 0.015625 : result->distance);
					result->bounds = *(box2f *)(cache->view + 0xa0);
					result->count = *(short *)(cache->view + 0x198);
					memcpy(result->points, cache->view + 0x19c, *(long *)(cache->view + 0x198) * sizeof(point2f));
				}
				else
				{
					result->bounds.x0 -= 0.00390625f;
					result->bounds.x1 += 0.00390625f;
					result->bounds.y0 -= 0.00390625f;
					result->bounds.y1 += 0.00390625f;
					result->points[0].x = result->bounds.x0;
					result->points[0].y = result->bounds.y0;
					result->points[1].x = result->bounds.x1;
					result->points[1].y = result->bounds.y0;
					result->points[2].x = result->bounds.x1;
					result->points[2].y = result->bounds.y1;
					result->points[3].x = result->bounds.x0;
					result->points[3].y = result->bounds.y1;
					result->count = 4;
				}
			}
			if (cache->view[0x84] && result->distance > *(real *)(cache->view + 0x88))
				result->state = 0;
			cache->point_count = (result->points - cache->points) + result->count;
		}
		cache->visited[word_index] |= bit;
	}
	return result;
}
