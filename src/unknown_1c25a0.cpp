// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_1C25A0.CPP: the physics (Havok) system's lifecycle callbacks
   (0x441624..0x441638 in the lifecycle table) */

#include "unknown_11c920.h"
#include "unknown_123b30.h"
#include "data_array.h"
#include "globals.h"
#include "unknown_1cec30.h"
#include "object_iterator.h"
#include "havok_reference.h"
#include <xtl.h>
#include <stdio.h>
#include <stdarg.h>

/* the havok components (unknown_1cec30.cpp) */
void havok_components_initialize(void);

void game_state_initialize_1edbc0(void);
void function_2263c0(void);
void function_146b30(void);
void function_146b80(void);
void function_146de0(void);
void function_226440(void);

/* an aligned block of physical memory: the offset back to the allocation
   sits before it */
void *g_479888;

// @retail 0x1c25a0
void function_1c25a0(void)
{
	g_51e9a0 = (long *)function_123d40("havok", "havok", sizeof(long));
	*g_51e9a0 = 0;
	game_state_initialize_1edbc0();
	function_2263c0();
	havok_components_initialize();
	function_146b30();
	function_146b80();
}

// @retail 0x1c2600
void function_1c2600(void)
{
	byte *block;

	function_146de0();
	block = (byte *)g_479888;
	if (!VirtualFree(block - ((long *)block)[-1], 0, MEM_RELEASE))
	{
		GetLastError();
	}
	g_479888 = NULL;
	data_dispose(g_51e9b8);
	g_51e9b8 = NULL;
	function_226440();
}
/* the havok components (0xa0 bytes; unknown_1cec30.cpp) as the physics
   update sees them: a list of contacts at +0x70, each 0x60 bytes */
struct s_havok_contact_state
{
	byte unknown00[0xa8];
	long time;
	word flags;
};

struct s_havok_contact_owner
{
	byte unknown00[0x44];
	s_havok_contact_state *state;
};

struct s_havok_contact
{
	byte unknown00[0x40];
	s_havok_contact_owner *owner;
	byte unknown44[0x60 - 0x44];
};

/* an hkArray of contacts */
struct s_havok_contact_array
{
	s_havok_contact *data;
	long size;
	dword capacity_and_flags;

	s_havok_contact &operator[](long index)
	{
		return data[index];
	}
};

struct s_havok_component_contacts
{
	byte unknown00[0x70];
	s_havok_contact_array contacts;
	byte unknown7c[0xa0 - 0x7c];
};

inline s_havok_component_contacts *havok_component_contacts_get(long component_index)
{
	return &((s_havok_component_contacts *)g_51e9b8->data)[component_index & 0xffff];
}

// @retail 0x1c3930
bool havok_object_type_can_have_component(long definition_index)
{
	byte type = *g_4e3b44[definition_index & 0xffff].bytes;
	bool result = true;

	if ((1 << type) & 0x1883)
	{
		if (!g_47f058)
		{
			result = *g_51e9a0 < 0x200;
		}
		else
		{
			result = g_51e9b8->actual_count < g_51e9b8->maximum_count;
		}
	}
	return result;
}

// @retail 0x1c3980
void havok_object_count(long object_index)
{
	s_havok_object *object = havok_object_get(object_index);

	if (!TEST_FIELD_BIT(object->havok_flag))
	{
		object->havok_flag = 1;
		(*g_51e9a0)++;
	}
}

// @retail 0x1c4fc0
void havok_component_contacts_mark1(long component_index)
{
	if (component_index != NONE)
	{
		s_havok_component_contacts *component = havok_component_contacts_get(component_index);
		long i;

		for (i = 0; i < component->contacts.size; i++)
		{
			s_havok_contact *contact = &component->contacts[i];
			s_havok_contact_state *state = contact->owner->state;

			if (state)
			{
				if (state->time != g_510c54->game_time)
				{
					state->flags = 0;
				}
				state->flags |= 1;
				state->time = g_510c54->game_time;
			}
		}
	}
}

// @retail 0x1c5040
void havok_component_contacts_mark2(long component_index)
{
	if (component_index != NONE)
	{
		s_havok_component_contacts *component = havok_component_contacts_get(component_index);
		long i;

		for (i = 0; i < component->contacts.size; i++)
		{
			s_havok_contact *contact = &component->contacts[i];
			s_havok_contact_state *state = contact->owner->state;

			if (state)
			{
				if (state->time != g_510c54->game_time)
				{
					state->flags = 0;
				}
				state->flags |= 2;
				state->time = g_510c54->game_time;
			}
		}
	}
}
#define PIN(value, lower, upper) ((value) < (lower) ? (lower) : (value) > (upper) ? (upper) : (value))

/* the game time of the last ... (NONE when unset) */
long g_47f054 = NONE;

// @retail 0x1c58a0
bool function_1c58a0(void)
{
	long time = g_47f054;
	bool result = false;

	if (time != NONE)
	{
		long game_time = g_510c54->game_time;

		long lower = game_time - 3;

		result = (time < lower ? lower : game_time < time ? game_time : time) == time;
	}
	return result;
}

/* a Havok collision body: its shape, the shape key in its parent, and the
   parent body */
struct s_havok_shape_view
{
	byte unknown00[8];
	dword user_data;
};

struct s_havok_cd_body
{
	s_havok_shape_view *shape;
	long shape_key;
	byte unknown08[4];
	s_havok_cd_body *parent;
};

// @retail 0x1c55b0
long havok_cd_body_shape_key_get(s_havok_cd_body const *body)
{
	long result = NONE;

	while (body->parent)
	{
		if (body->parent->shape->user_data == 0xcabcabb0)
		{
			break;
		}
		body = body->parent;
	}
	if (body->parent)
	{
		result = body->shape_key;
	}
	return result;
}

// @retail 0x1c4560
void havok_printf(char const *format, ...)
{
	char buffer[0x104];
	va_list arguments;

	va_start(arguments, format);
	_vsnprintf(buffer, 0xfe, format, arguments);
}
void havok_component_delete(long component_index);

// @retail 0x1c37f0
void havok_object_detach(long object_index)
{
	s_havok_object *object = havok_object_get(object_index);

	if (object->havok_component_index != NONE)
	{
		havok_component_delete(object->havok_component_index);
		object->havok_component_index = NONE;
	}
	object = havok_object_get(object_index);
	if (TEST_FIELD_BIT(object->havok_flag))
	{
		object->havok_flag = 0;
		(*g_51e9a0)--;
	}
}
/* the physics world and its counters */
hkWorld *g_51e9a4;
long g_47f050;

struct s_47f048_object;
extern s_47f048_object *g_47f048;
extern void *g_51ecac;

void function_278f00(void);
void function_146bf0(void);
long function_baf80(long object_index);
void function_0bfe40(word *flags, long bit, bool value);
long havok_component_new(long object_index);
void function_1cf120(long component_index);
struct s_havok_component;
void function_1d1260(s_havok_component *component);
void __stdcall function_1d01c0(s_havok_component *component);
void function_1d1540(s_havok_component *component);

/* the object headers as the physics code reads them */
struct s_physics_object_header
{
	short identifier;
	byte flags;
	byte type;
	short cluster_index;
	byte unknown06[2];
	struct s_physics_object *object;
};

struct s_physics_object
{
	long tag_index;
	byte unknown004[0xc - 0x4];
	long next_object_index;
	long first_child_index;
	byte unknown014[0xb4 - 0x14];
	long havok_component_index;
	byte unknown0b8[0xc0 - 0xb8];
	union
	{
		word flags;
		struct
		{
			word unknownc0_0 : 6;
			word bit6 : 1;
			word unknownc0_7 : 5;
			word bit12 : 1;
			word unknownc0_13 : 3;
		};
	};
	byte unknown0c2[0x11a - 0xc2];
	short region_variants_offset;
};

/* the havok components (unknown_1cec30.cpp) as the physics code reads them */
struct s_havok_component_flags
{
	short identifier;
	byte unknown02[2];
	union
	{
		dword flags;
		struct
		{
			dword unknown0 : 5;
			dword bit5 : 1;
			dword unknown6 : 5;
			dword bit11 : 1;
			dword unknown12 : 2;
			dword bit14 : 1;
			dword unknown15 : 2;
			dword bit17 : 1;
			dword unknown18 : 2;
			dword bit20 : 1;
			dword unknown21 : 11;
		};
	};
	long object_index;
	byte unknown0c[0xa0 - 0xc];
};

/* the model's regions (the object definition's model, +0x70) */
struct s_model_permutation_view
{
	byte unknown0[6];
	char region;
	byte unknown7;
};

struct s_model_region_view
{
	byte unknown00[0xc];
	s_model_permutation_view *permutations;
};

struct s_model_view
{
	byte unknown00[0x70];
	long region_count;
	s_model_region_view *regions;
};

struct s_object_definition_view
{
	byte unknown00[0x38];
	long model_tag_index;
};

PRIVATE inline s_physics_object_header *physics_object_header_get(long object_index)
{
	return &((s_physics_object_header *)g_4e0300->data)[object_index & 0xffff];
}

PRIVATE inline s_physics_object *physics_object_get(long object_index)
{
	return physics_object_header_get(object_index)->object;
}

PRIVATE inline s_havok_component_flags *havok_component_flags_get(long component_index)
{
	return &((s_havok_component_flags *)g_51e9b8->data)[component_index & 0xffff];
}

/* an object 0x1c2910 makes */
c_havok_reference_counted *g_4f55b0;

/* drops the references to 0x1c2910's object and to g_47f048 */
// @retail 0x1c2890
void function_1c2890(void)
{
	if (g_4f55b0)
	{
		havok_reference_remove(g_4f55b0);
		g_4f55b0 = NULL;
	}
	havok_reference_remove((c_havok_reference_counted *)g_47f048);
	g_47f048 = NULL;
}

// @retail 0x1c29d0
void function_1c29d0(void)
{
	((hkEntityApi *)g_47f048)->removeEntityListener((hkEntityListener *)g_51ecac);
	g_47f050--;
	g_51e9a4->removeEntity((hkEntity *)g_47f048);
	function_278f00();
}

// @retail 0x1c35f0
void __stdcall function_1c35f0(long object_index)
{
	s_physics_object_header *header = physics_object_header_get(object_index);

	if (((1 << header->type) & 0x1883) && !(header->flags & 0x10))
	{
		long root_index = function_baf80(object_index);

		header->object->bit12 = false;
		if (physics_object_header_get(root_index)->cluster_index != NONE)
		{
			s_physics_object *object = header->object;

			if (g_47f058)
			{
				s_havok_component_flags *component;

				object->havok_component_index = havok_component_new(object_index);
				function_1cf120(object->havok_component_index);
				component = havok_component_flags_get(object->havok_component_index);
				if (TEST_FIELD_BIT(object->bit6))
				{
					function_1d1540((s_havok_component *)component);
				}
				function_0bfe40(&header->object->flags, 0xc,
					TEST_FIELD_BIT(component->bit14) || TEST_FIELD_BIT(component->bit20));
			}
			havok_object_count(object_index);
		}
	}
}

// @retail 0x1c36f0
void __stdcall function_1c36f0(long parent_index, long object_index)
{
	s_physics_object *object = physics_object_get(object_index);
	long child_index = object->first_child_index;

	if (object->havok_component_index == NONE)
	{
		function_146bf0();
		function_1c35f0(object_index);
		function_278f00();
		function_146bf0();
	}
	while (child_index != NONE)
	{
		s_physics_object *child = physics_object_get(child_index);

		if (child_index == parent_index)
		{
			break;
		}
		function_1c36f0(parent_index, child_index);
		child_index = child->next_object_index;
	}
}

/* sets the flags of the object's havok component and rebuilds it, with the
   component's active state handled around the change.
   Standard convention (see docs/DECOMPILING.md):
   1. Retail keeps it __stdcall (both arguments on the stack, ret 8). With the
      marker this body matches byte for byte; without it LTCG passes the
      object index in eax.
   2. No data or code in retail holds its address. Its twelve callers
      (0xdbc80 0xe0c70 0xe0ef0 0xe16b0 0xe18d0 0xe24f0 0x1c38a0 0x1c4040
      0x1d2460 0x1e54d0 0x1ec690 0x278f00) all push both arguments; 0x1d2460
      itself takes its component in ecx, a register convention.
   3. Tried: declaring it __stdcall alone does nothing under LTCG; /GL- on
      the file would break the register convention of its callee 0x1cf120,
      which takes the component index in eax. */
// @retail 0x1c3770 standard
void __stdcall function_1c3770(long object_index, dword flags)
{
	s_physics_object *object = physics_object_get(object_index);

	if (object->havok_component_index != NONE)
	{
		s_havok_component_flags *component = havok_component_flags_get(object->havok_component_index);
		bool active = TEST_FIELD_BIT(component->bit5);

		if (active)
		{
			function_1d1260((s_havok_component *)component);
		}
		function_1d01c0((s_havok_component *)component);
		component->flags = flags;
		function_1cf120(object->havok_component_index);
		if (active)
		{
			function_1d1540((s_havok_component *)component);
		}
	}
}

// @retail 0x1c3850
void function_1c3850(long object_index)
{
	s_physics_object *object = physics_object_get(object_index);

	if (object->havok_component_index != NONE &&
		!TEST_FIELD_BIT(havok_component_flags_get(object->havok_component_index)->bit17))
	{
		function_1c3770(object_index, 0);
	}
}

PRIVATE inline char model_permutation_region_get(s_model_region_view const *region, char permutation)
{
	return permutation == NONE ? NONE : region->permutations[permutation].region;
}

// @retail 0x1c54b0
void function_1c54b0(long object_index, char const *variants)
{
	s_physics_object *object = physics_object_get(object_index);

	if (object->havok_component_index != NONE &&
		TEST_FIELD_BIT(havok_component_flags_get(object->havok_component_index)->bit11))
	{
		s_object_definition_view *definition = (s_object_definition_view *)g_4e3b44[object->tag_index & 0xffff].bytes;
		s_model_view *model = (s_model_view *)g_4e3b44[definition->model_tag_index & 0xffff].bytes;
		char const *current = (char const *)object + object->region_variants_offset;
		long i;

		for (i = 0; i < model->region_count; i++)
		{
			if (variants[i] != current[i] &&
				model_permutation_region_get(&model->regions[i], variants[i]) != model_permutation_region_get(&model->regions[i], current[i]))
			{
				function_1c3770(object_index, 0);
				break;
			}
		}
	}
}


// @retail 0x1c50c0
void function_1c50c0(void)
{
	long island_index;

	for (island_index = 0; island_index < g_51e9a4->m_island_count; )
	{
		hkSimulationIsland *island = g_51e9a4->m_islands[island_index];
		long entity_count = island->m_entity_count;
		long count = 0;
		bool removed = false;
		long i;

		for (i = 0; i < entity_count; i++)
		{
			long component_index = havok_entity_property_get(island->m_entities[i], HAVOK_PROPERTY_COMPONENT_INDEX);

			if (component_index != NONE &&
				(physics_object_get(havok_component_flags_get(component_index)->object_index)->flags & 0x100))
			{
				count++;
			}
		}
		if (count == entity_count)
		{
			g_51e9a4->removeSimulationIsland(island);
			removed = true;
		}
		if (!removed)
		{
			island_index++;
		}
	}
}

// @retail 0x1c2a10
void function_1c2a10(void)
{
	s_type_f1af8e iterator;

	function_bae80(&iterator, 0, 0);
	while (function_baeb0(&iterator))
	{
		physics_object_get(iterator.object_index)->havok_component_index = NONE;
	}
	function_bae80(&iterator, 0, 0);
	while (function_baeb0(&iterator))
	{
		long object_index = iterator.object_index;

		if (physics_object_get(object_index)->havok_component_index == NONE)
		{
			function_146bf0();
			function_1c35f0(object_index);
			function_278f00();
			function_146bf0();
		}
	}
	function_1c50c0();
	g_47f054 = g_510c54->game_time;
}

bool function_1c4040(long attempt, bool active, bool any_object, bool even_if_unknown, long excluded_component_index);

// @retail 0x1c51c0
void function_1c51c0(long component_index)
{
	bool force = false;
	long attempt = 0;

	while (g_47f050 > 0x34e)
	{
		if (!function_1c4040(attempt, force, false, true, component_index))
		{
			if (attempt < 2)
			{
				attempt++;
			}
			else if (!force)
			{
				force = true;
				attempt = 0;
			}
		}
	}
}

/* the object as function_1c3f30 reads it */
struct s_physics_object_flags_view
{
	byte unknown000[4];
	dword unknown004_0 : 14;
	dword bit14 : 1;
	dword unknown004_15 : 17;
	byte unknown008[0x13c - 0x8];
	long unknown13c;
};

/* what function_1c4040 reads at +0xb4 of g_4e034c */
struct s_physics_effect_globals
{
	byte unknown0[4];
	long effect_index;
};

struct s_tag_header_globals_physics_view
{
	byte unknown00[0xb4];
	s_physics_effect_globals *effect;
};

/* an hkArray of simulation islands */
struct s_simulation_island_array
{
	hkSimulationIsland **data;
	long count;
	dword capacity_and_flags;
};

struct s_physics_world_view
{
	byte unknown00[8];
	s_simulation_island_array active_islands;
	s_simulation_island_array inactive_islands;
	byte unknown20[0x2c - 0x20];
	hkSimulationIsland *fixed_island;
};

struct s_physics_object_detach_view
{
	byte unknown000[0x30];
	point3f position;
	byte unknown03c[0x70 - 0x3c];
	vector3f velocity;
	byte unknown07c[0xd4 - 0x7c];
	long unknownd4;
};

struct s_physics_entity_view
{
	byte unknown00[0x8c];
	long priority;
};

bool function_a7670(long object_index);
#include "unknown_1765e0.h"
void __stdcall function_b8540(long object_index);
void havok_object_detach(long object_index);

// @retail 0x1c3f30
long function_1c3f30(hkEntity *entity, long attempt, bool any_object, bool even_if_unknown, long excluded_component_index)
{
	long result = NONE;

	if (havok_entity_property_get(entity, HAVOK_PROPERTY_COMPONENT_INDEX) != NONE)
	{
		long component_index = havok_entity_property_get(entity, HAVOK_PROPERTY_COMPONENT_INDEX);

		if (component_index != excluded_component_index)
		{
			long object_index = havok_component_flags_get(component_index)->object_index;

			if (!function_a7670(object_index) || even_if_unknown)
			{
				s_physics_object_header *header = physics_object_header_get(object_index);
				bool unknown = TEST_FIELD_BIT(((s_physics_object_flags_view *)header->object)->bit14);

				if (any_object || (header->flags & 1))
				{
					if (unknown)
					{
						result = object_index;
					}
					else if ((1 << header->type) & 3)
					{
						if (attempt >= 2 && ((s_physics_object_flags_view *)header->object)->unknown13c == NONE)
						{
							return object_index;
						}
					}
					else if (((1 << header->type) & 0x800) && attempt >= 1)
					{
						result = object_index;
					}
				}
			}
		}
	}
	return result;
}

// @retail 0x1c4040
bool function_1c4040(long attempt, bool active, bool any_object, bool even_if_unknown, long excluded_component_index)
{
	s_physics_world_view *world = (s_physics_world_view *)g_51e9a4;
	s_simulation_island_array *islands = active ? &world->active_islands : &world->inactive_islands;
	long best_priority = 0x80000000;
	long best_object_index = NONE;
	bool result = false;
	long island_index;
	long i;

	if (!active)
	{
		hkSimulationIsland *island = world->fixed_island;

		for (i = 0; i < island->m_entity_count; i++)
		{
			hkEntity *entity = island->m_entities[i];
			long priority = ((s_physics_entity_view *)entity)->priority;

			if (priority > best_priority)
			{
				long object_index = function_1c3f30(entity, attempt, any_object, even_if_unknown, excluded_component_index);

				if (object_index != NONE)
				{
					best_priority = priority;
					best_object_index = object_index;
				}
			}
		}
	}
	for (island_index = 0; island_index < islands->count; island_index++)
	{
		hkSimulationIsland *island = islands->data[island_index];

		for (i = 0; i < island->m_entity_count; i++)
		{
			hkEntity *entity = island->m_entities[i];
			long priority = ((s_physics_entity_view *)entity)->priority;

			if (priority > best_priority)
			{
				long object_index = function_1c3f30(entity, attempt, any_object, even_if_unknown, excluded_component_index);

				if (object_index != NONE)
				{
					best_priority = priority;
					best_object_index = object_index;
				}
			}
		}
	}
	if (best_object_index != NONE)
	{
		s_physics_effect_globals *effect = ((s_tag_header_globals_physics_view *)g_4e034c)->effect;
		s_physics_object *object;
		bool unknown;

		if (g_4e6948->mode == 4 && ((s_physics_object_detach_view *)physics_object_get(best_object_index))->unknownd4 != NONE)
		{
			physics_object_get(best_object_index)->flags |= 0x20;
			function_1c3770(best_object_index, 0);
			return true;
		}
		object = physics_object_get(best_object_index);
		if (effect->effect_index != NONE)
		{
			function_1765e0(&((s_physics_object_detach_view *)object)->position, &((s_physics_object_detach_view *)object)->velocity,
				g_4687b0, effect->effect_index, 0, true);
		}
		object = physics_object_get(best_object_index);
		if (object->havok_component_index != NONE)
		{
			function_1d1260((s_havok_component *)havok_component_flags_get(object->havok_component_index));
		}
		object->bit6 = false;
		unknown = TEST_FIELD_BIT(physics_object_get(best_object_index)->bit6);
		if (unknown)
		{
			function_146bf0();
		}
		havok_object_detach(best_object_index);
		if (unknown)
		{
			function_278f00();
			function_146bf0();
		}
		function_b8540(best_object_index);
		return true;
	}
	return result;
}
