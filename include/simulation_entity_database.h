/* SIMULATION_ENTITY_DATABASE.H: the simulation world's entity database, as
   src/unknown_0a58d0.cpp (an entity's object) and src/unknown_0aa4d0.cpp
   (an entity's relevance to the observers) see it */

#ifndef SIMULATION_ENTITY_DATABASE_H
#define SIMULATION_ENTITY_DATABASE_H

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

#endif
