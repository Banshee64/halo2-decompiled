// @flags /O2 /Gr /arch:SSE
/* UNKNOWN_118E80.CPP: an object's forward vector in the world (lane I's
   outside function; the command scripts' 0x25a130 calls it). The position's
   counterpart is 0xb9dd0 (unknown_0b58c0.cpp). */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"

/* the object as this reads it */
struct s_forward_object
{
	byte unknown00[0x14];
	long parent_index;
	char parent_node;
	byte unknown19[0x70 - 0x19];
	real_vector3d forward;
	byte unknown7c[0x116 - 0x7c];
	short nodes_offset;
};

struct s_forward_object_header
{
	byte unknown0[8];
	s_forward_object *object;
};

/* the object's forward vector, turned by its parent's node when it has a
   parent */
// @retail 0x118e80
void function_118e80(long object_index, real_vector3d *forward)
{
	s_forward_object_header *headers = (s_forward_object_header *)g_4e0300->data;
	s_forward_object *object = headers[object_index & 0xffff].object;

	if (object->parent_index == NONE)
	{
		if (forward)
		{
			*forward = object->forward;
		}
		return;
	}

	s_forward_object *parent = headers[object->parent_index & 0xffff].object;
	real_matrix4x3 *matrix = (real_matrix4x3 *)((byte *)parent + parent->nodes_offset + object->parent_node * 0x34);

	if (forward)
	{
		real i = object->forward.i;
		real j = object->forward.j;
		real k = object->forward.k;

		forward->i = matrix->rotation.up.i * k + matrix->rotation.left.i * j + matrix->rotation.forward.i * i;
		forward->j = matrix->rotation.up.j * k + matrix->rotation.left.j * j + matrix->rotation.forward.j * i;
		forward->k = matrix->rotation.up.k * k + matrix->rotation.left.k * j + matrix->rotation.forward.k * i;
	}
}
