/* UNKNOWN_2C3550.CPP: the search for a way around the obstacles in a path's
   way: its nodes, kept in a binary heap ordered by cost */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "unknown_2c0d00.h"
#include <math.h>

// @flags /O2 /arch:SSE /Gr

/* a node of the search (0x34 bytes) */
struct s_avoidance_node
{
	point2f position;
	long value08;
	/* the direction from the point to the search's goal */
	point2f direction;
	/* the distance from the point to the search's goal */
	real distance;
	short obstacle;
	short value1a;
	bool side;
	byte unknown1d;
	/* the next nodes around the obstacle, on either side */
	short children[2];
	byte unknown22[2];
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
	real radius;
	bool value04;
	byte unknown05[3];
	s_obstacle_list *obstacles;
	long value0c;
	point2f goal;
	long value18;
	/* the obstacle that holds the goal */
	short goal_obstacle;
	short value1e;
	short best_index;
	byte unknown22[2];
	real best_distance;
	bool value28;
	bool value29;
	short node_count;
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

/* adds a node at the point, passing the obstacle on the side, unless it
   leads back around an obstacle on its way; returns its index or NONE */
// @retail 0x2c3680
short avoidance_add_node(s_avoidance_search *search, bool side, point2f const *point, long value08, short obstacle,
	short value1a, real cost, short parent)
{
	short result = NONE;

	if (search->node_count < 64)
	{
		point2f direction;
		real distance;
		bool reaches_goal_obstacle;
		short index;
		s_avoidance_node *node;

		direction.x = search->goal.x - point->x;
		direction.y = search->goal.y - point->y;
		distance = (real)sqrt(direction.x * direction.x + direction.y * direction.y);
		if (fabs(distance) < 0.0001f)
		{
			distance = 0.0f;
		}
		else
		{
			real inverse = 1.0f / distance;

			direction.x *= inverse;
			direction.y *= inverse;
		}
		reaches_goal_obstacle = false;
		for (index = parent; index != NONE; index = node->parent)
		{
			node = &search->nodes[index];
			if (node->obstacle != obstacle)
			{
				if (obstacle == search->goal_obstacle && search->goal_obstacle != NONE)
				{
					short sibling;
					short *child;

					reaches_goal_obstacle = true;
					sibling = node->children[!side];
					if (sibling != NONE)
					{
						s_avoidance_node *other = &search->nodes[sibling];

						if (other->direction.y * direction.y + other->direction.x * direction.x > 0.0f)
						{
							s_avoidance_node *from = &search->nodes[parent];

							if (0.0f > (from->direction.y * other->direction.x - other->direction.y * from->direction.x) *
								(direction.y * other->direction.x - other->direction.y * direction.x))
							{
								goto done;
							}
						}
					}
					child = &node->children[side];
					if (*child == parent || *child == NONE)
					{
						*child = search->node_count;
					}
				}
				break;
			}
			if (node->side != side)
			{
				goto done;
			}
			if (obstacle == search->goal_obstacle && search->goal_obstacle != NONE && distance > node->distance)
			{
				goto done;
			}
		}
		index = search->node_count++;
		node = &search->nodes[index];
		node->position.x = point->x;
		node->position.y = point->y;
		node->value08 = value08;
		node->direction = direction;
		node->value1a = value1a;
		node->required_count = 0;
		node->reached_count = 0;
		node->value = NONE;
		node->distance = distance;
		node->obstacle = obstacle;
		node->side = side;
		node->cost = distance + cost;
		node->parent = parent;
		node->marked = false;
		node->children[0] = NONE;
		node->children[1] = NONE;
		if (reaches_goal_obstacle && search->best_distance > node->distance)
		{
			search->best_distance = node->distance;
			search->best_index = index;
		}
		if (search->heap_count < 64)
		{
			short heap_index = search->heap_count++;

			search->heap[heap_index] = index;
			avoidance_heap_sift_up(search, heap_index);
		}
		result = index;
	}
done:
	return result;
}

/* starts a search from the start to the goal around the obstacles, and
   flags the obstacles the goal is within reach of */
// @retail 0x2c3900
void avoidance_search_begin(s_avoidance_search *search, bool value04, s_obstacle_list *obstacles, long value0c, real radius,
	point2f const *start, long value08, point2f const *goal, long value18, bool value29)
{
	short containing;

	search->radius = radius;
	search->value04 = value04;
	search->obstacles = obstacles;
	search->value0c = value0c;
	search->value28 = false;
	search->goal = *goal;
	search->value18 = value18;
	containing = obstacle_list_find_containing(obstacles, NONE, goal, radius);
	search->goal_obstacle = containing != NONE ? obstacles->obstacles[containing].group : NONE;
	search->value29 = value29;
	search->value1e = NONE;
	search->best_index = NONE;
	search->result = NONE;
	search->best_distance = 3.4028235e38f;
	search->node_count = 0;
	search->heap_count = 0;
	avoidance_add_node(search, false, start, value08, NONE, NONE, 0.0f, NONE);
	for (short i = 0; i < obstacles->count; i++)
	{
		s_obstacle *obstacle = &obstacles->obstacles[i];
		real dx = obstacle->center.x - goal->x;
		real dy = obstacle->center.y - goal->y;
		real distance = dx * dx;

		distance += dy * dy;
		if (radius + obstacle->radius > (real)sqrt(distance))
		{
			obstacle->flags |= 2;
		}
	}
}