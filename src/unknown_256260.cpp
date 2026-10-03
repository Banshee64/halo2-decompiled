// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_256260.CPP: slot handlers 0x76, 0x73, 0x74, 0x75 and their parent
   0x72 (handlers at 0x47f858..0x47f958, children at 0x470b70) */

#include "cseries.h"
#include "slot_handler.h"
#include "props.h"
#include "unknown_2551c0.h"

/* the slot state of handler 0x76 */
struct s_slot_76_state
{
	s_slot_header header;
	bool started;
	byte unknown0d;
	short timer;
	byte unknown10[0x40 - 0x10];
};

/* what function_1e4b50 returns for the actor's character */
struct s_character_76_view
{
	short unknown0;
	byte unknown2[2];
	real range;
	real duration;
};

s_data_array *g_5044c8;

short g_470be8 = -1;
short g_470bec = -2;

void *function_1e4b50(long actor_index);
short function_1a6fe0(long owner_index, short type);
prop_state *function_25d670(long prop_ref_index);
short __stdcall function_1ad550(long actor_index);

short __stdcall function_256260(long actor_index);
short __stdcall function_256300(long actor_index, s_slot *slot, bool active);
void __stdcall function_2562c0(long actor_index, s_slot *slot);
void __stdcall function_256460(long actor_index, s_slot *slot);
short __stdcall function_2564a0(long actor_index, s_slot *slot, bool active);
void __stdcall function_2564e0(long actor_index, s_slot *slot);
short __stdcall function_256620(long actor_index);
short __stdcall function_256650(long actor_index, s_slot *slot, bool active);
void __stdcall function_256680(long actor_index, s_slot *slot);
short __stdcall function_256810(long actor_index, s_slot *slot);
short __stdcall function_256af0(long actor_index, short level, bool active);

s_slot_handler_2 g_47f858 =
{
	{
		0x76, 2, 0, -2, 0,
		function_256260, function_256300, 0, function_2562c0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	0, 0, function_256460
};

s_slot_handler_2 g_47f8a8 =
{
	{
		0x73, 2, 0, -2, 0,
		function_1ad550, function_2564a0, 0, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	0, 0, function_2564e0
};

s_slot_handler_2 g_47f8f8 =
{
	{
		0x74, 2, 0, -2, 0,
		function_256620, function_256650, 0, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	0, 0, function_256680
};

s_slot_handler_0 g_47f944 =
{
	0x75, 0, 0, -2, 0, function_256810
};

s_slot_child g_470b70[6] =
{
	{4, 1, -1, {0}, 0.f, 0, 0},
	{0x75, 1, 0, {0}, 1.f, 0, 0},
	{0x76, 1, -1, {0}, 0.f, 0, 0},
	{0x74, 1, -1, {0}, 0.f, 0, 0},
	{0x73, 1, -1, {0}, 0.f, 0, 0},
	{5, 1, -1, {0}, 0.f, 0, 0},
};

s_slot_handler_1 g_47f958 =
{
	{
		0x72, 1, NONE, -2, 0,
		function_1a8370, 0, 0, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_256af0, 6, g_470b70
};

// @retail 0x256260
short __stdcall function_256260(long actor_index)
{
	short result = 0;
	s_handler_actor_view *actor = (s_handler_actor_view *)actor_get(actor_index);
	s_character_76_view *character = (s_character_76_view *)function_1e4b50(actor_index);

	if (character && perception_get(actor->perception_index)->unknown1c > character->unknown0)
	{
		result = 3;
	}
	return result;
}

// @retail 0x2562c0
void __stdcall function_2562c0(long actor_index, s_slot *slot)
{
	s_handler_actor_view *actor = (s_handler_actor_view *)actor_get(actor_index);

	if (actor->perception_index != NONE)
	{
		perception_get(actor->perception_index)->unknown1c = 0;
	}
}

// @retail 0x256300
short __stdcall function_256300(long actor_index, s_slot *slot, bool active)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_76_state *state = (s_slot_76_state *)slot;

	if (actor->prop_index != NONE && actor_perception_index(actor_index) != NONE)
	{
		s_character_76_view *character = (s_character_76_view *)function_1e4b50(actor_index);

		if (character)
		{
			if (state->started)
			{
				state->timer++;
				if (state->timer * g_510c54->rate < character->duration)
				{
					return g_470bec;
				}
			}
			else
			{
				prop_state *prop = function_25d670(actor->prop_index);
				real range = character->range * 0.75f;
				s_ai_object_iterator iterator;
				s_handler_object_view *object;

				iterator.next_index = perception_get(actor_perception_index(actor_index))->object_index;
				iterator.index = NONE;

				while ((object = function_290c80(&iterator)) != NULL)
				{
					byte *data;
					if (!object->flags134 && (data = (byte *)object + object->ai_offset) != NULL &&
						range > distance3d(&prop->position, (real_point3d *)(data + 0x14)))
					{
						return g_470bec;
					}
				}

				state->started = true;
				return g_470bec;
			}
		}

		return g_470be8;
	}

	return g_470be8;
}

// @retail 0x256460
void __stdcall function_256460(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown4a4 = 6;
	actor->unknown4a5 = 2;
	actor->unknown4a8 = actor->prop_index;
}

// @retail 0x2564a0
short __stdcall function_2564a0(long actor_index, s_slot *slot, bool active)
{
	short result = g_470be8;

	if (actor_get(actor_index)->prop_index != NONE)
	{
		result = g_470bec;
	}
	return result;
}

// @retail 0x2564e0
void __stdcall function_2564e0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown4a4 = 1;
	actor->unknown4a5 = 2;
	actor->unknown4a8 = actor->prop_index;
}

// @retail 0x256520
bool function_256520(long actor_index, real range)
{
	s_actor_view *actor = actor_get(actor_index);
	bool result = false;

	if (actor->prop_index != NONE && actor->unknown07c != NONE)
	{
		s_prop_datum *prop = prop_ref_get(actor->prop_index);
		long member_index = element_502420_get(actor->unknown07c)->first_actor_index;

		while (member_index != NONE)
		{
			s_actor_view *member = actor_get(member_index);
			long current_index = member_index;
			member_index = member->next_index;

			if (member != actor && !member->unknown007 && member->prop_index != NONE)
			{
				s_prop_datum *other = prop_ref_get(member->prop_index);

				if (other->prop_index == prop->prop_index &&
					function_1a6fe0(current_index, 0xe) != NONE &&
					range > other->unknown28)
				{
					result = true;
				}
			}
		}
	}

	return result;
}

// @retail 0x256620
short __stdcall function_256620(long actor_index)
{
	short result = 0;

	if (function_256520(actor_index, 4.f))
	{
		result = 3;
	}
	return result;
}

// @retail 0x256650
short __stdcall function_256650(long actor_index, s_slot *slot, bool active)
{
	return function_256520(actor_index, 6.f) ? g_470bec : g_470be8;
}

// @retail 0x256680
void __stdcall function_256680(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown4a5 = 2;
	actor->unknown4a8 = actor->prop_index;
	actor->unknown4a4 = 3;
}

// @retail 0x256af0
short __stdcall function_256af0(long actor_index, short level, bool active)
{
	return function_1a79e0(actor_index, level, active);
}
