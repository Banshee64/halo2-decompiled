// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_25D690.CPP: the ai's props (props.obj): the "prop", "prop_ref"
   and "tracking" data arrays (include/props.h) */

#include "cseries.h"
#include "globals.h"
#include "props.h"
#include "unknown_26b230.h"

s_prop_type_entry g_470f10[9] =
{
	{0, 0, -1, -1, -1, {0, 0, 0}},
	{1, 5, 2, 2, 2, {0, 0, 0}},
	{2, 2, 2, 1, 0, {0, 0, 0}},
	{3, 5, 2, 1, 0, {0, 0, 0}},
	{4, 1, 1, 0, 0, {0, 0, 0}},
	{5, 1, 1, 0, 0, {0, 0, 0}},
	{6, 1, 1, 0, 0, {0, 0, 0}},
	{7, 1, 1, 0, 0, {0, 0, 0}},
	{8, 1, 1, 0, 0, {0, 0, 0}},
};

/* the actor types: a name, then the type that leads this one */
struct s_actor_type_definition
{
	char const *name;
	short unknown4;
	short leader_type;
};

s_actor_type_definition g_471010 = {"flood carrier", 0, NONE};
s_actor_type_definition g_471018 = {"crew", 0, NONE};
s_actor_type_definition g_471020 = {"elite", 0, NONE};
s_actor_type_definition g_471028 = {"engineer", 0, NONE};
s_actor_type_definition g_471030 = {"flood", 0, NONE};
s_actor_type_definition g_471038 = {"grunt", 0, 0};
s_actor_type_definition g_471040 = {"hunter", 0, NONE};
s_actor_type_definition g_471048 = {"infection", 1, NONE};
s_actor_type_definition g_471050 = {"jackal", 0, NONE};
s_actor_type_definition g_471058 = {"marine", 0, NONE};
s_actor_type_definition g_471060 = {"mounted_weapon", 0, NONE};
s_actor_type_definition g_471068 = {"prophet", 0, NONE};
s_actor_type_definition g_471070 = {"bugger", 0, NONE};
s_actor_type_definition g_471078 = {"sentinel", 0, NONE};
s_actor_type_definition g_471080 = {"juggernaut", 0, NONE};

s_actor_type_definition *g_471088[16] =
{
	&g_471020, &g_471050, &g_471038, &g_471040, &g_471028, &g_471020, &g_471058, &g_471058,
	&g_471018, &g_471030, &g_471048, &g_471010, &g_471078, &g_471078, &g_471038, &g_471060
};

bool function_1a8220(long index, short a, short b, long unknown, short c, short d, short e);
void __stdcall function_25d020(long actor_index, long prop_ref_index, long a, long b, long c, long d);
bool function_1fb7e0(long actor_index, short type, s_1fb7e0_data const *data, long target_index, long unknown);
bool function_26ba60(long prop_index, long actor_index, long clump_index);
void function_25c780(long actor_index, long prop_ref_index);
void function_25c4e0(long prop_ref_index);
real function_30bf0(real_vector3d *v);
real_point3d *function_b9dd0(long object_index, real_point3d *result);

/* the clumps of g_502420 (0x50 bytes) as the props see them */
struct s_clump_prop_view
{
	byte unknown00[0x30];
	bool unknown30;
	byte unknown31[0x50 - 0x31];
};

/* the view of a prop_ref's tracking, if it has one (function_25d740 inlined) */
inline prop_view *prop_ref_view(s_prop_datum *datum)
{
	prop_view *result = NULL;
	if (datum->tracking_index != NONE)
	{
		tracking_datum *tracking = tracking_get(datum->tracking_index);
		if (tracking)
		{
			result = &tracking->view;
		}
	}
	return result;
}

/* clears what a prop view remembers of its last update */
inline void prop_view_reset(prop_view *view)
{
	view->unknown68 = false;
	view->unknown69 = false;
	view->unknown4c = false;
	view->unknown70 = 0;
	view->unknown6c = true;
	view->unknown6d = true;
	view->unknown88 = false;
	view->unknown90 = 0;
	view->unknown8a = 0;
}

// @retail 0x25ac00
void function_25ac00(long prop_ref_index, long actor_index)
{
	if (actor_get(actor_index)->unknown018 != NONE)
	{
		s_prop_datum *datum = prop_ref_get(prop_ref_index);
		prop_datum *prop = prop_get(datum->prop_index);

		if (!prop_state_get(datum)->unknown5e && prop->unknown24 && prop->unknown3c)
		{
			prop_view *view = function_25d740((s_prop_node *)datum);
			real range;

			if (prop->unknown23)
			{
				range = 15.f;
			}
			else if (view && view->unknown39 <= 2)
			{
				range = 10.f;
			}
			else
			{
				range = 3.f;
			}

			bool close = range > datum->unknown28;
			if ((prop->unknown23 && view && view->unknown2a) || close)
			{
				function_1fb7e0(actor_index, 0xc2, NULL, datum->object_index, NONE);
				prop->unknown3c = false;
			}
		}
	}
}

// @retail 0x25bba0
void function_25bba0(long actor_index, long unknown)
{
	s_actor_prop_view *actor = actor_prop_view_get(actor_index);

	if (actor->unknown684 <= 0)
	{
		real ticks = g_510c54->ticks_per_second * 1.5f;
		long rounded;
		__asm
		{
			fld ticks
			fistp rounded
		}
		actor->unknown686 = (short)rounded;
		actor->unknown688 = 1;
		actor->unknown68c = unknown;
		actor->unknown684 = 0;
	}
}

// @retail 0x25bc20
void function_25bc20(long actor_index, long unknown)
{
	if (actor_prop_view_get(actor_index)->unknown684 <= 0)
	{
		real ticks = g_510c54->ticks_per_second * 4.f;
		long rounded;
		__asm
		{
			fld ticks
			fistp rounded
		}
		actor_prop_view_get(actor_index)->unknown686 = (short)rounded;
		actor_prop_view_get(actor_index)->unknown688 = 6;
		actor_prop_view_get(actor_index)->unknown68c = unknown;
		actor_prop_view_get(actor_index)->unknown684 = 0;
	}
}

// @retail 0x25bcb0
void function_25bcb0(long actor_index, long prop_ref_index)
{
	function_25d020(actor_index, prop_ref_index, 2, NONE, NONE, NONE);
	actor_get(actor_index)->unknown223 = false;
}

// @retail 0x25bf10
void function_25bf10(long actor_index, long other_index, long prop_ref_index)
{
	s_actor_view *actor = actor_get(actor_index);
	s_actor_view *other = actor_get(other_index);
	bool friendly;

	if (prop_ref_index != NONE)
	{
		friendly = prop_get(prop_ref_get(prop_ref_index)->prop_index)->unknown23;
	}
	else
	{
		friendly = game_team_is_enemy(actor->unknown024, other->unknown024);
	}

	if (friendly)
	{
		return;
	}

	real_vector3d delta;

	vector3d_from_points3d(&other->position, &actor->position, &delta);
	if (magnitude_squared3d(&delta) < 64.f)
	{
		if (!other)
		{
			return;
		}
		if (other->unknown004 == g_471088[actor->unknown004]->leader_type)
		{
			function_1a8220(actor_index, 0x3e, 1, 3, 1, 0x2f, 3);
		}
		else if (other->unknown004 == actor->unknown004)
		{
			function_1a8220(actor_index, 0x3f, 1, 3, 1, 0x2f, 3);
		}
	}

	if (other && other->unknown004 == actor->unknown004)
	{
		function_1a8220(actor_index, 0x18, 1, 3, 1, NONE, 0);
	}
}

// @retail 0x25c170
void function_25c170(void)
{
	g_50241c = data_new_inlined("prop", 0x100, sizeof(prop_datum), 0, g_510c2c);
	g_502418 = data_new_inlined("prop_ref", 0x400, sizeof(s_prop_datum), 0, g_510c2c);
	g_502414 = data_new_inlined("tracking", 0x64, sizeof(tracking_datum), 0, g_510c2c);
}

// @retail 0x25c3e0
void prop_state_initialize(prop_state *state)
{
	state->unknown40 = NONE;
	state->unknown66 = false;
	state->unknown62 = false;
	state->unknown5e = false;
	state->unknown64 = false;
	state->unknown61 = false;
	state->unknown5f = false;
	state->unknown00 = NONE;
	state->unknown60 = true;
	state->unknown63 = false;
	state->unknown69 = false;
	state->unknown65 = false;
	state->unknown3c = NONE;
	state->unknown67 = false;
	state->unknown48 = *g_468788;
	state->unknown54 = NONE;
	state->unknown44 = NONE;
	state->unknown58 = false;
}

// @retail 0x25c440
void prop_view_initialize(prop_view *view)
{
	view->unknown06 = 4;
	view->unknown8c = 4;
	view->unknown10 = NONE;
	view->unknown14 = NONE;
	view->unknown50 = NONE;
	view->unknown8a = 0;
	view->unknown54 = 0.f;
	view->unknown58 = 0.f;
	view->unknown5c = 0.f;
	view->unknown60 = 0.f;
	view->unknowna2 = false;
	view->unknown94 = *g_4687a4;
	view->unknownb0 = NONE;
	view->unknown64 = false;
	view->unknown66 = 0;
	prop_view_reset(view);
}

// @retail 0x25c4e0
void function_25c4e0(long prop_ref_index)
{
	s_prop_datum *datum = prop_ref_get(prop_ref_index);
	prop_view *view = prop_ref_view(datum);

	if (view)
	{
		prop_view_reset(view);
	}

	prop_datum *prop = prop_get(datum->prop_index);
	prop->unknown32 = false;
	prop->unknown33 = false;
	prop->unknown34 = false;
}

// @retail 0x25c780
void function_25c780(long actor_index, long prop_ref_index)
{
	s_prop_datum *datum = prop_ref_get(prop_ref_index);
	datum_delete(g_502414, datum->tracking_index);
	datum->tracking_index = NONE;

	s_actor_prop_view *actor = actor_prop_view_get(actor_index);
	for (short i = 0; i < 8; i++)
	{
		if (actor->tracked_prop_indices[i] == prop_ref_index)
		{
			actor->tracked_prop_indices[i] = NONE;
			break;
		}
	}

	if (g_470f10[datum->type].unknown8 == 2)
	{
		datum->state = 0;
	}
	else if (datum->state >= 3 || datum->state == 2)
	{
		datum->state = 1;
	}
}

// @retail 0x25c820
void function_25c820(long prop_ref_index, long actor_index)
{
	s_prop_datum *datum = prop_ref_get(prop_ref_index);

	if (datum->tracking_index != NONE)
	{
		function_25c780(actor_index, prop_ref_index);
	}
	datum->state = 0;
	datum->unknown10 = 0.f;
}

// @retail 0x25cca0
short function_25cca0(long prop_ref_index)
{
	short result = 0;
	if (prop_ref_get(prop_ref_index)->unknown1c == 3)
	{
		result = 3;
	}
	return result;
}

// @retail 0x25d420
void function_25d420(long prop_ref_index, short type, long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);

	if (prop_ref_index != NONE)
	{
		s_prop_datum *datum = prop_ref_get(prop_ref_index);
		prop_view *view = prop_view_get(prop_ref_index);

		if (view)
		{
			view->unknown69 = true;
			if (type == 3)
			{
				view->unknown4c = true;
			}

			if (function_26ba60(datum->prop_index, actor_index, actor->unknown07c))
			{
				bool started;

				switch (type)
				{
				case 1:
					started = function_1fb7e0(actor_index, 0x33, NULL, datum->object_index, NONE);
					break;
				case 2:
					started = function_1fb7e0(actor_index, 0x32, NULL, datum->object_index, NONE);
					break;
				case 3:
					started = function_1fb7e0(actor_index, 0x35, NULL, datum->object_index, NONE) ||
						function_1fb7e0(actor_index, 0x3b, NULL, datum->object_index, NONE);
					break;
				default:
					return;
				}

				if (started)
				{
					prop_get(datum->prop_index)->unknown34 = true;
				}
			}
		}
	}
}

// @retail 0x25d510
void function_25d510(long actor_index)
{
	long prop_ref_index = actor_get(actor_index)->first_prop_index;

	for (;;)
	{
		if (prop_ref_index == NONE)
		{
			break;
		}
		s_prop_datum *datum = prop_ref_get(prop_ref_index);
		prop_ref_index = datum->next_index;

		if (datum->state >= 3)
		{
			prop_view *view = prop_ref_view(datum);
			if (view)
			{
				view->unknown69 = true;
			}
		}
	}
}

// @retail 0x25d580
void function_25d580(long actor_index)
{
	long prop_ref_index = actor_get(actor_index)->first_prop_index;

	for (;;)
	{
		if (prop_ref_index == NONE)
		{
			break;
		}
		s_prop_datum *datum = prop_ref_get(prop_ref_index);
		long current_index = prop_ref_index;
		prop_ref_index = datum->next_index;

		if (datum->state >= 3)
		{
			function_25c820(current_index, actor_index);
		}
	}
}

// @retail 0x25d670
prop_state *function_25d670(long prop_ref_index)
{
	return prop_state_get(prop_ref_get(prop_ref_index));
}

// @retail 0x25d690
prop_state *prop_state_get(s_prop_datum *datum)
{
	if (g_470f10[datum->type].kind == 1)
	{
		return &prop_get(datum->prop_index)->state;
	}
	if (datum->tracking_index != NONE)
	{
		return &tracking_get(datum->tracking_index)->state;
	}
	return &prop_get(datum->prop_index)->state;
}

// @retail 0x25d700
prop_view *prop_view_get(long index)
{
	long tmp0 = index & 0xffff;
	prop_view *result = NULL;
	s_prop_datum *datum = (s_prop_datum *)(g_502418->data + tmp0 * sizeof(s_prop_datum));
	if (datum->tracking_index != NONE)
	{
		byte *base = g_502414->data + (datum->tracking_index & 0xffff) * sizeof(tracking_datum);
		if (base)
		{
			result = (prop_view *)(base + 0x70);
		}
	}
	return result;
}

// @retail 0x25d740
prop_view *function_25d740(s_prop_node *node)
{
	prop_view *result = NULL;
	if (node->tracking_index != NONE)
	{
		byte *base = g_502414->data + (node->tracking_index & 0xffff) * sizeof(tracking_datum);
		if (base)
		{
			result = (prop_view *)(base + 0x70);
		}
	}
	return result;
}

// @retail 0x25d970
bool function_25d970(s_prop_datum *datum)
{
	if (datum->state >= 3)
	{
		prop_view *view = prop_ref_view(datum);
		if (view)
		{
			return !view->unknown88;
		}
	}
	return false;
}

// @retail 0x25d9b0
bool function_25d9b0(long prop_index)
{
	bool result = false;
	prop_view *view = prop_view_get(prop_index);

	if (view)
	{
		result = view->unknown50 != NONE;
	}
	return result;
}

// @retail 0x25da00
bool function_25da00(s_prop_node_view *node)
{
	s_prop_datum *datum = (s_prop_datum *)node;

	if (datum->state >= 3)
	{
		prop_view *view = prop_ref_view(datum);
		if (view)
		{
			return view->unknown88;
		}
	}
	return false;
}

// @retail 0x25da40
bool function_25da40(s_prop_datum *datum)
{
	prop_datum *prop = prop_get(datum->prop_index);

	if (datum->tracking_index != NONE)
	{
		if (prop->actor_index != NONE)
		{
			s_actor_view *actor = actor_get(prop->actor_index);
			if (actor->unknown004 == 15 || (actor->unknown267 && !actor->unknown268))
			{
				return false;
			}
		}
		return true;
	}

	return prop->unknown25 && !prop->unknown23;
}

// @retail 0x25dac0
short function_25dac0(long actor_index)
{
	short count = 0;
	long prop_ref_index = actor_get(actor_index)->first_prop_index;

	for (;;)
	{
		if (prop_ref_index == NONE)
		{
			break;
		}
		s_prop_datum *datum = prop_ref_get(prop_ref_index);
		long current_index = prop_ref_index;
		prop_ref_index = datum->next_index;

		if (datum->tracking_index != NONE)
		{
			function_25c820(current_index, actor_index);
			count++;
		}
	}
	return count;
}

// @retail 0x25b620
void function_25b620(long prop_ref_index, long actor_index, bool unknown)
{
	s_prop_datum *datum = prop_ref_get(prop_ref_index);
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown07c != NONE && !((s_clump_prop_view *)element_502420_get(actor->unknown07c))->unknown30)
	{
		if (unknown)
		{
			function_1fb7e0(actor_index, 0xd, NULL, datum->object_index, NONE);
		}
		else
		{
			function_1fb7e0(actor_index, 0xf, NULL, datum->object_index, NONE);
		}
	}
}

// @retail 0x25c860
void function_25c860(long prop_ref_index)
{
	s_prop_datum *datum = prop_ref_get(prop_ref_index);
	prop_state *state = prop_state_get(datum);
	prop_view *view = prop_ref_view(datum);
	bool tracked = datum->state >= 3;

	datum->state = 3;
	if (view && !tracked)
	{
		real ticks;
		long rounded;

		view->unknown90 = 0;
		view->unknowna2 = true;
		view->unknowna4 = state->position;
		view->unknown8c = view->unknown06;
		view->unknown8a = datum->unknown27;
		function_25c4e0(prop_ref_index);

		ticks = g_510c54->ticks_per_second * 30.f;
		__asm
		{
			fld ticks
			fistp rounded
		}
		view->unknown8e = (short)rounded;
		view->unknownb0 = g_510c54->game_time;
		view->unknown64 = false;
		view->unknown66 = 0;

		if (view->unknown10 >= 0)
		{
			real_point3d origin;

			function_b9dd0(datum->object_index, &origin);
			view->unknown94.i = origin.x - state->position.x;
			view->unknown94.j = origin.y - state->position.y;
			view->unknown94.k = origin.z - state->position.z;
			function_30bf0(&view->unknown94);
		}
		else
		{
			view->unknown94 = *g_4687a4;
		}
	}
}

// @retail 0x25d610
bool function_25d610(s_prop_datum *datum)
{
	s_prop_type_entry *entry = &g_470f10[datum->type];
	prop_datum *prop = prop_get(datum->prop_index);
	short type = prop->unknown04;
	bool result = false;

	if (type != 1 || (entry->kind && (type != prop->unknown04 || entry->unknown8 >= 2 || prop->unknown25)))
	{
		result = true;
	}
	return result;
}

long function_1e3480(long object_index);
long function_26ace0(long object_index, long actor_index, short type);
struct s_node_view;
void function_26be00(long actor_index, s_iterator *iterator);
s_node_view *function_26be30(s_iterator *iterator);

/* the actor's prop_ref of an object: the object itself, or the actor that
   controls it */
// @retail 0x25d770
long function_25d770(long actor_index, long object_index)
{
	long object_actor_index = function_1e3480(object_index);
	s_iterator iterator;
	s_prop_datum *datum;

	function_26be00(actor_index, &iterator);
	while ((datum = (s_prop_datum *)function_26be30(&iterator)) != NULL)
	{
		prop_datum *prop = prop_get(datum->prop_index);

		if (datum->state >= 1)
		{
			if (datum->object_index == object_index)
			{
				return iterator.index;
			}
			if (prop->unknown22 && prop->actor_index != NONE && prop->actor_index == object_actor_index)
			{
				return iterator.index;
			}
		}
	}
	return NONE;
}

/* the same lookup over all of the actor's prop_refs, creating the prop_ref
   if asked */
// @retail 0x25d810
long function_25d810(long object_index, long actor_index, bool create)
{
	long result = NONE;

	if (object_index != NONE)
	{
		s_actor_prop_view *actor = actor_prop_view_get(actor_index);
		long object_actor_index = function_1e3480(object_index);

		if (object_actor_index != actor_index)
		{
			s_iterator iterator;
			s_prop_datum *datum;

			function_26be00(actor_index, &iterator);
			while ((datum = (s_prop_datum *)function_26be30(&iterator)) != NULL)
			{
				prop_datum *prop = prop_get(datum->prop_index);

				if (datum->object_index == object_index ||
					prop->unknown22 && prop->actor_index != NONE && prop->actor_index == object_actor_index)
				{
					result = iterator.index;
					break;
				}
			}
			if (result == NONE && create && actor->unknown009 && actor->unknown07c != NONE)
			{
				result = function_26ace0(object_index, actor_index, 3);
				if (result != NONE)
				{
					prop_get(prop_ref_get(result)->prop_index)->unknown10 = g_510c54->game_time + g_510c54->ticks_per_second * 60;
				}
			}
		}
	}
	return result;
}

void __stdcall function_25c230(long actor_index, long prop_ref_index, short unknown);
long __stdcall function_25c570(long prop_ref_index, short unknown);

// @retail 0x25c3a0
long function_25c3a0(long actor_index, long prop_ref_index, short unknown)
{
	s_prop_datum *datum = prop_ref_get(prop_ref_index);

	if (datum->state < 1)
	{
		function_25c230(actor_index, prop_ref_index, unknown);
		return datum->tracking_index;
	}
	return function_25c570(prop_ref_index, unknown);
}
