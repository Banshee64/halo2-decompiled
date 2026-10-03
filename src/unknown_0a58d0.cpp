// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0A58D0.CPP: the simulation entity database: an entity's object */

#include "cseries.h"
#include "globals.h"
#include "object_type_definitions.h"
#include "simulation_entity_database.h"

/* the entity an index stands for, or none when its salt is stale; retail
   has no copy of its own (LTCG inlines it everywhere), and inlining it from
   here keeps the null test after the identifier comparison that retail has */
s_simulation_entity *simulation_entity_try_get(s_simulation_entity_table *table, long entity_index)
{
	s_simulation_entity *result;
	s_simulation_entity *entity = &table->entities[entity_index & 0x3ff];
	if (entity->identifier == entity_index)
		result = entity;
	else
		result = 0;
	return result;
}

/* 0xa58d0, kept out of the build: retail takes the index in ecx (the
   __fastcall the stub in src/stubs/unknown_09a9f0.cpp has), our LTCG passes it
   in eax, which breaks the matched caller 0xa3a20. Not matched itself:
   retail also keeps a null test of the entity after comparing its
   identifier. */
#if 0
long function_a58d0(long entity_index)
{
	long object_index = NONE;
	if (entity_index != NONE)
	{
		s_simulation_world_view *world = (s_simulation_world_view *)g_4cf77c;
		s_simulation_entity *entity = simulation_entity_try_get(&world->database->table, entity_index);
		if (entity)
		{
			s_entity_definitions *definitions = (s_entity_definitions *)g_4cf784;
			if (definitions->definitions[entity->type]->v7((s_entity *)entity) && entity->object_index != NONE)
				object_index = entity->object_index;
		}
	}
	return object_index;
}
#endif
