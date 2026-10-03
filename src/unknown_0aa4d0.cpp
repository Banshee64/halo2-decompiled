// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0AA4D0.CPP: the relevance of simulation entities to the observers
   (the creation relevance the entity definitions print, 0xaa4d0) and the
   update relevance and period of an entity (0xabac0) */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "data_array.h"
#include "object_type_definitions.h"
#include "simulation_entity_database.h"
#include "entity_relevance.h"
#include <float.h>
#include <xtl.h>

real_point3d *function_b9dd0(long object_index, real_point3d *result);

/* an observer of the simulation: its entity, where it is and where it looks */
struct s_relevance_observer
{
	long entity_index;
	real_point3d position;
	real_vector3d forward;
	char type;
	byte unknown1d[7];
};

struct s_relevance_observers
{
	byte unknown00[0xc];
	long count;
	byte unknown10[4];
	s_relevance_observer observers[1];
};

/* the facing thresholds and relevances of the two observer types */
real g_4cece8;
real g_4cecec;
real g_4cecf0;
real g_4cecf4;

/* the object an entity stands for (0xa58d0, inlined) */
static inline long simulation_entity_get_object_index(long entity_index)
{
	long object_index = NONE;
	if (entity_index != NONE)
	{
		s_simulation_world_view *world = (s_simulation_world_view *)g_4cf77c;
		s_simulation_entity *entity = world->database->table.try_get(entity_index);
		if (entity)
		{
			s_entity_definitions *definitions = (s_entity_definitions *)g_4cf784;
			if (definitions->definitions[entity->type]->v7((s_entity *)entity) && entity->object_index != NONE)
				object_index = entity->object_index;
		}
	}
	return object_index;
}

static inline real distance_squared3d(real_point3d const *p0, real_point3d const *p1)
{
	real_vector3d vector;
	vector3d_from_points3d(p0, p1, &vector);
	return magnitude_squared3d(&vector);
}

static inline real normalize3d(real_vector3d *v)
{
	real m = (real)sqrt(v->i * v->i + v->j * v->j + v->k * v->k);
	if (!(fabs(m) < 0.0001f))
	{
		real inv = 1.f / m;
		v->i = v->i * inv;
		v->j = inv * v->j;
		v->k = inv * v->k;
		return m;
	}
	return 0.f;
}

struct s_object_header_view
{
	byte unknown00[3];
	byte type;
	byte unknown04[4];
	void *object;
};

// @retail 0xaa4d0
real function_aa4d0(long count, long const *entity_indices, real maximum_distance,
	s_relevance_observers const *observers, bool *exact)
{
	real minimum_distance_squared = FLT_MAX;
	bool exact_match = false;
	bool facing = false;
	real facing_relevance = 0.0f;
	real relevance;
	for (long i = 0; i < count; i++)
	{
		for (long j = 0; j < observers->count; j++)
		{
			s_relevance_observer const *observer = &observers->observers[j];
			if (observer->entity_index == entity_indices[i])
			{
				relevance = 1.0f;
				exact_match = true;
				goto done;
			}
			long object_index = simulation_entity_get_object_index(entity_indices[i]);
			if (object_index != NONE)
			{
				real_point3d origin;
				function_b9dd0(object_index, &origin);
				real distance_squared = distance_squared3d(&origin, &observer->position);
				if (minimum_distance_squared > distance_squared)
					minimum_distance_squared = distance_squared;
				s_object_header_view *header = &((s_object_header_view *)g_4e0300->data)[object_index & 0xffff];
				if (((1 << header->type) & 3) && observer->type != NONE)
				{
					real threshold = 1.0f;
					real_vector3d direction;
					vector3d_from_points3d(&observer->position, &origin, &direction);
					if (normalize3d(&direction) != 0.0f)
					{
						switch (observer->type)
						{
						case 0:
							threshold = g_4cece8;
							facing_relevance = g_4cecf0;
							break;
						case 1:
							threshold = g_4cecec;
							facing_relevance = g_4cecf4;
							break;
						}
						if (dot_product3d(&observer->forward, &direction) >= threshold)
							facing = true;
					}
				}
			}
		}
	}
	relevance = 0.0f;
	if (maximum_distance > relevance)
	{
		real distance_relevance = (maximum_distance - (real)sqrt(minimum_distance_squared)) / maximum_distance;
		if (!(0.0f > distance_relevance))
			relevance = distance_relevance;
	}
	if (facing && !(relevance > facing_relevance))
		relevance = facing_relevance;
done:
	if (exact)
		*exact = exact_match;
	return relevance;
}

/* the update weights of an entity type (+0x10 of its creation weights) */
struct s_update_weight
{
	real fixed_relevance;
	real maximum_distance;
	real relevance_bounds[2];
	long period_bounds[2];
	real near_relevance[2];
	long far_time;
	real far_relevance[2];
	real stale_relevance;
	real exact_relevance;
	real flagged_relevance;
	real carried_bonus;
};

/* what an entity's update was last sent with: flags, a time and the observers */
struct s_update_state
{
	dword flags;
	long time;
	s_relevance_observers const *observers;
};

struct s_object_carried_view
{
	byte unknown000[0x248];
	long carrier;
};

static inline dword network_time_now(void)
{
	if (g_510548)
		return g_51054c;
	return GetTickCount();
}

// @retail 0xabac0
real function_abac0(real *relevance_out, s_creation_request const *request, s_update_state const *state, long *period_out)
{
	s_update_state const *const *state_reference = &state;
	if (relevance_out)
		*relevance_out = -1.0f;
	if (period_out)
		*period_out = -1;
	short type = request->definition_index;
	s_update_weight const *weight = (s_update_weight const *)g_4cef68[type].update;
	if (weight->fixed_relevance > 0.0f)
		return weight->fixed_relevance;
	bool exact = false;
	real relevance = function_aa4d0(1, &request->entity_index, weight->maximum_distance, state->observers, &exact);
	if (relevance_out)
		*relevance_out = relevance;
	if (type == 9)
	{
		if (exact)
			return weight->exact_relevance;
		if ((*state_reference)->flags & 1)
			return weight->flagged_relevance;
	}
	long period = weight->period_bounds[0];
	if (!(relevance >= weight->relevance_bounds[1]))
	{
		if (weight->relevance_bounds[0] >= relevance)
			period = weight->period_bounds[1];
		else
			period = (long)(weight->period_bounds[1] - (relevance - weight->relevance_bounds[0]) /
				(weight->relevance_bounds[1] - weight->relevance_bounds[0]) * (weight->period_bounds[1] - weight->period_bounds[0]));
	}
	if (period_out)
		*period_out = period;
	long time = network_time_now() - state->time;
	real result;
	if (time < period)
		result = (weight->near_relevance[1] - weight->near_relevance[0]) * relevance + weight->near_relevance[0];
	else if (time < weight->far_time)
		result = (weight->far_relevance[1] - weight->far_relevance[0]) * relevance + weight->far_relevance[0];
	else
		result = weight->stale_relevance;
	if (type == 12 && request->object_index != NONE)
	{
		s_object_carried_view *object = (s_object_carried_view *)((s_object_header *)g_4e0300->data)[request->object_index & 0xffff].object;
		if (object->carrier != NONE)
			result = weight->carried_bonus + result;
	}
	return result;
}
