#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1428b0.h"

// @flags /O2 /Gr /arch:SSE

/* local views of the simulation world, the object header data and the
   objects (the other files keep their own views of the same data) */
struct s_world_entry
{
	byte flag0 : 1;
	byte flag1 : 1;
	byte flag2 : 1;
	byte : 5;
	byte unknown01[7];
};

struct s_world_state
{
	byte unknown00[0x44];
	s_world_entry entries[1];
};

struct s_world_slot
{
	byte unknown00[0xc];
	dword flags;
	byte unknown10[0x10];
};

struct s_world_pool
{
	byte unknown00[0xc];
	s_world_state *state;
	byte unknown10[4];
	s_world_slot slots[1];
};

struct s_world_data
{
	byte unknown00[0x2098];
	s_world_pool pool;
};

struct s_world
{
	byte unknown00[4];
	s_world_data *data;
	long state;
};

struct s_object
{
	byte unknown00[0x14];
	long parent_index;
	char parent_node;
	byte unknown19[0x64 - 0x19];
	point3f position;
	byte unknown70[0x116 - 0x70];
	short nodes_offset;
};

struct s_object_header
{
	short identifier;
	union
	{
		byte flags;
		struct
		{
			byte flag0 : 1;
			byte flag1 : 1;
			byte : 6;
		};
	};
	byte type;
	byte unknown04[4];
	s_object *object;
};

// @retail 0xb58c0
void function_b58c0(long index, dword mask)
{
	long const *index_reference = &index;
	s_world *world = (s_world *)g_4cf77c;
	long state = world->state;

	if (state == 4 || state == 5)
	{
		if (state != 3 && state != 5)
		{
			s_world_pool *pool = &world->data->pool;

			if (pool->state->entries[*index_reference & 0x3ff].flag2)
			{
				s_world_slot *entry = &pool->slots[*index_reference & 0x3ff];
				entry->flags = entry->flags | mask;
			}
		}
	}
}

// @retail 0xb7360
void function_b7360(long object_index)
{
	s_record_pool *data = g_4e0300;
	s_object_header *headers = (s_object_header *)data->data;
	s_object_header *header = &headers[object_index & 0xffff];

	if (!(header->flags & 2))
	{
		while (object_index != NONE)
		{
			header = &headers[object_index & 0xffff];
			header->flag1 = 1;
			headers = (s_object_header *)data->data;
			object_index = headers[object_index & 0xffff].object->parent_index;
		}
	}
}

// @retail 0xb9dd0
point3f *function_b9dd0(long object_index, point3f *result)
{
	s_object_header *headers = (s_object_header *)g_4e0300->data;
	s_object *object = headers[object_index & 0xffff].object;

	if (object->parent_index == NONE)
	{
		*result = object->position;
		return result;
	}

	real x = object->position.x;
	real y = object->position.y;
	real z = object->position.z;
	s_object *parent = headers[object->parent_index & 0xffff].object;
	transform4x3f *matrix = (transform4x3f *)((byte *)parent + parent->nodes_offset + object->parent_node * 0x34);

	if (matrix->scale != 1.0f)
	{
		x *= matrix->scale;
		y *= matrix->scale;
		z *= matrix->scale;
	}

	result->x = matrix->rotation.up.i * z + matrix->rotation.left.i * y + matrix->rotation.forward.i * x + matrix->position.x;
	result->y = matrix->rotation.up.j * z + matrix->rotation.left.j * y + matrix->rotation.forward.j * x + matrix->position.y;
	result->z = matrix->rotation.up.k * z + matrix->rotation.left.k * y + matrix->rotation.forward.k * x + matrix->position.z;
	return result;
}
