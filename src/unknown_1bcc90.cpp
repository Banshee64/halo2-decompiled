// @flags /O2 /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x6b, and the evaluate callback many handlers share */

/* globals of the slot handlers (slot_handler.h) */
short g_46fbe4 = -1;
short g_46fbe8 = -2;
s_reference g_470fa0 = {NONE, NONE};
long g_46f348 = NONE;
dword g_4ee4ec;
dword g_557c40[5];
s_data_array *g_502424;
s_data_array *g_51e9d8;

// @retail 0x1bced0
short __stdcall function_1bced0(long actor_index, s_slot *slot, bool active)
{
	return g_46fbe8;
}
