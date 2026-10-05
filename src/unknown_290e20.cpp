// @flags /O2 /Gr
/* UNKNOWN_290E20.CPP: the scenario's named ai triggers (0x30 bytes each at
   +0x248 of g_4e0350): a lookup by name and the test whether enough of a
   trigger's conditions hold for a squad or squad group (outside functions
   lane A's ai script query 0x275d10 needs) */

#include "unknown_11c920.h"
#include "globals.h"
#include <string.h>

/* a condition of a trigger (0x38 bytes) */
struct s_ai_trigger_condition
{
	byte unknown00[0x38];
};

struct s_ai_trigger
{
	char name[0x20];
	byte flags;
	byte unknown21[3];
	short combine_mode;
	byte unknown26[2];
	long condition_count;
	s_ai_trigger_condition *conditions;
};

struct s_scenario_ai_triggers_view
{
	byte unknown000[0x248];
	long trigger_count;
	s_ai_trigger *triggers;
};

/* the triggers that have fired once (ai.cpp) */
extern void *g_5044cc;

bool __stdcall function_2912c0(s_ai_trigger_condition *condition, long squad_group_index, bool *result);
bool function_290f60(s_ai_trigger_condition *condition, bool *result, long squad_index);

// @retail 0x291790
short ai_trigger_find_by_name(char const *name)
{
	short result = NONE;
	s_scenario_ai_triggers_view *scenario = (s_scenario_ai_triggers_view *)g_4e0350;

	if (scenario)
	{
		short i;
		for (i = 0; i < scenario->trigger_count; i++)
		{
			s_ai_trigger *trigger = &((s_scenario_ai_triggers_view *)g_4e0350)->triggers[i];
			if (!_strnicmp(trigger->name, name, 0x20))
			{
				result = i;
				break;
			}
		}
	}
	return result;
}

// @retail 0x290e20
bool function_290e20(short trigger_index, long squad_index, long squad_group_index)
{
	s_ai_trigger *trigger = &((s_scenario_ai_triggers_view *)g_4e0350)->triggers[trigger_index];
	short count;
	short needed;
	short i;
	bool result;

	if ((trigger->flags & 1) && (((dword *)g_5044cc)[trigger_index >> 5] & (1 << (trigger_index & 0x1f))))
		return true;

	count = 0;
	result = false;
	if (trigger->combine_mode == 0)
		needed = 1;
	else
		needed = (short)trigger->condition_count;

	for (i = 0; i < trigger->condition_count; i++)
	{
		s_ai_trigger_condition *condition = &trigger->conditions[i];
		bool holds;

		if (squad_group_index != NONE && function_2912c0(condition, squad_group_index, &result))
			holds = true;
		else if (squad_index != NONE)
			holds = function_290f60(condition, &result, squad_index);
		else if (squad_group_index == NONE)
			holds = function_290f60(condition, &result, NONE);
		else
			holds = false;

		if (holds)
		{
			count++;
			if (count >= needed)
				break;
		}
	}

	if (count < needed)
		return false;
	if (trigger->flags & 1)
		((dword *)g_5044cc)[trigger_index >> 5] |= 1 << (trigger_index & 0x1f);
	return true;
}
