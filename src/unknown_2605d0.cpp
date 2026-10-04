// @flags /O2 /Gr
/* UNKNOWN_2605D0.CPP: choosing a reference for an actor to follow */

#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"
#include "unknown_2605d0.h"

void __stdcall function_261510(long actor_index, s_2605d0_request const *request);
s_reference __stdcall function_260670(long actor_index, s_2605d0_request const *request, s_261d20_entry *entries, short count, long unknown, long unknown2, byte *scratch, bool *unknown3);

// @retail 0x2605d0
s_reference function_2605d0(long actor_index, s_2605d0_request const *request, long unknown, long unknown2, byte *scratch, bool *unknown3)
{
	s_actor_view *actor = actor_get(actor_index);
	s_261d20_entry entries[0x200];

	if (actor->unknown007)
		return g_470fa0;

	function_261510(actor_index, request);
	if (actor->unknown220)
		actor->unknown3f2 = true;

	short count = function_261d20(actor_index, entries, 0x200, request);
	return function_260670(actor_index, request, entries, count, unknown, unknown2, scratch, unknown3);
}
