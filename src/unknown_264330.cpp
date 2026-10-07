#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1fb7e0.h"
#include "unknown_25d020.h"
#include <string.h>

// @flags /O2 /arch:SSE /Gr

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);
bool function_1df560(short team, short other_team);
void function_25b910(long actor_index, long prop_ref_index);
point3f *function_b9dd0(long object_index, point3f *position);
real function_30bf0(vector3f *vector);
long function_264260(vector3f const *facing, vector3f const *direction, real distance);
short ai_player_index_get(long player_index);
void function_268c60(long actor_index);
long function_25d810(long object_index, long actor_index, bool create);
bool function_25ccd0(s_type_5cfb45 *state, long object_index, short value, s_2640c0 *motion);
void function_265c30(long prop_index, long actor_index, bool active);
void __stdcall function_265290(long actor_index, long prop_index);
void __stdcall function_265550(long actor_index, long prop_index);
void function_ba1d0(long object_index, vector3f *linear, vector3f *angular);
void function_cb7e0(long unit_index, vector3f *vector);
long function_10f8f0(long object_index);
struct s_28fb50;
long function_28fb50(long object_index, s_28fb50 const *context, long target_index);
__forceinline long real_to_long(real value);

// @retail 0x265050
void function_265050(long actor_index, long prop_ref_index)
{
	byte *actor = g_4f55f0->data + (actor_index & 0xffff) * 0x888;
	byte *reference = g_502418->data + (prop_ref_index & 0xffff) * 0x3c;
	long object_index = *(long *)(reference + 0x20);
	byte *object = *(byte **)(g_4e0300->data + (object_index & 0xffff) * 12 + 8);
	byte *definition = *(byte **)((byte *)g_4e3b44 + (*(long *)object & 0xffff) * 16 + 8);
	if (*(real *)(definition + 0xd0) + 10.0f > *(real *)(reference + 0x28))
	{
		short state = *(short *)(actor + 0x358);
		if (state < 2 || (state == 2 && *(long *)(actor + 0x360) != object_index &&
			*(real *)(actor + 0x394) > *(real *)(reference + 0x28)))
		{
			memset(actor + 0x358, 0, 0x58);
			*(short *)(actor + 0x358) = 2;
			*(long *)(actor + 0x360) = *(long *)(reference + 0x20);
			*(real *)(actor + 0x36c) = *(real *)(definition + 0xd0);
			*(short *)(actor + 0x35a) = 0;
			*(long *)(actor + 0x368) = prop_ref_index;
			long parent_index = *(long *)(object + 0xc8);
			long selected = NONE;
			if (parent_index != NONE)
			{
				byte *parent = (byte *)function_badc0(parent_index, NONE);
				if (parent && ((1 << parent[0xaa]) & 3))
				{
					selected = parent_index;
					long unit_index = *(long *)(actor + 0x18);
					if (unit_index != NONE && selected == unit_index)
						*(short *)(actor + 0x35a) = 2;
					else if (!function_1df560(*(short *)(actor + 0x24), *(short *)(parent + 0x138)))
						*(short *)(actor + 0x35a) = 1;
				}
			}
			*(long *)(actor + 0x364) = selected;
			function_25b910(actor_index, prop_ref_index);
		}
	}
}

// @retail 0x264940
void function_264940(long actor_index, long prop_ref_index)
{
	byte *actor = g_4f55f0->data + (actor_index & 0xffff) * 0x888;
	byte *reference = g_502418->data + (prop_ref_index & 0xffff) * 0x3c;
	byte *prop = g_50241c->data + (*(long *)(reference + 8) & 0xffff) * 0xc4;
	actor[0x30c] = false;
	if (!prop[0x23] && 3.0f > *(real *)(reference + 0x28) && *(char *)(reference + 0x27) >= 3)
	{
		long object_index = *(long *)(reference + 0x20);
		point3f position;
		function_b9dd0(object_index, &position);
		byte *headers = g_4e0300->data;
		byte *object = *(byte **)(headers + (object_index & 0xffff) * 12 + 8);
		vector3f facing = *(vector3f *)(object + 0x168);
		vector3f direction;
		direction.i = position.x - *(real *)(actor + 0x238);
		direction.j = position.y - *(real *)(actor + 0x23c);
		direction.k = position.z - *(real *)(actor + 0x240);
		if (function_30bf0(&direction) == 0.0f)
			direction = *g_4687a8;
		short category = (short)function_264260(&facing, &direction, *(real *)(reference + 0x28));
		if (!prop[0x31])
		{
			if (category <= 2)
			{
				function_1fb7e0(actor_index, 0xa3, NULL, object_index, NONE);
				prop[0x31] = true;
				*(short *)(actor + 0x30e) = 0;
			}
		}
		else
		{
			object = *(byte **)(headers + (object_index & 0xffff) * 12 + 8);
			long player_index = *(long *)(object + 0x13c);
			if (player_index != NONE)
			{
				if (category <= 0)
				{
					actor[0x30c] = true;
					*(short *)(actor + 0x310) = ai_player_index_get(player_index);
				}
				else
				{
					actor[0x30c] = false;
					*(short *)(actor + 0x310) = NONE;
					*(short *)(actor + 0x30e) = 0;
				}
			}
		}
	}
	long child_index = *(long *)(actor + 0x7c);
	if (child_index != NONE && *(long *)(reference + 0x14) != NONE)
	{
		byte *tracking = g_502414->data + (*(long *)(reference + 0x14) & 0xffff) * 0x124;
		if (tracking && tracking + 0x70 && *(short *)(tracking + 0x70) >= 4 &&
			25.0f > *(real *)(reference + 0x28))
			function_268c60(child_index);
	}
}

// @retail 0x264330
void __stdcall function_264330(long actor_index, long prop_ref_index, s_2641c0 *context,
	s_2640c0 *motion, bool force)
{
	byte *actor = g_4f55f0->data + (actor_index & 0xffff) * 0x888;
	if (!actor[9]) return;
	s_prop_datum *reference = prop_ref_get(prop_ref_index);
	byte *object = *(byte **)(g_4e0300->data + (reference->object_index & 0xffff) * 12 + 8);
	byte *prop = (byte *)prop_get(reference->prop_index);
	long time = g_510c54->game_time;
	byte *unit = ((1 << object[0xaa]) & 3) ? object : NULL;
	bool update = false;
	if ((reference->state >= 1 && reference->state <= 2) || force ||
		(reference->state >= 1 && reference->unknown26 > 0))
		update = true;
	if (reference->state >= 1 && reference->state <= 2)
		reference->type = *(short *)(prop + 2);
	else if (reference->type != *(short *)(prop + 2) && unit && *(short *)(prop + 2) == 6 &&
		(real)(time - *(long *)(unit + 0x2e0)) * g_510c54->rate > 5.0f)
		reference->type = 6;
	s_type_f95cd3 *view = NULL;
	s_type_5cfb45 *state = NULL;
	if (reference->tracking_index != NONE)
	{
		s_type_e5ff81 *tracking = tracking_get(reference->tracking_index);
		state = &tracking->state;
		view = &tracking->view;
	}
	else if (g_470f10[*(short *)(prop + 2)].kind == 1 && g_510c54->game_time > *(long *)(prop + 0x58))
		state = (s_type_5cfb45 *)(prop + 0x58);
	if (state && update)
	{
		if (view && prop[0x22] && *(long *)(prop + 0x1c) != NONE)
		{
			byte *other = g_4f55f0->data + (*(long *)(prop + 0x1c) & 0xffff) * 0x888;
			long target = function_28fb50(*(long *)(other + 0x1c), (s_28fb50 const *)context, reference->object_index);
			if (target != reference->object_index) reference->object_index = target;
		}
		function_25ccd0(state, reference->object_index, reference->unknown1c, motion);
	}
	state = function_25d690(reference);
	if (view)
	{
		vector3f *direction = (vector3f *)((byte *)view + 0x2c);
		direction->i = state->position.x - context->field_c.x;
		direction->j = state->position.y - context->field_c.y;
		direction->k = state->position.z - context->field_c.z;
		reference->unknown28 = function_30bf0(direction);
		if (reference->unknown28 == 0.0f) *direction = *g_4687a8;
		if (view->unknown50 != NONE && view->unknown50 + real_to_long((real)g_510c54->field_2_3 * 5.0f) < time)
			function_265c30(prop_ref_index, actor_index, false);
		if (update)
		{
			vector3f velocity;
			function_ba1d0(reference->object_index, &velocity, NULL);
			real speed = (real)sqrt(velocity.k * velocity.k + velocity.j * velocity.j + velocity.i * velocity.i);
			byte *fields = (byte *)view;
			fields[0x3a] = speed < 0.1f ? 0 : speed < 0.5f ? 1 : speed < 1.0f ? 2 : 3;
			real approach = 0.0f - (direction->k * velocity.k + direction->j * velocity.j + direction->i * velocity.i);
			fields[0x3b] = approach < -1.0f ? 0 : approach < -0.5f ? 1 : approach < -0.1f ? 2 :
				approach < 0.1f ? 3 : approach < 0.5f ? 4 : approach < 1.0f ? 5 : 6;
			if (*(char *)(fields + 0x3b) < 4)
			{
				view->unknown64 = false;
				view->unknown66 = 0;
			}
			else
			{
				++view->unknown66;
				if (view->unknown66 >= real_to_long((real)g_510c54->field_2_3 * 0.2f))
				{
					view->unknown66 = g_510c54->field_2_3;
					view->unknown64 = true;
				}
			}
			fields[0x38] = reference->unknown28 < 1.0f ? 0 : reference->unknown28 < 6.0f ? 1 :
				reference->unknown28 < 10.0f ? 2 : reference->unknown28 < 30.0f ? 3 : 4;
			vector3f facing;
			if (unit) function_cb7e0(reference->object_index, &facing);
			else facing = *(vector3f *)(object + 0x70);
			view->unknown39 = (char)function_264260(&facing, direction, reference->unknown28);
		}
	}
	else
	{
		real x = state->position.x - context->field_c.x;
		real y = state->position.y - context->field_c.y;
		real z = state->position.z - context->field_c.z;
		reference->unknown28 = (real)sqrt(z * z + y * y + x * x);
	}
	if (state->unknown66)
	{
		long related_index = function_25d810(state->unknown3c, actor_index, false);
		if (related_index != NONE)
		{
			s_prop_datum *related = prop_ref_get(related_index);
			point3f position;
			function_b9dd0(related->object_index, &position);
			real x = position.x - context->field_c.x;
			real y = position.y - context->field_c.y;
			real z = position.z - context->field_c.z;
			related->unknown28 = (real)sqrt(z * z + y * y + x * x);
			function_265290(actor_index, related_index);
		}
	}
	else if (unit && (state->unknown5f || function_10f8f0(reference->object_index) == 0x500000a))
		function_265550(actor_index, prop_ref_index);
	else if (reference->type == 3)
		function_265050(actor_index, prop_ref_index);
	else if (reference->type == 4)
		function_265290(actor_index, prop_ref_index);
	if (prop[0x25]) function_264940(actor_index, prop_ref_index);
	if (view) *(real *)((byte *)view + 0x3c) = function_265d30(actor_index, prop_ref_index);
}
