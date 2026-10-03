// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "real_math.h"

/* the actor's props as targets */

void *function_1e5380(long actor_index);
bool function_25d970(s_prop_datum *datum);

/* the block function_1e5380 returns */
struct s_character_variant_ranges
{
	byte unknown00[0x18];
	real minimum_distance;
	real maximum_distance;
};

/* the prop's state as these functions read it */
struct s_prop_state_1ff
{
	byte unknown00[0x3c];
	long unknown3c;
	byte unknown40[0x5e - 0x40];
	bool unknown5e;
};

/* the elements of g_50241c (0xc4 bytes) */
struct s_50241c_element_1ff
{
	byte unknown00[0x23];
	bool unknown23;
	byte unknown24[0xc4 - 0x24];
};

/* whether the point is in the range the actor's variant fights at, and the
   prop's target there */
// @retail 0x1ff6c0
bool function_1ff6c0(long actor_index, long prop_index, real_point3d const *point, long *target)
{
	s_actor_view *actor = actor_get(actor_index);
	bool result = false;
	s_character_variant_ranges *ranges = (s_character_variant_ranges *)function_1e5380(actor_index);

	if (ranges)
	{
		real distance = distance3d(&actor->position, point);

		if (distance > ranges->minimum_distance && ranges->maximum_distance > distance)
		{
			if (prop_index != NONE)
			{
				s_prop_node_view *node = prop_node_get(prop_index);
				s_prop_state_1ff *state = (s_prop_state_1ff *)prop_state_get((s_prop_datum *)node);
				s_50241c_element_1ff *element = (s_50241c_element_1ff *)(g_50241c->data + (node->unknown08 & 0xffff) * sizeof(s_50241c_element_1ff));

				if (!element->unknown23 || state->unknown5e)
					return result;
				if ((node->unknown24 < 1 || node->unknown24 > 2) && !function_25d970((s_prop_datum *)node))
					return result;
				*target = state->unknown3c;
			}
			return true;
		}
	}
	return result;
}
