#include "cseries.h"
#include "globals.h"

// @flags /O2 /Gr

/* An object's type, read from its definition (the word at +0x290 of the
   definition tag). Decompiled by lane O because the engine at 0x459d18
   (unknown_2420a0.cpp) calls it with a register argument. */

struct s_typed_object
{
	long definition_index;
};

struct s_typed_object_header
{
	byte unknown00[8];
	s_typed_object *object;
};

struct s_typed_definition
{
	byte unknown00[0x290];
	short type;
};

// @retail 0x101280
short function_101280(long object_index)
{
	s_typed_object *object = ((s_typed_object_header *)g_4e0300->data)[object_index & 0xffff].object;

	return ((s_typed_definition *)g_4e3b44[object->definition_index & 0xffff].bytes)->type;
}
