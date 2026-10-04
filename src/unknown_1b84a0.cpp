// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "slot_handler.h"

/* slot type 0x23 */

struct s_slot_23
{
	s_slot_header header;
	bool unknown0c;
	bool unknown0d;
	short unknown0e;
	byte unknown10[0x1b - 0x10];
	bool unknown1b;
	bool unknown1c;
	char unknown1d;
	byte unknown1e[2];
	s_type_c3b527 point;
	vector3f facing;
	byte unknown3c[0x40 - 0x3c];
};

short __stdcall function_1b84a0(long actor_index);
void __stdcall function_1b85a0(long actor_index, s_slot *slot);
void __stdcall function_1b89d0(long actor_index, s_slot *slot);
void __stdcall function_1b8ae0(long actor_index, s_slot *slot);
bool function_25da00(s_prop_node_view *node);

// @retail 0x1b84a0
short __stdcall function_1b84a0(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (actor->prop_index != NONE && !actor->unknown223)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);
		s_prop_view_fields *view = prop_node_view(node);

		if (view && node->unknown24 >= 3 && (view->unknown70 == 1 || !function_25da00(node)))
			result = 3;
	}
	return result;
}

// @retail 0x1b8540
bool __stdcall function_1b8540(long actor_index, s_slot *slot)
{
	bool result = false;
	s_actor_view *actor = actor_get(actor_index);

	if (actor->prop_index != NONE && prop_node_get(actor->prop_index)->unknown27 < 2)
	{
		s_slot_23 *state = (s_slot_23 *)slot;

		state->unknown0c = false;
		state->unknown0d = false;
		state->unknown0e = 0;
		result = true;
	}
	return result;
}

real function_1f8940(long actor_index);
void *function_1e4db0(long actor_index);

/* the block of the actor's character tag function_1e4db0 returns */
struct s_character_db0_flags
{
	byte flags;
};

bool function_1f86f0(long index);
bool function_1f4f40(long actor_index, vector3f const *facing, short unknown, s_type_c3b527 const *point, bool face_prop);
void function_262800(long actor_index, s_reference reference, bool unknown);

/* slot type 0x23, update: once the actor stands at its prop's position, it
   turns to face it; then it lets the prop go */
// @retail 0x1b89d0
void __stdcall function_1b89d0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_23 *state = (s_slot_23 *)slot;

	if (actor->prop_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);
		s_prop_view_fields *view = prop_node_view(node);

		if (view)
		{
			if (state->unknown0c)
				return;

			state->unknown0c = state->unknown0d && function_1f86f0(actor_index);
			if (state->unknown0c)
			{
				if (!state->unknown1b || !state->unknown1d || state->unknown1c)
					return;

				function_1f4f40(actor_index, &state->facing, state->unknown1d, &state->point, true);
				state->unknown1c = true;
				state->unknown0c = false;
			}

			if (view->unknown70 != 0 || !function_25da00(node))
				return;

			function_262800(actor_index, actor->unknown418, false);
		}
	}

	state->unknown0c = true;
}

// @retail 0x1b8ae0
void __stdcall function_1b8ae0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->prop_index != NONE)
	{
		s_prop_view_fields *view = prop_node_view(prop_node_get(actor->prop_index));
		s_slot_23 *state = (s_slot_23 *)slot;

		actor->unknown4a2 = true;
		if (view->unknown06 != 2)
			state->unknown0e = g_510c54->field_2_3;
		else if (state->unknown0e > 0)
			state->unknown0e--;

		if (actor_get(actor_index)->unknown50c && actor_get(actor_index)->unknown504 == 1 &&
			function_1f8940(actor_index) >= 2.0f && state->unknown0e <= 0)
		{
			actor->unknown41c = 3;
			actor->unknown420 = 0;
		}
		else if (view->unknown70 == 0)
		{
			actor->unknown420 = 2;
			actor->unknown41c = 4;
		}
		else if (view->unknown70 == 1)
		{
			point3f point;

			function_210850(&view->unknown78, &point);
			actor->unknown424.point = point;
			actor->unknown420 = 3;
			actor->unknown41c = 4;
		}

		if (actor->unknown229)
			actor->unknown482 = true;

		s_character_db0_flags *character = (s_character_db0_flags *)function_1e4db0(actor_index);

		if (character && (character->flags & 2) && actor->unknown086 <= 3)
			actor->unknown483 = true;
	}
}

// @retail 0x1b8c60
short __stdcall function_1b8c60(long actor_index, s_slot *slot, bool active)
{
	s_slot_23 *state = (s_slot_23 *)slot;
	short result = g_46fbe8;

	if (state->unknown0c)
		result = g_46fbe4;
	return result;
}

s_slot_handler_2 g_47e908 =
{
	{
		0x23, 2, 0, -2, 0,
		function_1b84a0, function_1b8c60, function_1b8540, slot_proc_nothing, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1b85a0, function_1b89d0, function_1b8ae0
};
