// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "real_math.h"
#include "path.h"
#include "unknown_20fe20.h"
#include "lane_c_callees.h"
#include <float.h>
#include <string.h>

real function_30bf0(real_vector3d *v);

bool function_26c4e0(s_node_point const *start, s_node_point const *end,
	s_path_trace_result *result, s_pathfinding_data *pathfinding,
	long start_node_index, long end_node_index, long flags);

/* Local views of fields not yet named in path.h. */
struct s_path_input_view
{
	real radius;
	bool unknown04;
	byte unknown05[0xb];
	bool start_valid;
	s_node_point start;
	long start_node_index;
	bool attractor_valid;
	real_point3d attractor_point;
	long attractor_object_index;
	real attractor_radius;
	real attractor_weight;
	bool unknown44;
	bool distance_limit_valid;
	byte unknown46[2];
	real distance_limit;
	real link_penalty;
};

struct s_path_destination_view
{
	byte unknown00[0x54];
	bool destination_valid;
	s_node_point destination;
	long destination_node_index;
	real destination_radius;
};

/* The existing path_node view begins at the heap-index field. This view
   begins at the actual node's start and exposes the hash key at +8. */
struct s_path_node_key_view
{
	short child;
	short parent;
	long unknown04;
	long node_index;
	bool flag0c;
	bool flag0d;
	bool flag0e;
	byte unknown0f;
	short unknown10;
	short unknown12;
	long unknown14;
	s_node_point entry_point;
	real entry_distance;
	real attractor_distance;
	real path_distance;
	real cost;
	real estimated_distance;
	short quantized_cost;
	short depth;
	short heap_index;
	short unknown42;
};

struct s_path_lookup_view
{
	byte unknown00[0xb0];
	s_path_node_key_view nodes[1024];
	short heap_count;
	path_heap_entry heap[1025];
	short hash_table[4096];
};

struct s_path_location_entry_view
{
	short unknown00;
	short unknown02;
	long node_index;
};

struct s_path_location_view
{
	short flags;
	short count;
	s_path_location_entry_view entries[3];
};

struct s_path_link_view
{
	long node_index;
	word flags;
	short unknown06;
	long type;
	long index;
	s_node_point point;
	real_vector3d vector;
	bool unknown2c;
	bool unknown2d;
	bool unknown2e;
};

struct s_path_closest_view
{
	byte unknown00[0x90];
	short node_index;
	short unknown92;
	real distance;
	real estimated_distance;
	s_node_point point;
};

short __stdcall build_path_links_for_sector(s_pathfinding_data const *pathfinding,
	s_path_node_key_view const *node, s_path_link_view *links, path_state const *state);
PRIVATE bool path_state_traverse(path_state *state);
PRIVATE void path_heap_bubble_down(path_state *state, short index);
PRIVATE real path_attractor_weight(path_state const *state, s_node_point const *start,
	real_point3d const *end, real *distance_out);

PRIVATE void path_heap_bubble_up(path_state *state, short index);
PRIVATE void path_heap_insert(path_state *state, short node_index, short cost);
short path_node_from_hash_table(path_state *state, long node_index);
PRIVATE void closest_point_to_attractor(real_point3d const *attractor,
	real_point3d const *start, real_point3d const *end, real_point3d *out);

// @retail 0x270590
void path_input_set_start(s_path_source *source, s_node_point const *point, long node_index)
{
	s_path_input_view *input = (s_path_input_view *)source;
	input->start_valid = true;
	input->start = *point;
	input->start_node_index = node_index;
}

// @retail 0x2705c0
void path_input_set_attractor(s_path_source *source, real_point3d const *point,
	real radius, long object_index, real weight, bool unknown)
{
	s_path_input_view *input = (s_path_input_view *)source;
	input->attractor_valid = true;
	input->attractor_point = *point;
	input->attractor_object_index = object_index;
	input->attractor_radius = radius;
	input->attractor_weight = weight;
	input->unknown44 = unknown;
}

// @retail 0x270600
PRIVATE bool function_270600(s_path_location const *location, long node_index)
{
	s_path_location_view const *view = (s_path_location_view const *)location;
	for (short i = 0; i < view->count; i++)
	{
		if (view->entries[i].unknown00 == NONE &&
			view->entries[i].node_index == node_index && (view->flags & (1 << i)))
		{
			return true;
		}
	}
	return false;
}

// @retail 0x270640
PRIVATE bool path_state_approach_point(path_state *state, s_node_point const *point,
	long node_index, bool *at_start, s_node_point *out)
{
	short index = path_node_from_hash_table(state, node_index);
	if (index == NONE)
	{
		return false;
	}
	s_path_lookup_view *lookup = (s_path_lookup_view *)state;
	s_path_node_key_view *node = &lookup->nodes[index];
	while (node->parent != NONE)
	{
		s_path_node_key_view *parent = &lookup->nodes[node->parent];
		if (point->output_index != parent->entry_point.output_index)
		{
			break;
		}
		s_path_trace_result trace;
		if (function_26c4e0(point, &parent->entry_point, &trace,
			(s_pathfinding_data *)state->pathfinding, node_index, parent->node_index, 0))
		{
			break;
		}
		node = &lookup->nodes[node->parent];
	}
	if (node->parent == NONE)
	{
		*at_start = true;
		*out = ((s_path_input_view *)&state->source)->start;
	}
	else
	{
		*at_start = false;
		*out = node->entry_point;
	}
	return true;
}

// @retail 0x270750
bool function_270750(byte *buffer, long unknown, s_actor_point_target const *target,
	real *distance, long a, long b)
{
	path_state *state = (path_state *)buffer;
	s_path_input_view *input = (s_path_input_view *)&state->source;
	s_path_lookup_view *lookup = (s_path_lookup_view *)state;
	real *attractor_distance = (real *)a;
	real_vector3d *direction = (real_vector3d *)b;
	short index = path_node_from_hash_table(state, unknown);
	if (index == NONE)
	{
		if (attractor_distance)
		{
			*attractor_distance = FLT_MAX;
		}
		if (direction)
		{
			*direction = *g_4687a4;
		}
		*distance = FLT_MAX;
		return false;
	}
	s_path_node_key_view *node = &lookup->nodes[index];
	real path_distance = function_210970(&node->entry_point, target) + node->path_distance;
	real closest_distance = 0.0f;
	if (input->attractor_valid)
	{
		real_point3d start_point, end_point, closest;
		function_210850(&node->entry_point, &start_point);
		function_210850(target, &end_point);
		closest_point_to_attractor(&input->attractor_point, &start_point, &end_point, &closest);
		double dx = (double)closest.x - input->attractor_point.x;
		double dy = (double)closest.y - input->attractor_point.y;
		double dz = (double)closest.z - input->attractor_point.z;
		real value = (real)sqrt(dz * dz + dx * dx + dy * dy);
		closest_distance = value > node->attractor_distance ? node->attractor_distance : value;
	}
	if (attractor_distance)
	{
		*attractor_distance = closest_distance;
	}
	*distance = path_distance;
	if (direction)
	{
		short child = NONE;
		short current = index;
		do
		{
			node = &lookup->nodes[current];
			node->child = child;
			child = current;
			current = node->parent;
		}
		while (current != NONE);
		current = child;
		real accumulated = 0.0f;
		while (current != NONE && accumulated < 0.8f)
		{
			node = &lookup->nodes[current];
			current = node->child;
			accumulated += node->entry_distance;
		}
		s_node_point const *end = current == NONE ? target : &node->entry_point;
		function_210be0(&input->start, end, direction);
		function_30bf0(direction);
	}
	return true;
}

// @retail 0x2713c0
void path_state_destination(path_state *state, s_node_point const *point,
	long node_index, real radius)
{
	s_path_destination_view *destination = (s_path_destination_view *)state;
	destination->destination_valid = true;
	destination->destination = *point;
	destination->destination_node_index = node_index;
	destination->destination_radius = radius;
}

// @retail 0x2713f0
PRIVATE bool path_state_begin(path_state *state)
{
	s_path_input_view *input = (s_path_input_view *)&state->source;
	s_path_destination_view *destination = (s_path_destination_view *)state;
	s_pathfinding_data *pathfinding = (s_pathfinding_data *)state->pathfinding;
	if (!pathfinding)
	{
		return false;
	}
	long node_count = *(long *)pathfinding;
	if (input->start_node_index < 0 || input->start_node_index >= node_count ||
		!(input->start.point.z > -1000.0f))
	{
		return false;
	}
	real distance = 0.0f;
	long quantized = 0;
	if (destination->destination_valid)
	{
		double length = function_210970(&input->start, &destination->destination);
		distance = (real)length;
		quantized = (long)(length * 10.0);
		if (quantized >= 32767)
		{
			return false;
		}
	}
	s_pathfinding_node *sector = &pathfinding->nodes[input->start_node_index];
	short index = state->unknownae++;
	s_path_lookup_view *lookup = (s_path_lookup_view *)state;
	s_path_node_key_view *node = &lookup->nodes[index];
	node->parent = NONE;
	node->unknown04 = NONE;
	node->node_index = input->start_node_index;
	node->entry_point = input->start;
	node->entry_distance = 0.0f;
	node->path_distance = 0.0f;
	node->attractor_distance = FLT_MAX;
	node->cost = 0.0f;
	node->estimated_distance = distance;
	node->quantized_cost = (short)quantized;
	node->depth = 0;
	node->flag0c = (sector->flags >> 12) & 1;
	node->flag0d = (sector->flags & 0x3c0) != 0;
	node->flag0e = false;
	node->unknown10 = NONE;
	node->unknown14 = NONE;
	if (destination->destination_valid)
	{
		state->unknown90 = index;
		*(real *)((byte *)state + 0x94) = distance;
		*(real *)((byte *)state + 0x98) = distance;
		*(s_node_point *)((byte *)state + 0x9c) = input->start;
	}
	lookup->hash_table[(node->node_index & 511) * 8] = index;
	path_heap_insert(state, index, (short)quantized);
	return true;
}

// @retail 0x2715a0
bool function_2715a0(byte *buffer)
{
	path_state *state = (path_state *)buffer;
	s_path_lookup_view *lookup = (s_path_lookup_view *)state;
	s_path_closest_view *closest = (s_path_closest_view *)state;
	state->unknownae = 0;
	state->heap_count = 1;
	memset(lookup->hash_table, 0xff, sizeof(lookup->hash_table));
	closest->node_index = NONE;
	closest->distance = FLT_MAX;
	closest->estimated_distance = FLT_MAX;
	bool result = false;
	if (path_state_begin(state))
	{
		result = path_state_traverse(state);
	}
	if (!result)
	{
		if (state->location.unknown00 == 0)
		{
			state->location.unknown00 = 32;
		}
		else if (++state->location.unknown00 > 32)
		{
			state->location.unknown00 = 0;
		}
	}
	return result;
}

// @retail 0x271630
PRIVATE bool path_state_traverse(path_state *state)
{
	s_path_input_view *input = (s_path_input_view *)&state->source;
	s_path_destination_view *destination = (s_path_destination_view *)state;
	s_path_lookup_view *lookup = (s_path_lookup_view *)state;
	s_path_closest_view *closest = (s_path_closest_view *)state;
	s_path_location_view *location = (s_path_location_view *)&state->location;
	s_pathfinding_data *pathfinding = (s_pathfinding_data *)state->pathfinding;
	real radius = 0.2f > input->radius ? 0.2f : input->radius;
	real link_penalty = input->link_penalty;
	s_path_link_view links[64];
	while (state->heap_count > 1)
	{
		short index = state->heap[1].node;
		s_path_node_key_view *node = &lookup->nodes[index];
		node->heap_index = NONE;
		if (--state->heap_count > 1)
		{
			state->heap[1] = state->heap[state->heap_count];
			path_heap_bubble_down(state, 1);
		}
		if (index == NONE)
		{
			break;
		}
		if (destination->destination_valid)
		{
			if (node->node_index == destination->destination_node_index)
			{
				closest->point = destination->destination;
				closest->node_index = index;
				closest->distance = 0.0f;
				break;
			}
			real limit = 5.0f > closest->distance ? 5.0f : closest->distance;
			if (node->estimated_distance > limit * 10.0f + closest->estimated_distance)
			{
				break;
			}
		}
		short count = build_path_links_for_sector(pathfinding, node, links, state);
		for (short i = 0; i < count; ++i)
		{
			s_path_link_view *link = &links[i];
			if (link->node_index == node->unknown04 && !link->unknown2e && !node->flag0e)
			{
				continue;
			}
			if (!(link->flags & 1) || (node->flag0c && (link->flags & 0x1000)))
			{
				continue;
			}
			bool blocked = false;
			if (location->flags > 0)
			{
				for (short j = 0; j < location->count; ++j)
				{
					if ((location->flags & (1 << j)) &&
						location->entries[j].unknown00 == (short)link->type &&
						location->entries[j].node_index == link->index)
					{
						blocked = true;
						break;
					}
				}
			}
			if (blocked || (!input->unknown04 && (link->flags & 2) &&
				function_1fa6b0(&pathfinding->nodes[link->node_index], pathfinding)))
			{
				continue;
			}
			real length_squared = link->vector.i * link->vector.i +
				link->vector.j * link->vector.j + link->vector.k * link->vector.k;
			real diameter = radius * 2.0f;
			if (link->unknown2c && !link->unknown2d && diameter * diameter > length_squared)
			{
				continue;
			}
			s_node_point point;
			point.point.x = link->vector.i * 0.5f + link->point.point.x;
			point.point.y = link->vector.j * 0.5f + link->point.point.y;
			point.point.z = link->vector.k * 0.5f + link->point.point.z;
			point.output_index = link->point.output_index;
			if (destination->destination_valid && length_squared > 16.0f &&
				length_squared > diameter * diameter)
			{
				real length = (real)sqrt(length_squared);
				real_vector3d to_destination;
				function_210be0(&link->point, &destination->destination, &to_destination);
				real t = (link->vector.j * to_destination.j +
					link->vector.k * to_destination.k + link->vector.i * to_destination.i) /
					(link->vector.i * link->vector.i + link->vector.j * link->vector.j +
					link->vector.k * link->vector.k);
				real margin = radius / length;
				if (margin > t)
				{
					t = margin;
				}
				else if (t > 1.0f - margin)
				{
					t = 1.0f - margin;
				}
				point.point.x = link->vector.i * t + link->point.point.x;
				point.point.y = link->vector.j * t + link->point.point.y;
				point.point.z = link->vector.k * t + link->point.point.z;
			}
			double distance = function_210970(&node->entry_point, &point);
			real entry_distance = (real)distance;
			real path_distance = (real)(distance + node->path_distance);
			real attractor_distance;
			real entry_cost;
			if (input->attractor_valid)
			{
				real weight = path_attractor_weight(state, &node->entry_point, &point.point, &attractor_distance);
				entry_cost = (weight + 1.0f) * entry_distance;
				attractor_distance = node->attractor_distance > attractor_distance ?
					attractor_distance : node->attractor_distance;
			}
			else
			{
				attractor_distance = 0.0f;
				entry_cost = entry_distance;
			}
			if (link_penalty > 0.0f)
			{
				switch ((short)link->type)
				{
				case 1: case 2: case 5: case 6:
					entry_cost += link_penalty;
					break;
				}
			}
			real cost = node->cost + entry_cost;
			real estimated_distance = cost;
			real destination_distance;
			if (destination->destination_valid)
			{
				double remaining = function_210970(&point, &destination->destination);
				destination_distance = (real)remaining;
				estimated_distance = (real)(remaining + cost);
			}
			long quantized = (long)(estimated_distance * 10.0f);
			if (quantized >= 32767 || (input->distance_limit_valid && path_distance > input->distance_limit))
			{
				continue;
			}
			short hash = (short)((link->node_index & 511) * 8);
			short next = lookup->hash_table[hash];
			while (next != NONE)
			{
				s_path_node_key_view *candidate = &lookup->nodes[next];
				if (candidate->node_index == link->node_index)
				{
					if (!candidate->flag0e && !link->unknown2e)
					{
						break;
					}
					real dx = candidate->entry_point.point.x - link->point.point.x;
					real dy = candidate->entry_point.point.y - link->point.point.y;
					real dz = candidate->entry_point.point.z - link->point.point.z;
					if (dx * dx + dz * dz + dy * dy <= 0.09f)
					{
						break;
					}
				}
				hash = (short)((hash + 1) & 4095);
				next = lookup->hash_table[hash];
			}
			if (next == NONE)
			{
				next = state->unknownae;
				if (next >= 1024)
				{
					continue;
				}
				++state->unknownae;
				lookup->hash_table[hash] = next;
				lookup->nodes[next].heap_index = NONE;
			}
			else if (quantized >= lookup->nodes[next].quantized_cost || lookup->nodes[next].heap_index == NONE)
			{
				continue;
			}
			if (next == NONE)
			{
				continue;
			}
			s_path_node_key_view *child = &lookup->nodes[next];
			child->parent = index;
			child->unknown04 = node->node_index;
			child->node_index = link->node_index;
			child->entry_point = point;
			child->entry_distance = entry_distance;
			child->attractor_distance = attractor_distance;
			child->path_distance = path_distance;
			child->cost = cost;
			child->estimated_distance = estimated_distance;
			child->quantized_cost = (short)quantized;
			child->flag0c = (link->flags >> 12) & 1;
			child->flag0d = (link->flags & 0x3c0) != 0;
			child->flag0e = link->unknown2e;
			*(long *)&child->unknown10 = link->type;
			child->unknown14 = link->index;
			short depth = node->depth;
			switch ((short)link->type)
			{
			case 1: case 5: case 6:
				depth += 2;
				break;
			case 2:
				depth += *(short *)((byte *)&pathfinding->surfaces[link->index] + 0xa) == 0 ? 2 : 1;
				break;
			default:
				++depth;
				break;
			}
			child->depth = depth;
			if (child->heap_index == NONE)
			{
				path_heap_insert(state, next, (short)quantized);
			}
			else
			{
				state->heap[child->heap_index].cost = (short)quantized;
				path_heap_bubble_up(state, child->heap_index);
			}
			if (destination->destination_valid && destination->destination_radius > 0.0f &&
				closest->distance > destination_distance)
			{
				closest->point = child->entry_point;
				closest->node_index = next;
				closest->distance = destination_distance;
				closest->estimated_distance = estimated_distance;
			}
		}
	}
	return !destination->destination_valid || destination->destination_radius >= closest->distance;
}

// @retail 0x271fd0
PRIVATE void path_heap_insert(path_state *state, short node_index, short cost)
{
	short index = state->heap_count;
	if (index < 1024)
	{
		state->heap_count = index + 1;
		state->heap[index].node = node_index;
		state->heap[index].cost = cost;
		path_heap_bubble_up(state, index);
	}
}

// @retail 0x272700
short path_node_from_hash_table(path_state *state, long node_index)
{
	s_path_lookup_view *lookup = (s_path_lookup_view *)state;
	short hash_index = (node_index & 511) * 8;
	short result;
	do
	{
		result = lookup->hash_table[hash_index];
		hash_index = (hash_index + 1) & 4095;
	}
	while (result != NONE && lookup->nodes[result].node_index != node_index);
	return result;
}

// @retail 0x272740
PRIVATE void closest_point_to_attractor(real_point3d const *attractor,
	real_point3d const *start, real_point3d const *end, real_point3d *out)
{
	real_vector3d delta;
	delta.i = end->x - start->x;
	delta.j = end->y - start->y;
	delta.k = end->z - start->z;
	real length_squared = delta.i * delta.i + delta.j * delta.j + delta.k * delta.k;
	if (length_squared > 0.0f)
	{
		real t = ((start->y - attractor->y) * delta.j +
			(start->z - attractor->z) * delta.k +
			(start->x - attractor->x) * delta.i) / length_squared;
		if (t < 0.0f || t > 1.0f)
		{
			*out = *end;
		}
		else
		{
			out->x = delta.i * t + start->x;
			out->y = delta.j * t + start->y;
			out->z = delta.k * t + start->z;
		}
	}
	else
	{
		*out = *start;
	}
}

// @retail 0x272810
PRIVATE real path_attractor_weight(path_state const *state, s_node_point const *start,
	real_point3d const *end, real *distance_out)
{
	s_path_input_view const *input = (s_path_input_view const *)&state->source;
	real distance = FLT_MAX;
	real weight = 0.0f;
	real_point3d local_end;
	real_point3d local_attractor;
	function_210690(start->output_index, end, &local_end);
	function_210690(start->output_index, &input->attractor_point, &local_attractor);
	real_vector3d delta;
	vector3d_from_points3d(&start->point, &local_end, &delta);
	real length_squared = delta.k * delta.k + delta.i * delta.i + delta.j * delta.j;
	real_point3d closest;
	if (length_squared > 0.0f)
	{
		real t = ((start->point.z - local_attractor.z) * delta.k +
			(start->point.y - local_attractor.y) * delta.j +
			(start->point.x - local_attractor.x) * delta.i) / length_squared;
		if (t < 0.0f || t > 1.0f)
		{
			closest = local_end;
		}
		else
		{
			closest.x = delta.i * t + start->point.x;
			closest.y = delta.j * t + start->point.y;
			closest.z = delta.k * t + start->point.z;
		}
	}
	else
	{
		closest = start->point;
	}
	vector3d_from_points3d(&local_attractor, &closest, &delta);
	real distance_squared = delta.k * delta.k + delta.j * delta.j + delta.i * delta.i;
	if (input->attractor_radius * input->attractor_radius > distance_squared)
	{
		double length = sqrt(distance_squared);
		distance = (real)length;
		weight = (real)((1.0 - length / input->attractor_radius) * input->attractor_weight);
	}
	*distance_out = distance;
	return weight;
}
