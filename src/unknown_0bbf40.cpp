// @flags /O2 /Ob1 /Gr
/* UNKNOWN_0BBF40.CPP: object flag setters of the script functions */

#include "cseries.h"
#include "globals.h"
#include "unknown_0bbf40.h"

#define FLAG(bit) (1 << (bit))
#define SET_FLAG(flags, bit, value) ((value) ? ((flags) |= FLAG(bit)) : ((flags) &= ~FLAG(bit)))

/* the objects (a local view) */
struct s_object_0bbf40
{
	byte unknown00[4];
	dword flags;
};

struct s_object_header_0bbf40
{
	byte unknown00[8];
	s_object_0bbf40 *object;
};

inline s_object_0bbf40 *object_get_0bbf40(long object_index)
{
	return ((s_object_header_0bbf40 *)g_4e0300->data)[object_index & 0xffff].object;
}

// @retail 0xbbf40
void function_bbf40(long object_index, bool flag)
{
	if (object_index != NONE)
	{
		dword *flags = &object_get_0bbf40(object_index)->flags;
		SET_FLAG(*flags, 19, flag);
	}
}

// @retail 0xbbf80
void function_bbf80(long object_index, bool flag)
{
	if (object_index != NONE)
	{
		dword *flags = &object_get_0bbf40(object_index)->flags;
		SET_FLAG(*flags, 24, flag);
	}
}

// @retail 0xbc150
void function_bc150(long object_index, bool flag)
{
	if (object_index != NONE)
	{
		dword *flags = &object_get_0bbf40(object_index)->flags;
		SET_FLAG(*flags, 21, flag);
	}
}
void function_b8b70(long object_index);

// @retail 0xbc100
void function_bc100(long object_index, bool flag)
{
	if (object_index != NONE)
	{
		dword *flags = &object_get_0bbf40(object_index)->flags;
		SET_FLAG(*flags, 20, flag);
		function_b8b70(object_index);
	}
}

struct s_predicted_resource_block;
bool function_16e5e0(s_predicted_resource_block const *block, short mode);

/* the objects' definitions (a local view) */
struct s_object_definition_0bbf40
{
	byte unknown00[0xb4];
	byte predicted_resources[0xc];
};

/* the objects (another local view) */
struct s_object_tree_0bbf40
{
	long definition_index;
	byte unknown04[0xc - 4];
	long next_object_index;
	long first_child_index;
};

struct s_object_tree_header_0bbf40
{
	byte unknown00[8];
	s_object_tree_0bbf40 *object;
};

/* requests (or releases) the predicted resources of an object definition */
// @retail 0xbbe00
bool function_bbe00(long definition_index, bool load)
{
	bool result = true;
	if (definition_index != NONE)
	{
		s_object_definition_0bbf40 *definition = (s_object_definition_0bbf40 *)g_4e3b44[definition_index & 0xffff].bytes;
		if (load)
			result = function_16e5e0((s_predicted_resource_block const *)definition->predicted_resources, 1);
		else
			result = function_16e5e0((s_predicted_resource_block const *)definition->predicted_resources, 2);
	}
	return result;
}

/* the same for an object and every object attached to it */
// @retail 0xbbec0
bool function_bbec0(long object_index, bool load)
{
	bool result = true;
	if (object_index != NONE)
	{
		s_object_tree_0bbf40 *object = ((s_object_tree_header_0bbf40 *)g_4e0300->data)[object_index & 0xffff].object;
		long child_index;

		result = function_bbe00(object->definition_index, load) & 1;
		for (child_index = object->first_child_index; child_index != NONE; child_index = object->next_object_index)
		{
			object = ((s_object_tree_header_0bbf40 *)g_4e0300->data)[child_index & 0xffff].object;
			result &= function_bbec0(child_index, load);
		}
	}
	return result;
}
