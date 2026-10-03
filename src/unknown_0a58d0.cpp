// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0A58D0.CPP: the simulation entity database: an entity's object */

#include "cseries.h"
#include "globals.h"
#include "object_type_definitions.h"

/* the entity definitions (g_4cf784, src/unknown_067eb0.cpp): a count, then
   one definition per entity type */
extern long g_4cf784;

struct s_entity_definitions
{
	long count;
	c_object_type_definition *definitions[1];
};

/* an entity in the database (0x20 bytes) */
struct s_simulation_entity
{
	long identifier;
	short type;
	bool field_6;
	byte unknown07;
	long object_index;
	byte unknown0c[0x14];
};

struct s_simulation_entity_table
{
	s_simulation_entity *try_get(long entity_index);

	byte unknown00[0x14];
	s_simulation_entity entities[0x400];
};

struct s_simulation_entity_database
{
	byte unknown00[0x2098];
	s_simulation_entity_table table;
};

struct s_simulation_world_view
{
	byte unknown00[4];
	s_simulation_entity_database *database;
};

inline s_simulation_entity *s_simulation_entity_table::try_get(long entity_index)
{
	s_simulation_entity *result;
	s_simulation_entity *entity = &entities[entity_index & 0x3ff];
	if (entity->identifier == entity_index)
		result = entity;
	else
		result = 0;
	return result;
}

/* not matched: retail takes the index in ecx and keeps a null test of the
   entity after comparing its identifier */
// @retail 0xa58d0
long function_a58d0(long entity_index)
{
	long object_index = NONE;
	if (entity_index != NONE)
	{
		s_simulation_world_view *world = (s_simulation_world_view *)g_4cf77c;
		s_simulation_entity *entity = world->database->table.try_get(entity_index);
		if (entity)
		{
			s_entity_definitions *definitions = (s_entity_definitions *)g_4cf784;
			if (definitions->definitions[entity->type]->v7((s_entity *)entity) && entity->object_index != NONE)
				object_index = entity->object_index;
		}
	}
	return object_index;
}
