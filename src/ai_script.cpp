// @flags /O2 /Ob1 /arch:SSE /Gr
/* AI_SCRIPT.CPP: the ai references of the script functions. An ai index
   names a squad, a squad group, an actor or a starting location by its type
   in the top two bits. */

#include "cseries.h"
#include "globals.h"
#include "squads.h"
#include "ai_script.h"
#include "command_scripts.h"

enum
{
	_ai_index_type_squad = 0,
	_ai_index_type_squad_group,
	_ai_index_type_actor,
	_ai_index_type_starting_location
};

/* walks the squads an ai index names */
struct s_ai_squad_iterator
{
	s_squad_group_iterator group_iterator;
	long squad_group_index;
	long squad_index;
	long next_squad_index;
};

/* the scenario's squads (g_4e0350, globals.h), 0x74 bytes each, and their
   starting locations, 0x64 bytes each */
struct s_scenario_starting_location
{
	long name;
	byte unknown04[0x64 - 4];
};

struct s_scenario_squad
{
	byte unknown00[0x48];
	long starting_location_count;
	s_scenario_starting_location *starting_locations;
	byte unknown50[0x74 - 0x50];
};

struct s_scenario_squads_view
{
	byte unknown000[0x160];
	long squad_count;
	s_scenario_squad *squads;
};

/* the objects (a local view) */
struct s_ai_script_object
{
	byte unknown000[0xec];
	real body_vitality;
};

struct s_ai_script_object_header
{
	byte unknown00[8];
	s_ai_script_object *object;
};

inline s_ai_script_object *ai_script_object_get(long object_index)
{
	return ((s_ai_script_object_header *)g_4e0300->data)[object_index & 0xffff].object;
}

inline long ai_index_get_type(long ai_index)
{
	return (dword)ai_index >> 30;
}

/* the actor an actor or starting location index names */
// @retail 0x272b70
long ai_index_get_actor(long ai_index)
{
	long actor_index = NONE;
	long type = ai_index_get_type(ai_index);
	if (type == _ai_index_type_actor)
	{
		actor_index = data_datum_index(g_4f55f0, ai_index & 0xffff);
	}
	else if (type == _ai_index_type_starting_location)
	{
		short squad_index = (short)((ai_index >> 16) & 0x3fff);
		short starting_location_index = (short)ai_index;
		s_scenario_squads_view *scenario = (s_scenario_squads_view *)g_4e0350;
		if (squad_index >= 0 && squad_index < scenario->squad_count)
		{
			s_scenario_squad *squad = &scenario->squads[(word)squad_index];
			if (starting_location_index >= 0 && starting_location_index < squad->starting_location_count)
			{
				s_scenario_starting_location *starting_location = &squad->starting_locations[starting_location_index];
				s_squad_actor_iterator iterator;
				squad_actor_iterator_new(&iterator, squad_index);
				s_actor_datum *actor;
				while ((actor = squad_actor_iterator_next(&iterator)) != NULL)
				{
					if (actor->starting_location_name == starting_location->name)
					{
						actor_index = iterator.actor_index;
						break;
					}
				}
			}
		}
	}
	return actor_index;
}

// @retail 0x272d50
void ai_squad_iterator_new(s_ai_squad_iterator *iterator, long ai_index)
{
	if (ai_index_get_type(ai_index) == _ai_index_type_squad_group)
	{
		iterator->squad_group_index = ai_index & 0xffff;
		squad_group_iterator_new(&iterator->group_iterator, ai_index & 0xffff);
	}
	else if (ai_index_get_type(ai_index) == _ai_index_type_squad)
	{
		iterator->squad_group_index = NONE;
		iterator->next_squad_index = ai_index & 0xffff;
	}
	else
	{
		iterator->squad_group_index = NONE;
		iterator->next_squad_index = NONE;
	}
}

// @retail 0x272d90
void ai_actor_iterator_new(s_ai_actor_iterator *iterator, long ai_index)
{
	short type = (short)(ai_index_get_type(ai_index) & 3);
	iterator->single_actor = false;
	if (type == _ai_index_type_squad_group)
	{
		iterator->squad_group_index = ai_index & 0xffff;
		squad_group_iterator_new(&iterator->group_iterator, ai_index & 0xffff);
		squad_group_iterator_next(&iterator->group_iterator);
		iterator->squad_index = iterator->group_iterator.squad_index;
		squad_actor_iterator_new(&iterator->actor_iterator, iterator->squad_index);
		iterator->actor_index = NONE;
	}
	else if (type == _ai_index_type_squad)
	{
		iterator->squad_group_index = NONE;
		iterator->squad_index = ai_index & 0xffff;
		iterator->actor_index = NONE;
		squad_actor_iterator_new(&iterator->actor_iterator, ai_index & 0xffff);
	}
	else if (type == _ai_index_type_actor || type == _ai_index_type_starting_location)
	{
		iterator->squad_index = NONE;
		iterator->squad_group_index = NONE;
		iterator->actor_index = ai_index_get_actor(ai_index);
		iterator->single_actor = true;
	}
	else
	{
		iterator->squad_index = NONE;
		iterator->squad_group_index = NONE;
		iterator->actor_index = NONE;
	}
}

// @retail 0x272e20
s_actor_datum *ai_actor_iterator_next(s_ai_actor_iterator *iterator)
{
	s_actor_datum *actor = NULL;
	while (iterator->squad_index != NONE)
	{
		actor = squad_actor_iterator_next(&iterator->actor_iterator);
		iterator->actor_index = iterator->actor_iterator.actor_index;
		if (actor)
			return actor;
		iterator->actor_index = NONE;
		if (iterator->squad_group_index == NONE)
			return actor;
		s_squad_datum *squad = squad_group_iterator_next(&iterator->group_iterator);
		iterator->squad_index = iterator->group_iterator.squad_index;
		if (!squad)
			return NULL;
		squad_actor_iterator_new(&iterator->actor_iterator, iterator->squad_index);
		actor = NULL;
	}
	if (iterator->single_actor)
	{
		if (iterator->actor_index != NONE)
			actor = actor_datum_get(iterator->actor_index);
		iterator->single_actor = false;
		return actor;
	}
	return NULL;
}

/* the objects (a local view) */
struct s_ai_script_unit
{
	byte unknown000[0xaa];
	byte object_type;
	byte unknown0ab[0x12c - 0xab];
	long actor_index;
};

/* the ai index of the actor of a unit */
// @retail 0x272ff0
long function_272ff0(long object_index)
{
	if (object_index != NONE)
	{
		s_ai_script_unit *unit = (s_ai_script_unit *)ai_script_object_get(object_index);
		if ((1 << unit->object_type) & 3)
		{
			long actor_index = unit->actor_index;
			if (actor_index != NONE)
				return (actor_index & 0xffff) | 0x80000000;
		}
	}
	return 0xc3e703e7;
}
inline s_squad_datum *ai_squad_iterator_next(s_ai_squad_iterator *iterator)
{
	s_squad_datum *squad = NULL;
	if (iterator->squad_group_index == NONE)
	{
		if (iterator->next_squad_index != NONE)
		{
			iterator->squad_index = iterator->next_squad_index;
			iterator->next_squad_index = NONE;
			squad = squad_get(iterator->squad_index);
		}
	}
	else
	{
		squad = squad_group_iterator_next(&iterator->group_iterator);
		iterator->squad_index = iterator->group_iterator.squad_index;
	}
	return squad;
}

/* the clump objects (g_502420), 0x50 bytes each */
struct s_ai_script_clump_object
{
	byte unknown00[0x3c];
	bool flag3c;
	byte unknown3d[0x50 - 0x3d];
};

// @retail 0x2738a0
void function_2738a0(long ai_index, bool flag)
{
	if (ai_index != NONE)
	{
		s_ai_actor_iterator iterator;
		ai_actor_iterator_new(&iterator, ai_index);
		s_actor_datum *actor = ai_actor_iterator_next(&iterator);
		while (actor)
		{
			actor->flag00c = flag;
			if (flag && actor->clump_object_index != NONE)
				((s_ai_script_clump_object *)g_502420->data)[actor->clump_object_index & 0xffff].flag3c = true;
			actor = ai_actor_iterator_next(&iterator);
		}
	}
}

inline void squad_set_flag1(long squad_index, bool flag)
{
	if (g_4f55d0->active)
		squad_get(squad_index)->flag1 = flag;
}

inline void squad_set_flag0(long squad_index, bool flag)
{
	if (g_4f55d0->active)
		squad_get(squad_index)->flag0 = flag;
}

// @retail 0x273900
void function_273900(long ai_index, bool flag)
{
	if (ai_index != NONE)
	{
		s_ai_squad_iterator iterator;
		ai_squad_iterator_new(&iterator, ai_index);
		while (ai_squad_iterator_next(&iterator))
			squad_set_flag1(iterator.squad_index, flag);
	}
}

// @retail 0x2739d0
void function_2739d0(long ai_index, bool flag)
{
	if (ai_index != NONE)
	{
		switch (ai_index_get_type(ai_index))
		{
		case _ai_index_type_squad:
		case _ai_index_type_squad_group:
		{
			s_ai_squad_iterator iterator;
			ai_squad_iterator_new(&iterator, ai_index);
			while (ai_squad_iterator_next(&iterator))
				squad_set_flag0(iterator.squad_index, flag);
			break;
		}
		case _ai_index_type_actor:
		case _ai_index_type_starting_location:
		{
			long actor_index = ai_index_get_actor(ai_index);
			if (actor_index != NONE)
				actor_datum_get(actor_index)->flag228 = flag;
			break;
		}
		}
	}
}

// @retail 0x273ef0
void function_273ef0(long ai_index, bool flag)
{
	s_ai_actor_iterator iterator;
	ai_actor_iterator_new(&iterator, ai_index);
	s_actor_datum *actor = ai_actor_iterator_next(&iterator);
	while (actor)
	{
		actor->flag223 = flag;
		actor = ai_actor_iterator_next(&iterator);
	}
}

/* counts the actors an ai index names (mode 0 and 1 pick a count of each
   squad, 2 their difference) and averages their vitality */
// @retail 0x273f30
long function_273f30(long ai_index, short mode, long *actor_count, real *average_vitality)
{
	long result = 0;
	long count = 0;
	real vitality = 0.0f;
	if ((short)(ai_index_get_type(ai_index) & 3) == _ai_index_type_actor || (short)(ai_index_get_type(ai_index) & 3) == _ai_index_type_starting_location)
	{
		long actor_index = ai_index_get_actor(ai_index);
		if (actor_index != NONE)
		{
			s_actor_datum *actor = actor_datum_get(actor_index);
			vitality = ai_script_object_get(actor->unit_index)->body_vitality;
			result = 1;
			count = 1;
		}
	}
	else
	{
		s_ai_squad_iterator iterator;
		ai_squad_iterator_new(&iterator, ai_index);
		s_squad_datum *squad;
		while ((squad = ai_squad_iterator_next(&iterator)) != NULL)
		{
			switch (mode)
			{
			case 0:
				result += squad->count_a;
				break;
			case 1:
				result += squad->count_c;
				break;
			default:
			{
				long difference = squad->count_a - squad->count_c;
				result += difference < 0 ? 0 : difference;
				break;
			}
			}
			vitality += squad->actor_count * squad->value10;
			count += squad->actor_count;
		}
	}
	if (actor_count)
		*actor_count = count;
	if (average_vitality)
	{
		if (count > 0)
			*average_vitality = vitality / count;
		else
			*average_vitality = 0.0f;
	}
	return result;
}

// @retail 0x275a50
void function_275a50(long ai_index, bool flag)
{
	if (g_4f55d0->active && ai_index != NONE)
	{
		s_ai_squad_iterator iterator;
		ai_squad_iterator_new(&iterator, ai_index);
		s_squad_datum *squad;
		while ((squad = ai_squad_iterator_next(&iterator)) != NULL)
		{
			if (flag)
				squad->flag9 = true;
			else
				squad->flag9 = false;
		}
	}
}

// @retail 0x275ad0
void function_275ad0(long unit_index, bool flag)
{
	if (unit_index != NONE)
	{
		long actor_index = ((s_ai_script_unit *)ai_script_object_get(unit_index))->actor_index;
		if (actor_index != NONE)
		{
			s_actor_datum *actor = actor_datum_get(actor_index);
			if (actor->flag00a)
				actor->flag00b = flag;
		}
	}
}

/* the length of the chain of command scripts of the actor an ai index names */
// @retail 0x2767f0
short function_2767f0(long ai_index)
{
	short count = 0;
	long type = ai_index_get_type(ai_index);
	if (type == _ai_index_type_actor || type == _ai_index_type_starting_location)
	{
		long actor_index = ai_index_get_actor(ai_index);
		if (actor_index != NONE)
		{
			long script_index = actor_datum_get(actor_index)->command_script_index;
			while (script_index != NONE)
			{
				script_index = command_script_get(script_index)->next_index;
				count++;
			}
		}
	}
	return count;
}