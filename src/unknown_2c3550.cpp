/* UNKNOWN_2C3550.CPP: the search for a way around the obstacles in a path's
   way: its nodes, kept in a binary heap ordered by cost */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "unknown_2c0d00.h"
#include "unknown_26c380.h"
#include "globals.h"
#include <math.h>
#include <string.h>

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
	s_match_globals const *value0c;
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

static __forceinline real avoidance_normalize2d(point2f *direction)
{
	real distance = (real)sqrt(direction->x * direction->x + direction->y * direction->y);

	if (!(fabs(distance) < 0.0001f))
	{
		real inverse = 1.0f / distance;

		direction->x = inverse * direction->x;
		direction->y = direction->y * inverse;
		return distance;
	}
	return 0.0f;
}

/* adds a node at the point, passing the obstacle on the side, unless it
   leads back around an obstacle on its way; returns its index or NONE */
// @retail 0x2c3680
short avoidance_add_node(s_avoidance_search *search, point2f const *point, bool side, long value08, short obstacle,
	short value1a, real cost, short parent)
{
	short result = NONE;

	if (search->node_count < 64)
	{
		bool reaches_goal_obstacle = false;
		point2f direction;
		real distance;
		short index;
		s_avoidance_node *node;
		/* This short remains on the stack in the retail calling convention. */
		(void)&obstacle;

		direction.x = search->goal.x - point->x;
		direction.y = search->goal.y - point->y;
		distance = avoidance_normalize2d(&direction);
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
void avoidance_search_begin(s_avoidance_search *search, bool value04, s_obstacle_list *obstacles, s_match_globals const *value0c, real radius,
	point2f const *start, long value08, point2f const *goal, long value18, bool value29)
{
	short containing;

	search->radius = radius;
	search->obstacles = obstacles;
	search->value0c = value0c;
	search->value04 = value04;
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
	avoidance_add_node(search, start, false, value08, NONE, NONE, 0.0f, NONE);
	for (short i = 0; i < obstacles->count; i++)
	{
		s_obstacle *obstacle = &obstacles->obstacles[i];
		real dx = obstacle->center.x - goal->x;
		real dy = obstacle->center.y - goal->y;
		real distance = dx * dx;

		distance += dy * dy;
		if ((real)sqrt(distance) < radius + obstacle->radius)
		{
			obstacle->flags |= 2;
		}
	}
}

struct s_avoidance_trace
{
	real distance;
	long sector;
	long edge;
	short obstacle;
	short group;
};

struct s_path_location;
bool __stdcall function_26ccb0(s_pathfinding_data const *pathfinding, s_obstacle_list const *obstacles,
	short ignored_obstacle, point2f const *origin, long sector, long target_sector, point2f const *direction,
	real radius, real distance, bool first, bool stop_at_goal, bool value29, s_path_location const *location, s_avoidance_trace *trace);
void obstacle_tangent_directions(point2f const *point, s_obstacle_list const *list, short index, real radius,
	point2f *left, point2f *right, real *tangent_length);

static inline bool avoidance_bit_test(dword const *bits, long index)
{
	return (bits[index >> 5] & (1 << (index & 31))) != 0;
}

static inline void avoidance_bit_set(dword *bits, long index)
{
	bits[index >> 5] |= 1 << (index & 31);
}

static __forceinline point2f *avoidance_scale2d(point2f const *direction, real distance, point2f *scaled)
{
	scaled->y = direction->y * distance;
	scaled->x = distance * direction->x;
	return scaled;
}

static __forceinline point2f *avoidance_add2d(point2f const *a, point2f const *b, point2f *out)
{
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	return out;
}

static __forceinline point2f *avoidance_point_along2d(point2f const *origin, point2f const *direction,
	real distance, point2f *point)
{
	point->x = direction->x * distance + origin->x;
	point->y = direction->y * distance + origin->y;
	return point;
}

static __forceinline real avoidance_length2d(point2f const *direction)
{
	real square = direction->x * direction->x;

	square += direction->y * direction->y;
	return (real)sqrt(square);
}

/* Flood the obstacles met by either tangent and add a node on each clear side. */
// @retail 0x2c39f0
short __stdcall avoidance_expand_obstacle(s_pathfinding_data const *pathfinding, s_avoidance_search *search,
	short node_index, short obstacle_index)
{
	s_avoidance_node *node = &search->nodes[node_index];
	dword visited[2];
	short pending[64];
	short pending_count;
	short added;

	memset(visited, 0, ((search->obstacles->count + 31) >> 5) * sizeof(dword));
	avoidance_bit_set(visited, obstacle_index);
	added = 0;
	pending[0] = obstacle_index;
	pending_count = 1;
	do
	{
		pending_count--;
		short current = pending[pending_count];
		short group = current != NONE ? search->obstacles->obstacles[current].group : NONE;
		point2f directions[2];
		real tangent_length;

		obstacle_tangent_directions(&node->position, search->obstacles, current, search->radius,
			&directions[0], &directions[1], &tangent_length);
		if (search->radius > tangent_length)
		{
			tangent_length = search->radius;
		}
		real distance = search->obstacles->obstacles[current].radius + search->radius + tangent_length;
		for (short side = 0; side < 2; side++)
		{
			s_avoidance_trace trace;

			function_26ccb0(pathfinding, search->obstacles, current, &node->position, node->value08, NONE,
				&directions[side], search->radius, distance, false, false, search->value29, false, &trace);
			if (trace.obstacle != NONE && !avoidance_bit_test(visited, trace.obstacle))
			{
				avoidance_bit_set(visited, trace.obstacle);
				pending[pending_count++] = trace.obstacle;
			}
			if (trace.distance > tangent_length && trace.group != group)
			{
				distance = (trace.distance + tangent_length) * 0.5f;
				point3f origin;
				vector3f direction;
				s_path_trace_result sector_trace;

				origin.x = node->position.x;
				origin.y = node->position.y;
				origin.z = 0.0f;
				direction.i = directions[side].x;
				direction.j = directions[side].y;
				direction.k = 0.0f;
				function_26c590(pathfinding, &origin, node->value08, NONE, &direction, distance, NULL, &sector_trace);
				if (avoidance_add_node(search, (point2f const *)&sector_trace.point, side > 0,
					((s_sector_trace_result *)&sector_trace)->sector_index, group, obstacle_index,
					node->cost - node->distance + distance, node_index) != NONE)
				{
					added++;
				}
			}
		}
	}
	while (pending_count > 0);
	return added;
}

// @retail 0x2c3d40
bool avoidance_search_step(s_avoidance_search *search, s_pathfinding_data const *pathfinding, bool allow_partial)
{
	if (search->heap_count > 0)
	{
		short index = search->heap[0];

		search->heap_count--;
		search->heap[0] = search->heap[search->heap_count];
		avoidance_heap_sift_down(search, 0);
		if ((short)index != NONE)
		{
			s_avoidance_node *node = &search->nodes[(short)index];
			point2f const *direction = &node->direction;
			short added = 0;
			s_avoidance_trace trace;

			function_26ccb0(pathfinding, search->obstacles, NONE, &node->position, node->value08,
				search->value18, direction, search->radius, node->distance, node->parent == NONE,
				true, search->value29, false, &trace);
			if (trace.edge == 0xffff)
			{
				if (trace.obstacle == NONE)
				{
					if (trace.sector == search->value18 || node->distance < 0.2f)
					{
						point2f point;

						point.x = node->position.x + direction->x * trace.distance;
						point.y = node->position.y + direction->y * trace.distance;
						search->value1e = avoidance_add_node(search, &point, false, search->value18, NONE, NONE,
							node->cost - node->distance + trace.distance, (short)index);
						added = 1;
					}
				}
				else
				{
					s_obstacle *obstacle = &search->obstacles->obstacles[trace.obstacle];

					if (allow_partial && trace.group == search->goal_obstacle)
					{
						point2f offset;
						point2f point;
						point2f movement;

						offset.x = search->goal.x - obstacle->center.x;
						offset.y = search->goal.y - obstacle->center.y;
						point = node->position;
						avoidance_scale2d(direction, trace.distance, &movement);
						avoidance_add2d(&point, &movement, &point);
						real dx = point.x - search->goal.x;
						real dy = point.y - search->goal.y;
						if (dy * dy + dx * dx < obstacle->radius * obstacle->radius &&
							obstacle->radius - sqrt(offset.y * offset.y + offset.x * offset.x) < obstacle->radius * 0.5f)
						{
							search->value1e = avoidance_add_node(search, &point, false, search->value18, NONE, NONE,
								node->cost, (short)index);
							added = 1;
						}
					}
					if (!added)
					{
						if (trace.group == search->goal_obstacle && search->best_distance > node->distance &&
							node->obstacle == trace.group)
						{
							search->best_distance = node->distance;
							search->best_index = (short)index;
						}
						added = avoidance_expand_obstacle(pathfinding, search, (short)index, trace.obstacle);
					}
				}
			}
			else if (node->value1a != NONE)
			{
				added = avoidance_expand_obstacle(pathfinding, search, (short)index, node->value1a);
				trace.obstacle = node->value1a;
			}
			node->required_count = added;
			if (!added)
			{
				long group = NONE;

				if (trace.obstacle != NONE)
				{
					group = search->obstacles->obstacles[trace.obstacle].group;
				}

				avoidance_mark_path(search, (short)index, (short)group);
			}
		}
	}
	return search->value1e == NONE && search->heap_count > 0;
}

struct s_avoidance_limits
{
	byte unknown00[0x6c];
	real distance;
};

// @retail 0x2c4030
bool avoidance_search(s_pathfinding_data const *pathfinding, s_avoidance_search *search,
	s_avoidance_limits const *limits, bool value04, s_obstacle_list *obstacles, real radius,
	point2f const *start, long sector, point2f const *goal, long target_sector, bool allow_partial, bool value29)
{
	avoidance_search_begin(search, value04, obstacles, g_4e0348, radius, start, sector, goal, target_sector, value29);
	while (avoidance_search_step(search, pathfinding, allow_partial))
	{
	}
	if (search->value1e != NONE)
	{
		search->value28 = true;
	}
	else if (search->best_index != NONE)
	{
		if (allow_partial)
		{
			s_avoidance_node *node = &search->nodes[search->best_index];
			bool accept = limits->distance > 0.0f && limits->distance > node->distance;

			if (!accept)
			{
				s_obstacle *obstacle = &obstacles->obstacles[node->value1a];
				point2f offset;

				offset.x = goal->x - obstacle->center.x;
				offset.y = goal->y - obstacle->center.y;
				accept = obstacle->radius > node->distance &&
					obstacle->radius - avoidance_length2d(&offset) < obstacle->radius * 0.5f;
			}
			if (accept)
			{
				point2f point;

				avoidance_point_along2d(&node->position, &node->direction, node->distance, &point);
				search->value1e = avoidance_add_node(search, &point, false, search->value18, NONE, NONE,
					node->distance, search->best_index);
			}
		}
		else
		{
			search->value1e = search->best_index;
		}
	}
	return search->value1e != NONE;
}
