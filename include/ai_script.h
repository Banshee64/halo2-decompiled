/* AI_SCRIPT.H: the ai references of the script functions (src/ai_script.cpp) */

#ifndef AI_SCRIPT_H
#define AI_SCRIPT_H

#include "cseries.h"
#include "squads.h"

/* walks the actors an ai index names */
struct s_ai_actor_iterator
{
	long squad_index;
	long squad_group_index;
	long actor_index;
	bool single_actor;
	s_squad_actor_iterator actor_iterator;
	s_squad_group_iterator group_iterator;
};

void ai_actor_iterator_new(long ai_index, s_ai_actor_iterator *iterator);
s_actor_datum *ai_actor_iterator_next(s_ai_actor_iterator *iterator);

long ai_index_get_actor(long ai_index);
long function_272c90(long ai_index);
long function_272ff0(long object_index);
void function_273150(long list_index);
void function_2738a0(long ai_index, bool flag);
void function_273900(long ai_index, bool flag);
void function_2739d0(long ai_index, bool flag);
void function_273ef0(long ai_index, bool flag);
long function_273f30(long ai_index, short mode, long *actor_count, real *average_vitality);
void function_274a50(long ai_index, long vehicle_index, long filter_range, bool load);
void function_274da0(long ai_index, long filter_range);
void function_275a50(long ai_index, bool flag);
void function_275b20(long ai_index, bool value);
void function_275ad0(long unit_index, bool flag);
short function_275d70(long ai_index);
long function_275e20(long ai_index);
bool function_275fc0(long vehicle_index, bool flag);
short function_276050(long ai_index);
void function_2760a0(long actor_index, long script_index, long name, long sound_index, real scale, real pitch);
bool function_276380(long ai_index);
void function_276440(long ai_index, short script_index);
void function_276480(long ai_index, short script_index);
bool function_2766f0(long ai_index, long name_index);
bool function_276770(long ai_index, long name_index);
short function_2767f0(long ai_index);
void function_276b40(long object_index, real a, real b, real c);
void function_276cc0(real a, real b, real c);

#endif
