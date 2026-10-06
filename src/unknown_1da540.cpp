// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1DA540.CPP: the damage of havok collisions (0x1d96d0..) */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1cec30.h"

/* an entry of the 16 collisions function_1d96d0 gathers on its stack (0x94
   bytes); it constructs them with the vector constructor iterator */
struct s_collision_damage_entry
{
	long definition_index;
	dword flags;
	byte unknown08[0x1c - 8];
	long object_value28;
	long object_value2c;
	point3f position;
	point3f origin;
	vector3f direction;
	byte unknown48[0x7c - 0x48];
	short unknown7c;
	byte unknown7e[0x84 - 0x7e];
	byte kind;
	byte unknown85[3];
	long instance_index;
	long material_index;
	long surface_index;

	s_collision_damage_entry();
};

// @retail 0x1da540
s_collision_damage_entry::s_collision_damage_entry()
{
	unknown7c = NONE;
}


struct s_collision_damage_object
{
	byte unknown00[0x28];
	long value28;
	long value2c;
	byte unknown30[0xb4 - 0x30];
	long component_index;
};

struct s_collision_damage_surface
{
	byte unknown00[5];
	byte material_index;
	byte unknown06[2];
};

struct s_collision_damage_mesh
{
	byte unknown00[0x2c];
	s_collision_damage_surface *surfaces;
};

struct s_collision_damage_geometry
{
	byte unknown00[0x9c];
	s_collision_damage_surface *surfaces;
	byte unknowna0[0xc8 - 0xa0];
};

struct s_collision_damage_instance
{
	byte unknown00[0x34];
	short geometry_index;
	byte unknown36[0x58 - 0x36];
};

struct s_collision_damage_structure
{
	byte unknown00[0x13c];
	s_collision_damage_geometry *geometry;
	long unknown140;
	s_collision_damage_instance *instances;
};

struct s_collision_damage_definition
{
	long unknown00;
	long damage_index;
};

struct s_collision_damage_globals
{
	byte unknown00[0xbc];
	s_collision_damage_definition *definition;
};

struct s_slot_entry_list;
extern s_slot_entry_list *g_4e0340;
struct s_type_1e6529;
void function_d6660(s_type_1e6529 *data, long definition_index);
real function_1201a0(vector3f *vector, vector3f const *fallback);

struct s_collision_damage_location
{
	long instance;
	long material;
	long surface;
};

PRIVATE __forceinline s_havok_component_element48 *collision_damage_contact_get(s_havok_component *component, long index)
{
	return &component->unknown88.data[index];
}

PRIVATE __forceinline void collision_damage_location(dword key, long *instance, long *material, long *surface)
{
	long absolute_index = key & 0xffff;
	long part = (key >> 16) & 0x1fff;
	*instance = NONE;
	switch (key >> 29)
	{
	case 1:
		*material = ((s_collision_damage_mesh *)g_4e0340)->surfaces[absolute_index].material_index;
		*surface = absolute_index;
		break;
	default:
		s_collision_damage_structure *structure = (s_collision_damage_structure *)g_4e0348;
		*instance = absolute_index;
		*material = structure->geometry[structure->instances[absolute_index].geometry_index].surfaces[part].material_index;
		*surface = part;
		break;
	}
}

// @retail 0x1da550
bool function_1da550(long object_index, long contact_index, s_collision_damage_entry *entry)
{
	s_collision_damage_object *object = (s_collision_damage_object *)havok_object_get(object_index);
	s_havok_component *component = havok_component_get(object->component_index);
	s_havok_component_element48 *contact = collision_damage_contact_get(component, contact_index);
	bool result = false;
	if (contact->impulse > 0.4f)
	{
		s_collision_damage_location location;
		collision_damage_location(*(dword *)&contact->unknown00[4], &location.instance, &location.material, &location.surface);
		function_d6660((s_type_1e6529 *)entry, ((s_collision_damage_globals *)g_4e034c)->definition->damage_index);
		entry->origin = contact->position;
		entry->position = contact->position;
		entry->direction = contact->normal;
		function_1201a0(&entry->direction, g_4687a8);
		entry->kind = 2;
		entry->unknown7c = contact->material_b;
		entry->object_value28 = object->value28;
		entry->object_value2c = object->value2c;
		entry->flags |= 0x100;
		entry->surface_index = location.surface;
		entry->instance_index = location.instance;
		entry->material_index = location.material;
		result = true;
	}
	return result;
}
