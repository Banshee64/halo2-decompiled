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
