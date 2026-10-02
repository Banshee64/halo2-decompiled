#include "cseries.h"

// @flags /O2 /Gr

struct path_node
{
	short heap_index;
	byte unknown02[0x42];
};

struct path_heap_entry
{
	short node;
	short cost;
};

struct path_state
{
	byte unknown000[0xf0];
	path_node nodes[1023];
	byte unknown_pad[4];
	short heap_count;
	path_heap_entry heap[1];
};

// @retail 0x271e50
PRIVATE void path_heap_bubble_up(path_state *state, short index)
{
	short cost = state->heap[index].cost;
	short node = state->heap[index].node;

	while (index > 1)
	{
		short parent = index >> 1;
		short parent_cost = state->heap[parent].cost;
		short parent_node = state->heap[parent].node;
		if (cost >= parent_cost)
		{
			break;
		}
		state->heap[index].node = parent_node;
		state->heap[index].cost = parent_cost;
		state->nodes[state->heap[index].node].heap_index = index;
		index = parent;
	}

	state->heap[index].node = node;
	state->heap[index].cost = cost;
	state->nodes[node].heap_index = index;
}

// @retail 0x271ef0
PRIVATE void path_heap_bubble_down(path_state *state, short index)
{
	short cost;
	short node;
	node = state->heap[index].node;
	cost = state->heap[index].cost;

	for (;;)
	{
		short best = index;
		short best_node = node;
		short best_cost = cost;
		short i = 0;
		short child = index * 2;
		for (; i < 2; i++, child++)
		{
			if (child >= state->heap_count)
			{
				break;
			}
			short child_cost = state->heap[child].cost;
			if (child_cost < best_cost)
			{
				best_node = state->heap[child].node;
				best = child;
				best_cost = child_cost;
			}
		}
		if (best == index)
		{
			break;
		}
		state->heap[index].cost = best_cost;
		state->heap[index].node = best_node;
		state->nodes[best_node].heap_index = index;
		index = best;
	}

	state->heap[index].node = node;
	state->heap[index].cost = cost;
	state->nodes[node].heap_index = index;
}
