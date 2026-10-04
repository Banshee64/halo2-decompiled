// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* the last callback of slot handler 0x50 (g_47eeb8, update48); the rest of
   that handler's callbacks are at 0x1bf990..0x1bff80 (lane B's region) */

struct s_slot_50
{
	s_slot_header header;
	byte unknown0c[0x1b - 0xc];
	bool unknown1b;
	point3f unknown1c;
	byte unknown28[0x40 - 0x28];
};

// @retail 0x1c0230
void __stdcall function_1c0230(long actor_index, s_slot *slot)
{
	s_slot_50 *state = (s_slot_50 *)slot;

	if (state->unknown1b)
	{
		s_actor_view *actor = actor_get(actor_index);

		actor->unknown41c = 4;
		actor->unknown420 = 4;
		actor->unknown424.point = state->unknown1c;
	}
}
