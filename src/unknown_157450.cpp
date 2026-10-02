// @flags /O2 /Gr
/* UNKNOWN_157450.CPP: the lifecycle callbacks of entry 22 */

#include "cseries.h"
#include "game_state.h"
#include <string.h>

class c_unknown_157450
{
public:
	virtual void slot0() = 0;
	virtual void slot1() = 0;
	virtual void slot2() = 0;
};

struct s_unknown_157450
{
	byte unknown000[0xc14];
	long index;
	byte unknownc18[0x84];
};

s_unknown_157450 *g_4e9ae8;
dword g_502258[0x27];
c_unknown_157450 *g_55e4d0[1];

// @retail 0x157450
void function_157450(void)
{
	s_unknown_157450 *data = (s_unknown_157450 *)game_state_malloc("unknown", "unknown", sizeof(s_unknown_157450));

	memset(data, 0, sizeof(*data));
	memset(g_502258, 0, sizeof(g_502258));
	g_4e9ae8 = data;
	data->index = NONE;
}

// @retail 0x157bb0
void function_157bb0(void)
{
	c_unknown_157450 *object = g_55e4d0[g_4e9ae8->index];

	if (object)
	{
		object->slot2();
		g_4e9ae8->index = NONE;
	}
}
