/* UNKNOWN_2C3550.CPP: the search for a way around the obstacles in a path's
   way: its nodes, kept in a binary heap ordered by cost */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"

// @flags /O2 /arch:SSE /Gr

/* a node of the search (0x34 bytes) */
struct s_avoidance_node
{
	byte unknown00[0x24];
	real cost;
	short parent;
	/* how many of its children must be reached before it is */
	short required_count;
	short reached_count;
	short value;
	bool marked;
	byte unknown31[3];
};

/* the search's state */
struct s_avoidance_search
{
	byte unknown000[0x2c];
	s_avoidance_node nodes[64];
	short heap_count;
	short heap[64];
	short result;
};

/* moves a heap entry up while its parent costs more */
// @retail 0x2c3550
void avoidance_heap_sift_up(s_avoidance_search *search, short index)
{
	while (index > 0)
	{
		short parent = (index - 1) >> 1;
		short parent_node = search->heap[parent];

		if (!(search->nodes[parent_node].cost > search->nodes[search->heap[index]].cost))
		{
			break;
		}
		search->heap[parent] = search->heap[index];
		search->heap[index] = parent_node;
		index = parent;
	}
}

/* moves a heap entry down while a child costs less */
// @retail 0x2c35c0
void avoidance_heap_sift_down(s_avoidance_search *search, short index)
{
	if (index >= search->heap_count)
	{
		return;
	}
	for (;;)
	{
		short count = search->heap_count;
		short smallest = index;
		short child = index * 2 + 1;

		if (child < count && search->nodes[search->heap[index]].cost > search->nodes[search->heap[child]].cost)
		{
			smallest = child;
		}
		child = index * 2 + 2;
		if (child < count && search->nodes[search->heap[smallest]].cost > search->nodes[search->heap[child]].cost)
		{
			smallest = child;
		}
		if (smallest == index)
		{
			break;
		}
		{
			short node = search->heap[index];
			short other = search->heap[smallest];

			search->heap[smallest] = node;
			search->heap[index] = other;
		}
		index = smallest;
	}
}

/* marks the node and its parents back to the root, as far as each parent
   has had all its children reached; the root's value is the result */
// @retail 0x2c3cc0
void avoidance_mark_path(s_avoidance_search *search, short index, short value)
{
	while (index != NONE)
	{
		s_avoidance_node *node = &search->nodes[index];
		s_avoidance_node *parent;

		node->marked = true;
		if (node->value == NONE && value != NONE)
		{
			node->value = value;
		}
		if (node->parent == NONE)
		{
			search->result = node->value;
			return;
		}
		parent = &search->nodes[node->parent];
		parent->reached_count++;
		if (parent->reached_count < parent->required_count)
		{
			return;
		}
		index = node->parent;
	}
}
