// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1E3920.CPP: the radius within which an actor counts as arrived */

#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"
#include "unknown_1e3920.h"

long function_1e4a50(long index);

/* the actor's movement block of its character tag (function_1e4a50) */
struct s_character_movement_view
{
	byte unknown0[8];
	real arrival_radius;
};

// @retail 0x1e3920
real function_1e3920(long actor_index)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	real radius = actor->unknown4cc;

	if (radius == 0.0f)
	{
		s_moving_object *unit = moving_object_get(actor->unit_index);

		if (actor->unknown26c != NONE)
		{
			s_tag_element *element = function_1e5450(actor_index, moving_object_get(actor->unknown26c)->tag_index);

			if (element)
				radius = *(real *)((byte *)element + 0x18);
			if (radius <= 0.0f)
				radius = 0.5f;
		}
		else
		{
			s_tag_element *element;

			if (actor->unknown229 && (element = function_1e5450(actor_index, unit->tag_index)) != 0)
			{
				radius = *(real *)((byte *)element + 0x18);
			}
			else
			{
				s_character_movement_view *movement = (s_character_movement_view *)function_1e4a50(actor->tag_index);

				if (movement)
					radius = movement->arrival_radius;
			}
			if (radius <= 0.0f)
				radius = 0.3f;
		}
	}

	if (radius > 0.2f)
		return radius;
	return 0.2f;
}
