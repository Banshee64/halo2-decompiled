// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_265CB0.CPP: the props an actor knows (0x265cb0..) */

#include "cseries.h"
#include "globals.h"
#include "slot_handler.h"

real __stdcall function_265d30(long actor_index, long prop_index);

// @retail 0x265cb0
void function_265cb0(long actor_index)
{
	long prop_index = actor_get(actor_index)->first_prop_index;

	for (;;)
	{
		s_prop_node_view *node;
		long current_index;
		s_prop_view_fields *view;

		if (prop_index == NONE)
		{
			break;
		}
		node = prop_node_get(prop_index);
		current_index = prop_index;
		prop_index = node->next_index;
		view = prop_node_view(node);
		if (view)
		{
			view->unknown3c = function_265d30(actor_index, current_index);
		}
	}
}
