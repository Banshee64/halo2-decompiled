// @flags /O2 /Ob1 /arch:SSE /Gr
#include "cseries.h"
#include "unknown_26b230.h"
#include "globals.h"
#include "slot_handler.h"
#include "actor_moving.h"
#include "unknown_2626b0.h"
#include <math.h>

#define OWNER_STATE(index) ((s_slot_owner_entry *)(g_4f55f0->data + ((index) & 0xffff) * sizeof(s_slot_owner_entry)))

// @retail 0x1f8640
byte function_1f8640(long index)
{
	return OWNER_STATE(index)->unknown50c;
}

// @retail 0x1f8660
bool function_1f8660(long index)
{
	s_slot_owner_entry *s = OWNER_STATE(index);
	byte flag = s->unknown50c;
	bool result = false;
	if (flag && s->unknown504 == 1)
		result = true;
	return result;
}

// @retail 0x1f86a0
void function_1f86a0(long index)
{
	s_slot_owner_entry *s = OWNER_STATE(index);
	s->unknown50c = 0;
	s->unknown5ac = NONE;
	s->unknown5b0 = NONE;
	s->unknown5b4 = 0;
	s->unknown5b6 = 0;
	s->unknown4ac = 0;
	s->unknown504 = 0;
}

// @retail 0x1f86f0
bool function_1f86f0(long index)
{
	return OWNER_STATE(index)->unknown504 == 2;
}

// @retail 0x1f8720
bool function_1f8720(long index)
{
	return OWNER_STATE(index)->unknown504 == 3;
}

// @retail 0x1f8750
void function_1f8750(long index)
{
	s_slot_owner_entry *s = OWNER_STATE(index);
	s->unknown508++;
	s->unknown50c = 0;
	s->unknown504 = 3;
}

/* a request the movement code passes to the actor's unit (function_e6900) */
struct s_moving_unit_request
{
	long type;
	union
	{
		struct
		{
			real_point3d point;
			real_vector3d facing;
		} face;
		struct
		{
			short unknown4;
			byte unknown6[2];
			real_point3d point;
			real_vector3d vector;
		} turn;
	};
};

bool function_262590(long actor_index, s_reference reference, bool unknown);
long function_1e4990(long index);

/* the actor has arrived */
// @retail 0x1f8780
void function_1f8780(long actor_index, bool unknown)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	long type = 0;

	actor->unknown50c = true;
	actor->unknown504 = 2;
	if (!actor->unknown227)
	{
		short goal = actor->unknown4ac;

		if ((goal == 4 || goal == 6 || goal == 5) &&
			function_262590(actor_index, actor->unknown4b8_reference, false))
		{
			real radius = function_1e3920(actor_index);

			if (function_210ac0((s_node_point *)function_262b40(actor->unknown4b8_reference), &actor->position) <= radius + 0.2)
				actor->unknown227 = true;
		}
		else if (actor->unknown4e8)
		{
			actor->unknown227 = true;
		}
	}

	if (unknown)
	{
		s_moving_object_header *header = &((s_moving_object_header *)g_4e0300->data)[actor->unit_index & 0xffff];

		if (header->type == 0)
			type = header->object[0x3dc];
		switch (type)
		{
		case 2:
		{
			s_moving_unit_request request;
			bool success = false;

			if (actor->unknown4ac == 6)
			{
				request.type = 0x2d;
				request.turn.unknown4 = 0;
				success = function_262a90(actor->unknown4b8_reference, &request.turn.point, &request.turn.vector);
			}
			else if (actor->unknown4ac == 4)
			{
				long character = function_1e4990(actor->tag_index);

				if (!character || (*(byte *)character & 2) ||
					!(function_262b40(actor->unknown4b8_reference)->flags & 0x40))
				{
					break;
				}
				request.type = 0x2f;
				success = function_262af0(actor->unknown4b8_reference, &request.face.point, &request.face.facing);
				if (success)
					request.face.facing = actor->unknown290;
			}
			else
			{
				break;
			}

			if (success && function_e6900(actor->unit_index, (s_unit_request *)&request))
			{
				actor->unknown227 = true;
			}
			else
			{
				actor->unknown50c = false;
				actor->unknown504 = 3;
			}
			break;
		}
		}
	}
}
/* the length of the rest of the actor's path */
// @retail 0x1f8940
real function_1f8940(long actor_index)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	real length = 0.0f;

	if (actor->unknown50c && actor->unknown504 == 1)
	{
		real_point3d previous = actor->position;
		short i;

		for (i = actor->path_index; i < actor->path_count; i++)
		{
			s_actor_path_point *point = &actor->path[i];
			real_point3d position;

			if (point->node.output_index == NONE ||
				!function_2104b0(point->node.output_index, &point->node.point, &position))
			{
				position = point->node.point;
			}
			length += distance3d(&position, &previous);
			previous = position;
		}
	}

	return length;
}