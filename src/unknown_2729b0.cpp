// @flags /O2 /Gr
/* UNKNOWN_2729B0.CPP: the character variant tag of an actor */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"
#include "slot_handler.h"
#include "unknown_2729b0.h"

/* g_4e0350 (the scenario) as these functions see it */
struct s_2729b0_variant
{
	long unknown0;
	long tag_index;
};

struct s_2729b0_scenario_view
{
	byte unknown000[0x150];
	long variant_count;
	s_2729b0_variant *variants;
	byte unknown158[0x244 - 0x158];
	byte *starting_locations;
};

/* the elements of g_502408 (0xd4 bytes) and g_51e9d8 (0x98 bytes) */
struct s_2729b0_squad
{
	byte unknown00[0x86];
	bool has_variant;
	byte unknown87;
	long variant_tag_index;
	byte unknown8c[0xd4 - 0x8c];
};

struct s_2729b0_swarm
{
	byte unknown00[0x2a];
	short starting_location_index;
	byte unknown2c[0x98 - 0x2c];
};

// @retail 0x2729b0
void *function_2729b0(s_2729b0_starting_location *location)
{
	void *result = 0;
	short variant_index = location->variant_index;

	if (variant_index >= 0)
	{
		s_2729b0_scenario_view *scenario = (s_2729b0_scenario_view *)g_4e0350;
		if (variant_index < scenario->variant_count)
		{
			s_2729b0_variant *variant = &scenario->variants[variant_index];
			if (variant->tag_index != NONE)
				result = g_4e3b44[variant->tag_index & 0xffff].bytes;
		}
	}

	return result;
}

// @retail 0x272a00
void *function_272a00(long actor_index)
{
	void *result = 0;
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown858 != NONE)
	{
		s_2729b0_squad *squad = (s_2729b0_squad *)(g_502408->data + (actor->unknown858 & 0xffff) * sizeof(s_2729b0_squad));
		if (squad->has_variant)
		{
			result = g_4e3b44[squad->variant_tag_index & 0xffff].bytes;
			if (result)
				return result;
		}
	}

	if (actor->unknown030 != NONE)
	{
		s_2729b0_swarm *swarm = (s_2729b0_swarm *)(g_51e9d8->data + (actor->unknown030 & 0xffff) * sizeof(s_2729b0_swarm));
		short location_index = swarm->starting_location_index;
		if (location_index != NONE)
		{
			s_2729b0_starting_location *location = (s_2729b0_starting_location *)(((s_2729b0_scenario_view *)g_4e0350)->starting_locations + location_index * 0x7c);
			if (location)
				return function_2729b0(location);
		}
	}

	return result;
}

// @retail 0x272ab0
short function_272ab0(long actor_index)
{
	short result = 0;
	s_character_variant *variant = (s_character_variant *)function_272a00(actor_index);

	if (variant)
		result = variant->default_index;

	return result;
}

// @retail 0x272ad0
short function_272ad0(s_character_variant const *variant)
{
	if (variant->block_indices[9] <= 0)
		return variant->default_index;

	return variant->block_indices[9] - 1;
}
