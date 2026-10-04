// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_153950.CPP: the game speed (time dilation and camera shake) slots */

#include "unknown_11c920.h"
#include "globals.h"
#include <math.h>
#include <string.h>

/* the result of the speed effects: the type, the amount and the shake */
struct s_speed_result
{
	long type;
	real amount;
	s_speed_shake shake;
};

byte g_510c60;
short g_468cec[7] = { 0, 1, 2, 3, 4, 5, 6 };

real function_17ca10(real x, short curve);
real function_30bf0(vector3f *v);
vector3f *random_unit_vector(vector3f *result, dword *seed);

// @retail 0x153950
void function_153950(void)
{
	s_game_speed *speed = g_510c5c;
	long index;

	if (speed->duration != NONE && g_510c54->game_time - speed->start_time > speed->duration && !speed->reverse)
	{
		speed->duration = NONE;
	}
	if (speed->flags & 1)
	{
		if (speed->timer24 > 0)
		{
			speed->timer24--;
		}
		else if (speed->flags & 2)
		{
			speed->flags &= ~1;
			g_502120->value228 = 0.0f;
			g_502120->value220 = 0.0f;
			g_502120->value224 = 0.0f;
		}
	}

	for (index = 0; index < 4; index++)
	{
		s_speed_slot *slot = speed->slots + index;

		if (index != NONE && g_4e8c20->entries[index] != NONE)
		{
			real quarter_second = (real)g_510c54->field_2_3 * 0.25f;
			long rounded;
			long decay;
			long i;

			__asm
			{
				fld quarter_second
				fistp rounded
			}
			decay = 255 / rounded;

			i = 0;
			do
			{
				if (slot->decay[i] > 0)
				{
					long value = slot->decay[i] - decay;
					slot->decay[i] = value < 0 ? 0 : value;
				}
				i++;
			}
			while (i < 4);
			if (slot->decay89 > 0)
			{
				slot->decay89 -= decay;
			}
			slot->flag0 = 0;
			if (slot->timer7e > 0)
			{
				slot->timer7e--;
			}
			slot->flag1 = 0;
			if (slot->timer80 > 0)
			{
				slot->timer80--;
			}
			slot->flag2 = 0;
			if (slot->timer82 > 0)
			{
				slot->timer82--;
			}
			if (slot->timer7c > 0)
			{
				slot->timer7c--;
				if (slot->timer7c == 0)
				{
					g_502120->entries[index].value80 = 0.0f;
					g_502120->entries[index].value84 = 0.0f;
					memset(slot->values6c, 0, sizeof(slot->values6c));
				}
			}
		}
		else
		{
			long i;

			memset(slot, 0, sizeof(s_speed_slot));
			memset(&g_502120->entries[index], 0, sizeof(s_speed_table_entry));
			for (i = 0; i < 8; i++)
			{
				g_502120->entries[index].items[i].a = NONE;
				g_502120->entries[index].items[i].b = NONE;
				g_502120->entries[index].items[i].scale = 1.0f;
			}
		}
	}
}

void function_1542a0(short seconds, real x, real y, real z);

// @retail 0x1538b0
void function_1538b0(void)
{
	s_game_speed *speed = g_510c5c;

	memset(speed, 0, sizeof(*speed));
	speed->duration = NONE;
	speed->time2c = g_510c54->game_time;
	g_510c60 = false;
	g_4e8c28 = *g_468718;
	if (g_4e6948->state == 1)
	{
		if (g_4e0350->flags & 0x80)
		{
			function_1542a0(0, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			function_1542a0(0, 0.0f, 0.0f, 0.0f);
		}
		g_510c60 = true;
	}
}

// @retail 0x153b80
void function_153b80(long index)
{
	if (index != NONE)
	{
		s_speed_slot *slot = (g_510c5c)->slots + index;

		memset(&slot->request, 0, sizeof(s_speed_request));
		slot->timer7e = 0;
		slot->flag0 = 0;
	}
}

void function_154d70(s_speed_request *request, s_speed_slot *slot, real scale);
void function_155240(s_speed_values *values, s_speed_slot *slot, real priority);

// @retail 0x153bd0
void function_153bd0(long index, real scale)
{
	if (index != NONE)
	{
		s_speed_slot *slot = &g_510c5c->slots[index];
		s_speed_request request;
		s_speed_values values;

		memset(&request, 0, sizeof(request));
		memset(&values, 0, sizeof(values));

		values.value[2] = scale * 0.01;
		values.value[0] = 1.0f;
		values.value[5] = 1.0f;
		request.shake = *(s_speed_shake const *)g_4686cc;
		request.duration = 1.0f;
		g_502120->entries[index].value80 = scale;
		g_502120->entries[index].value84 = scale;
		request.type = 1;
		request.priority = 2;
		request.amount = scale;
		function_154d70(&request, slot, scale);
		function_155240(&values, slot, scale);
	}
}

// @retail 0x153cd0
void function_153cd0(long player_index)
{
	long index = *(short *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c + 0x28);

	if (index != NONE)
	{
		s_speed_table_entry *entry = g_502120->entries + index;

		entry->value80 = 0.0f;
		entry->value84 = 0.0f;
	}
}

// @retail 0x154220
void function_154220(short seconds, real x, real y, real z)
{
	s_game_time_globals *time = g_510c54;
	s_game_speed *speed;
	real scaled;
	long ticks;

	g_4e8c28.x = x;
	g_4e8c28.y = y;
	g_4e8c28.z = z;
	scaled = (real)seconds * (1.0f / 30.0f);
	scaled = scaled * (real)time->field_2_3;
	__asm
	{
		fld scaled
		fistp ticks
	}
	speed = g_510c5c;
	speed->duration = (short)ticks;
	speed->reverse = false;
	speed->start_time = time->game_time;
	g_510c60 = false;
}

// @retail 0x1542a0
void function_1542a0(short seconds, real x, real y, real z)
{
	s_game_time_globals *time = g_510c54;
	s_game_speed *speed;
	real scaled;
	long ticks;

	g_4e8c28.x = x;
	g_4e8c28.y = y;
	g_4e8c28.z = z;
	scaled = (real)seconds * (1.0f / 30.0f);
	scaled = scaled * (real)time->field_2_3;
	__asm
	{
		fld scaled
		fistp ticks
	}
	speed = g_510c5c;
	speed->duration = (short)ticks;
	speed->reverse = true;
	speed->start_time = time->game_time;
}

// @retail 0x154310
void function_154310(long index, s_speed_result *result)
{
	s_game_speed *speed = g_510c5c;

	result->amount = 0.0f;
	if (speed->duration != NONE)
	{
		real t;

		result->type = 1;
		if (g_510c60)
		{
			if (g_4e0350->flags & 0x80)
			{
				*(point3f *)&result->shake.vector = *g_468710;
			}
			else
			{
				*(point3f *)&result->shake.vector = *g_468718;
			}
		}
		else
		{
			*(point3f *)&result->shake.vector = g_4e8c28;
		}
		result->shake.scale = 1.0f;
		if (speed->duration > 0)
		{
			t = (real)(g_510c54->game_time - speed->start_time) / (real)speed->duration;
			if (0.0f > t)
				t = 0.0f;
			else if (t > 1.0f)
				t = 1.0f;
			t = function_17ca10(t, 5);
		}
		else
		{
			t = 1.0f;
		}
		result->amount = t;
		if (!speed->reverse)
		{
			result->amount = 1.0f - t;
		}
		result->amount = (0.0f > result->amount) ? 0.0f : ((result->amount > 1.0f) ? 1.0f : result->amount);
	}
	else if (index != NONE)
	{
		s_speed_slot *slot = speed->slots + index;

		if (slot->timer7e > 0 || (slot->flags & 1))
		{
			result->type = g_468cec[slot->request.type];
			result->shake = slot->request.shake;
			if (slot->request.duration > 0.0f)
			{
				result->amount = function_17ca10((real)slot->timer7e / slot->request.duration, slot->request.curve) * slot->request.amount;
			}
			else
			{
				result->amount = slot->request.amount;
			}
			result->amount = (0.0f > result->amount) ? 0.0f : ((result->amount > 1.0f) ? 1.0f : result->amount);
		}
	}
}

// @retail 0x1544e0
void function_1544e0(transform4x3f *matrix, real distance, real angle)
{
	s_random_globals *random = g_4e7408;

	if (angle != 0.0f)
	{
		vector3f axis;

		random_unit_vector(&axis, &random->seed);

		real s = (real)sin(angle);
		real c = (real)cos(angle);
		real t = 1.0f - c;

		matrix->scale = 1.0f;
		matrix->forward.i = (1.0f - axis.i * axis.i) * c + axis.i * axis.i;
		matrix->forward.j = t * axis.i * axis.j + axis.k * s;
		matrix->forward.k = t * axis.k * axis.i - axis.j * s;
		matrix->left.i = t * axis.i * axis.j - axis.k * s;
		matrix->left.j = (1.0f - axis.j * axis.j) * c + axis.j * axis.j;
		matrix->left.k = t * axis.k * axis.j + axis.i * s;
		matrix->up.i = t * axis.k * axis.i + axis.j * s;
		matrix->up.j = t * axis.k * axis.j - axis.i * s;
		matrix->up.k = (1.0f - axis.k * axis.k) * c + axis.k * axis.k;
		matrix->position.z = 0.0f;
		matrix->position.y = 0.0f;
		matrix->position.x = 0.0f;
	}
	if (distance != 0.0f)
	{
		vector3f v;

		random_unit_vector(&v, &random->seed);
		matrix->position.x = v.i * distance;
		matrix->position.y = v.j * distance;
		matrix->position.z = v.k * distance;
	}
}

// @retail 0x154d70
void function_154d70(s_speed_request *request, s_speed_slot *slot, real scale)
{
	s_game_time_globals *time = g_510c54;

	if (slot->request.priority > request->priority && (real)slot->timer7e * time->rate > request->duration)
	{
		return;
	}
	if (g_468cec[request->type] == 0)
	{
		return;
	}
	slot->request = *request;
	slot->request.duration = (real)time->field_2_3 * slot->request.duration;
	slot->timer7e = (short)slot->request.duration;
	slot->request.amount = slot->request.amount * scale;
	slot->flags |= 1;
}

// @retail 0x154df0
void function_154df0(vector3f *direction, s_speed_bounds *bounds, long index, s_speed_slot *slot, real priority)
{
	s_game_time_globals *time = g_510c54;
	real timer = (real)slot->timer80;

	if (bounds->duration > timer || priority > slot->priority9c || (priority >= slot->priority9c && bounds->duration > time->rate * timer))
	{
		vector3f w;
		vector3f v;

		v = *direction;
		v.k = 0.0f;
		function_30bf0(&v);

		s_unknown_185ab0_entry *view = g_4ed284->entries + index;
		real yaw = view->yaw;
		real pitch = view->pitch;

		w.i = (real)(cos(yaw) * cos(pitch));
		w.j = (real)(sin(yaw) * cos(pitch));
		w.k = 0.0f;
		function_30bf0(&w);

		if (fabs(v.j * v.j + v.i * v.i + v.k * v.k - 1.0f) < 0.0001f && fabs(w.j * w.j + w.i * w.i + w.k * w.k - 1.0f) < 0.0001f)
		{
			real dot = v.j * w.j + w.i * v.i;
			real angle;
			real cross;

			if (-1.0f > dot)
				dot = -1.0f;
			else if (dot > 1.0f)
				dot = 1.0f;
			angle = (real)acos((-1.0f > dot) ? -1.0f : ((dot > 1.0f) ? 1.0f : dot));
			cross = v.j * w.i - w.j * v.i;
			if (0.0f > cross)
			{
				angle = 0.0f - angle;
			}

			slot->bounds = *bounds;
			slot->priority9c = priority;
			slot->bounds.duration = (real)time->field_2_3 * slot->bounds.duration;
			slot->timer80 = (short)slot->bounds.duration;
			slot->forward.k = 0.0f;
			slot->forward.i = (real)cos(angle);
			slot->forward.j = (real)sin(angle);

			s_random_globals *random = g_4e7408;
			real scale = function_259d0(&random->seed, __FILE__, __LINE__, slot->bounds.lower, slot->bounds.upper);
			real rotation = function_x82e52f(&random->seed, __FILE__, __LINE__) * 6.2831855f;
			vector3f *up = g_4687b0;
			vector3f *e = &slot->forward;
			vector3f *u = &slot->vector;

			real ci = up->k * e->j - up->j * e->k;
			real cj = up->i * e->k - up->k * e->i;
			real ck = e->i * up->j - e->j * up->i;
			u->i = ci;
			u->j = cj;
			u->k = ck;
			function_30bf0(u);

			real s = (real)sin(rotation);
			real c = (real)cos(rotation);
			real t = (u->k * e->k + u->j * e->j + e->i * u->i) * (1.0f - c);
			real x = u->i;
			real y = u->j;
			real z = u->k;

			u->i = e->i * t + x * c - (y * e->k - z * e->j) * s;
			u->j = e->j * t + y * c - (z * e->i - x * e->k) * s;
			u->k = e->k * t + z * c - (x * e->j - y * e->i) * s;
			u->i = u->i * scale;
			u->j = u->j * scale;
			u->k = u->k * scale;
			slot->flags |= 2;
		}
	}
}

// @retail 0x155240
void function_155240(s_speed_values *values, s_speed_slot *slot, real priority)
{
	s_game_time_globals *time = g_510c54;
	real rate = time->rate;
	real timer = (real)slot->timer82;
	real t = rate * timer;
	real scaled;
	long ticks;

	if (values->value[0] > t || priority > slot->priority98 || (priority >= slot->priority98 && values->value[0] > t))
	{
		slot->values50 = *values;
		slot->priority98 = priority;
		scaled = (real)time->field_2_3 * slot->values50.value[0];
		__asm
		{
			fld scaled
			fistp ticks
		}
		slot->timer82 = (short)ticks;
		slot->flags |= 4;
	}
}
