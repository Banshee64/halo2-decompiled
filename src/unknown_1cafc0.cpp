// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1CAFC0.CPP: the object that drives three animation channels of
   one graph (0x1cafc0..0x1ce1e0) */

#include "cseries.h"
#include "globals.h"
#include "unknown_1c62f0.h"
#include "unknown_123680.h"
#include "unknown_1cafc0.h"
#include "unknown_11cb00.h"
#include <math.h>
#include <string.h>

/* an entry of the graph's weapon block at +0x54 (0x1dd560 finds one by name) */
struct s_graph_name_entry
{
	long name;
	long weapon_class;
};

#define PIN(value, lower, upper) ((value) < (lower) ? (lower) : (value) > (upper) ? (upper) : (value))

/* callees outside this file */
void function_279d80(s_graph_tag *graph, c_animation_id animation_id, long node_count, real frame, real weight,
	s_graph_inheritance *inheritance, dword const *node_mask, real_quaternion_transform *transforms, bool interpolate);
void function_141e10(matrix3x3 *out, real_quaternion const *q);
c_animation_id function_1dd0b0(s_graph_tag *graph, long name);
void function_1dd1c0(s_graph_tag *graph, real_matrix4x3 *matrices, real_orientation const *orientations,
	real_matrix4x3 const *root);

/* the blend counters (unknown_1d9240.cpp) */
struct s_1d9240;
void function_1d9240(s_1d9240 *p, char flag, real x);
real function_1d9430(s_1d9240 const *p);
void function_1d9470(s_1d9240 const *p, s_blend_orientation const *targets, long count, dword const *mask,
	s_blend_orientation *orientations);

bool g_46fbf4 = true;
bool g_46fbf5 = true;

inline bool channel_valid(c_animation_channel const *channel)
{
	return channel->graph_tag_index != NONE && channel->animation_id.index != NONE;
}

// @retail 0x1cafc0
void s_animation_state::reset()
{
	long i;

	for (i = 0; i < 3; i++)
	{
		channels[i].reset();
	}
	flags = 0;
	unknown6e = 0;
	graph_tag_index = NONE;
	unknown64.unknown1 = 0;
	unknown64.unknown0 = 0;
	unknown64.unknown3 = 0;
	unknown60.unknown1 = 0;
	unknown60.unknown0 = 0;
	unknown60.unknown3 = 0;
	unknown70 = NONE;
	unknown7c = NONE;
	unknown74 = NONE;
	unknown78 = NONE;
	unknown80 = 0.0f;
}

// @retail 0x1caed0
s_animation_state::s_animation_state()
{
	reset();
}

// @retail 0x1cb0a0
void s_animation_state::channels_clear_partial()
{
	c_animation_channel *channel = &channels[3];
	long i = 3;

	do
	{
		channel--;
		i--;
		channel->graph_tag_index = NONE;
		channel->animation_id.graph_index = NONE;
		channel->animation_id.index = NONE;
		channel->unknown08 = NONE;
		channel->unknown10 = 0;
	}
	while (i);
}

// @retail 0x1cbd30
short s_animation_state::node_count_get()
{
	return (short)graph_get()->node_count;
}

// @retail 0x1cbde0
long s_animation_state::node_find(long name)
{
	s_graph_tag *graph = graph_get();
	long i;

	for (i = 0; i < graph->node_count; i++)
	{
		if (graph->nodes[i].name == name)
		{
			return i;
		}
	}
	return NONE;
}

// @retail 0x1cbe20
s_graph_entry *s_animation_state::entry_get(long index)
{
	if (index == NONE)
	{
		return NULL;
	}
	return &graph_get()->entries[index];
}

// @retail 0x1cbe90
long s_animation_state::node_count()
{
	s_graph_tag *graph = graph_get();
	long count = 0;

	if (graph)
	{
		count = (short)graph->node_count;
	}
	return count;
}

// @retail 0x1cc2d0
s_graph_tag *s_animation_state::graph_get()
{
	return graph_tag_get(graph_tag_index);
}

// @retail 0x1cb680
void s_animation_state::update_blend_flags()
{
	dword flags0 = channels[0].flags;
	dword flags1 = channels[1].flags;

	if (!g_46fbf4 || !channel_valid(&channels[1]) || 0.45f > unknown80)
	{
		flags &= ~0x20;
		flags0 &= ~0x380;
		flags1 |= 0x380;
	}
	else if (unknown80 > 0.55f)
	{
		flags |= 0x20;
		flags0 |= 0x380;
		flags1 &= ~0x380;
	}
	else
	{
		return;
	}
	if (channel_valid(&channels[0]))
	{
		channels[0].flags = (word)flags0;
	}
	if (channel_valid(&channels[1]))
	{
		channels[1].flags = (word)flags1;
	}
}

// @retail 0x1ccba0
void s_animation_state::channels_clear()
{
	long i;

	for (i = 0; i < 3; i++)
	{
		channels[i].clear();
	}
}

// @retail 0x1ccc50
void s_animation_state::secondary_channels_clear()
{
	long i;

	for (i = 1; i < 3; i++)
	{
		channels[i].clear();
	}
}

// @retail 0x1cd510
c_animation_id *s_animation_state::current_animation_get(c_animation_id *result)
{
	c_animation_id none;

	if (g_46fbf5 && channel_valid(&channels[2]))
	{
		*result = channels[2].animation_id;
	}
	else if (g_46fbf4 && channel_valid(&channels[1]) && unknown80 >= 0.9999f)
	{
		*result = channels[1].animation_id;
	}
	else if (channel_valid(&channels[0]))
	{
		*result = channels[0].animation_id;
	}
	else
	{
		*result = none;
	}
	return result;
}

PRIVATE __forceinline real blend_fraction(real blend)
{
	real fraction = PIN(blend + 0.0001f, 0.0f, 0.9999f) * 1.0002f;

	return PIN(fraction, 0.0f, 1.0f);
}

// @retail 0x1cdf00
real s_animation_state::blend_fraction_get()
{
	return blend_fraction(unknown80);
}

// @retail 0x1cc2f0
void s_animation_state::names_resolve(s_animation_names *names, long mode, long weapon_class, long weapon_type, long set)
{
	s_graph_name_entry *entry;
	long resolved_weapon_class;

	names->mode = mode;
	names->weapon_class = weapon_class;
	names->weapon_type = weapon_type;
	names->set = set;
	if (mode == 0x7000101)
	{
		names->mode = unknown70;
		if (names->mode == NONE)
		{
			names->mode = 0x6000086;
		}
	}
	if (names->mode == 0x7000001)
	{
		names->mode = 0x6000086;
	}
	if (weapon_class == 0x7000101)
	{
		names->weapon_class = unknown74;
	}
	if (names->weapon_class == 0x7000001 || names->weapon_class == NONE)
	{
		names->weapon_class = 0x7000083;
	}
	if (weapon_type == 0x7000101)
	{
		names->weapon_type = unknown78;
	}
	if (names->weapon_type == 0x7000001 || names->weapon_type == NONE)
	{
		names->weapon_type = 0x30000d9;
	}
	resolved_weapon_class = names->weapon_class;
	entry = (s_graph_name_entry *)function_1dd560((s_sorted_array *)&graph_get()->weapons, names->weapon_type, sizeof(s_graph_name_entry));
	if (entry && entry->weapon_class != NONE && entry->weapon_class != 0x30000d9 && resolved_weapon_class != 0x400054b)
	{
		resolved_weapon_class = entry->weapon_class;
	}
	names->weapon_class = resolved_weapon_class;
	if (set == 0x7000101)
	{
		names->set = unknown7c;
		if (names->set == NONE)
		{
			names->set = 0x400000c;
		}
	}
	if (names->set == 0x7000001)
	{
		names->set = 0x400000c;
	}
}

// @retail 0x1ce1e0
void s_animation_state::animation_touch(c_animation_id animation_id)
{
	if (graph_tag_index != NONE)
	{
		function_1dd9d0(graph_tag_get(graph_tag_index), animation_id);
	}
}

// @retail 0x1ccb80
void function_1ccb80(c_animation_channel *channel, real seconds)
{
	if (channel_valid(channel))
	{
		channel->set_frame_position(seconds * 30.0f);
	}
}

// @retail 0x1cbdb0
bool s_animation_state::node_map_build(long render_model_tag_index, long *node_count, long *node_map)
{
	return function_1dd4c0(render_model_tag_index, graph_tag_get(graph_tag_index), node_count, node_map);
}

// @retail 0x1cb5b0
bool s_animation_state::channel_update(c_animation_channel *channel, animation_event_callback callback, long user)
{
	bool result = false;

	if (graph_tag_index != NONE && channel_valid(channel))
	{
		if (!(flags & 1))
		{
			channel->update(this, callback, user);
		}
		result = true;
	}
	return result;
}

// @retail 0x1cbe50
s_graph_inheritance *s_animation_state::inheritance_get(c_animation_id animation_id)
{
	s_graph_inheritance *result = NULL;

	if (animation_id.index != NONE && animation_id.graph_index != NONE)
	{
		result = function_1daff0(graph_tag_get(graph_tag_index), animation_id);
	}
	return result;
}

// @retail 0x1ccb40
real function_1ccb40(c_animation_channel const *channel)
{
	real result = 0.0f;

	if (channel_valid(channel))
	{
		result = (real)channel->get_animation()->frame_count * (1.0f / 30.0f);
	}
	return result;
}

// @retail 0x1cb3b0
c_animation_id s_animation_state::variant_get(c_animation_id animation_id)
{
	if (animation_id.index != NONE)
	{
		s_graph_tag *graph = graph_tag_get(graph_tag_index);

		if (graph)
		{
			return *function_1dd630(graph, &animation_id, animation_id, (flags >> 1) & 1);
		}
	}
	return animation_id;
}

// @retail 0x1cb410
bool s_animation_state::channel_start(c_animation_channel *channel, c_animation_id animation_id, long unknown08,
	char unknown0c, char unknown0d, char unknown0e, word channel_flags)
{
	bool result = false;

	if (graph_tag_index != NONE)
	{
		c_animation_id id = animation_id;

		if (channel_flags & 0x10)
		{
			id = variant_get(animation_id);
		}
		if (channel->set(graph_tag_index, channel_flags, id, unknown08, unknown0c, unknown0d, unknown0e))
		{
			if (channel_flags & 4)
			{
				channel->set_frame_position(0.0f);
			}
			if (channel_flags & 8)
			{
				channel->rate = 1.0f;
			}
			result = true;
		}
	}
	return result;
}

// @retail 0x1ce110
void s_animation_state::translation_apply(real_orientation_1ce110 *orientation, real scale)
{
	if (unknown6e & 2)
	{
		c_animation_channel *channel = &channels[2];

		if (channel_valid(channel))
		{
			real fraction = channel->get_frame_ratio() * scale;

			real_point3d *translation = &orientation->translation;
			real x = translation->x + unknown84.i * fraction;
			real y = translation->y + unknown84.j * fraction;
			real z = translation->z + unknown84.k * fraction;

			translation->x = x;
			translation->y = y;
			translation->z = z;
		}
	}
}

// @retail 0x1ce180
void s_animation_state::resources_request(long mode, long weapon_class, long weapon_type, bool urgent, bool other)
{
	if (graph_tag_index != NONE)
	{
		s_animation_names names;

		names_resolve(&names, mode, weapon_class, weapon_type, 0x7000101);
		function_1ddd00(graph_tag_get(graph_tag_index), names.mode, names.weapon_class, names.weapon_type, urgent, other);
	}
}

// @retail 0x1cdf50
void s_animation_state::channels_finish()
{
	channels[2].clear();
	if (channel_valid(&channels[0]))
	{
		channels[0].set_frame_last();
	}
	if (channel_valid(&channels[1]))
	{
		channels[1].set_frame_last();
	}
	unknown64.unknown1 = 0;
	unknown64.unknown0 = 0;
	unknown64.unknown3 = 0;
	if (channel_valid(&channels[2]) && (channels[2].flags & 1))
	{
		channels[2].unknown11 |= 1;
	}
	if (channel_valid(&channels[0]) && (channels[0].flags & 1))
	{
		channels[0].unknown11 |= 1;
	}
	if (channel_valid(&channels[1]) && (channels[1].flags & 1))
	{
		channels[1].unknown11 |= 1;
	}
}

/* whether the graph has the mode; if so, requests its resources (lane B's
   units.cpp declares it with a void pointer) */
// @retail 0x1cb920
bool function_1cb920(void *data, long mode)
{
	s_animation_state *state = (s_animation_state *)data;
	bool result = false;

	if (state->graph_tag_index != NONE)
	{
		bool found = function_1dd560((s_sorted_array *)&state->graph_get()->mode_count, mode, 0x14) != NULL;

		if (found)
		{
			state->resources_request(mode, 0x7000101, 0x7000101, true, false);
		}
		result = found;
	}
	return result;
}

#define DEFAULT_WEAPON_NAME 0x30000d9

/* requests the urgent resources of a mode, weapon class and weapon type */
PRIVATE inline void graph_weapon_type_request(s_graph_tag *graph, long mode, long weapon_class, long weapon_type)
{
	s_graph_weapon_type *animations = (s_graph_weapon_type *)graph_weapon_type_get(graph, mode, weapon_class, weapon_type);

	if (animations)
	{
		long i;

		for (i = 0; i < animations->urgent_resource_count; i++)
		{
			s_cache_resource *resource = &graph->resources[animations->urgent_resources[i]];

			if (resource->streamed)
			{
				function_1236f0(resource, true);
			}
		}
	}
}

PRIVATE inline void graph_weapon_types_request(s_graph_tag *graph, long mode, long weapon_class, long weapon_type)
{
	graph_weapon_type_request(graph, mode, weapon_class, weapon_type);
	graph_weapon_type_request(graph, mode, weapon_class, DEFAULT_WEAPON_NAME);
	graph_weapon_type_request(graph, mode, DEFAULT_WEAPON_NAME, weapon_type);
	graph_weapon_type_request(graph, mode, DEFAULT_WEAPON_NAME, DEFAULT_WEAPON_NAME);
}

// @retail 0x1cc3f0
bool s_animation_state::animation_lookup(s_animation_names *found, s_animation_names *names, long mode, long weapon_class,
	long weapon_type, long set, long lookup_flags, c_animation_id *result)
{
	s_graph_tag *graph = graph_get();
	bool success = false;
	c_animation_id animation_id;

	names_resolve(names, mode, weapon_class, weapon_type, set);
	*found = *names;
	*result = graph->animation_get(names->mode, names->weapon_class, names->weapon_type, names->set,
		&found->mode, &found->weapon_class, &found->weapon_type);
	if (result->index != NONE)
	{
		success = true;
	}
	else
	{
		if (lookup_flags & 2)
		{
			*result = graph->animation_get(names->mode, names->weapon_class, names->weapon_type, 0x400000c,
				&found->mode, &found->weapon_class, &found->weapon_type);
			if (result->index != NONE)
			{
				found->set = 0x400000c;
				success = true;
			}
		}
		if (result->index == NONE && (lookup_flags & 4))
		{
			long names_mode = names->mode;
			long names_weapon_class = names->weapon_class;
			long names_weapon_type = names->weapon_type;

			if (function_1db120(graph, names_mode, names_weapon_class, names_weapon_type))
			{
				success = true;
			}
			else if (function_1db120(graph, names_mode, names_weapon_class, DEFAULT_WEAPON_NAME))
			{
				success = true;
				found->weapon_type = DEFAULT_WEAPON_NAME;
			}
			else if (function_1db120(graph, names_mode, DEFAULT_WEAPON_NAME, names_weapon_type))
			{
				success = true;
				found->weapon_class = DEFAULT_WEAPON_NAME;
			}
			else if (function_1db120(graph, names_mode, DEFAULT_WEAPON_NAME, DEFAULT_WEAPON_NAME))
			{
				success = true;
				found->weapon_class = DEFAULT_WEAPON_NAME;
				found->weapon_type = DEFAULT_WEAPON_NAME;
			}
			else if (function_1db120(graph, DEFAULT_WEAPON_NAME, DEFAULT_WEAPON_NAME, DEFAULT_WEAPON_NAME))
			{
				success = true;
				found->mode = DEFAULT_WEAPON_NAME;
				found->weapon_class = DEFAULT_WEAPON_NAME;
				found->weapon_type = DEFAULT_WEAPON_NAME;
			}
		}
		if (!success)
		{
			return success;
		}
	}
	{
		long found_mode = found->mode;
		long found_weapon_class = found->weapon_class;
		long found_weapon_type = found->weapon_type;
		long i;

		graph_weapon_types_request(graph, found_mode, found_weapon_class, found_weapon_type);
		for (i = 0; i < graph->inheritance_count; i++)
		{
			graph_weapon_types_request(graph_inherited_get(graph, i), found_mode, found_weapon_class, found_weapon_type);
		}
	}
	return success;
}

/* the transition sets of four animation sets */
struct s_transition_set
{
	long set;
	long transition_set;
};

s_transition_set const g_46fbf8[4] =
{
	{ 0xa000014, 0xa000521 },
	{ 0x9000015, 0x9000522 },
	{ 0x9000016, 0x9000523 },
	{ 0xa000017, 0xa000524 },
};

PRIVATE __forceinline long transition_set_get(long set)
{
	long i;

	for (i = 0; i < 4; i++)
	{
		if (set == g_46fbf8[i].set)
		{
			return g_46fbf8[i].transition_set;
		}
	}
	return NONE;
}

// @retail 0x1cccd0
c_animation_id *s_animation_state::transition_lookup(c_animation_id *result, long set, long mode, long weapon_class,
	long weapon_type, long *transition_set)
{
	c_animation_id animation_id;
	long found_set = transition_set_get(set);

	*transition_set = NONE;
	if (g_46fbf4 && found_set != NONE)
	{
		s_animation_names found;
		s_animation_names names;

		if (animation_lookup(&found, &names, mode, weapon_class, weapon_type, found_set, 0, &animation_id))
		{
			*transition_set = found_set;
		}
		else
		{
			animation_id.graph_index = NONE;
			animation_id.index = NONE;
		}
	}
	*result = animation_id;
	return result;
}

/* the frame event type that marks where a transition of one kind may start */
PRIVATE inline long transition_event_type(long kind)
{
	long type = 5;

	if (kind == 1)
	{
		type = 5;
	}
	else if (kind == 2)
	{
		type = 6;
	}
	else if (kind == 3)
	{
		type = 7;
	}
	else if (kind == 4)
	{
		type = 8;
	}
	return type;
}

/* function_1dade0's search, which retail inlines here */
PRIVATE inline short animation_event_next(s_animation const *animation, long type, long frame)
{
	long i;

	for (i = 0; i < animation->event_count; i++)
	{
		s_animation_event const *event = &animation->events[i];

		if (event->type == type && event->frame > frame)
		{
			return event->frame;
		}
	}
	return NONE;
}

// @retail 0x1ccda0
c_animation_id *s_animation_state::transition_find(c_animation_id *result, long mode, long set, bool *blend)
{
	c_animation_id none;

	*blend = false;
	if (g_46fbf5)
	{
		long name = unknown7c;
		real fraction = blend_fraction(unknown80);

		if (g_46fbf4 && channel_valid(&channels[1]) && fraction > 0.55f)
		{
			long transition_set = transition_set_get(name);

			if (transition_set != NONE)
			{
				name = transition_set;
			}
		}
		c_animation_id transition = graph_get()->transition_find(unknown70, unknown74, unknown78, name, NONE, NONE, mode, set,
			NONE, NONE, 0);
		c_animation_id event_transition;
		s_animation const *animation = channels[0].get_animation();
		bool found = false;

		if (animation)
		{
			long frame = real_truncate(channels[0].frame_position);
			long kind;

			for (kind = 1; kind <= 4; kind++)
			{
				c_animation_id id = graph_get()->transition_find(unknown70, unknown74, unknown78, unknown7c, NONE, NONE,
					mode, set, NONE, NONE, (char)kind);

				if (id.index != NONE)
				{
					long type = transition_event_type(kind);
					long event_frame = function_1dade0(animation, type, NONE);

					while (event_frame != NONE && event_transition.index == NONE)
					{
						long delta = frame - event_frame;

						found = true;
						if (delta < 1 && delta > -3)
						{
							event_transition = id;
						}
						else
						{
							event_frame = animation_event_next(animation, type, event_frame);
						}
					}
				}
			}
		}
		if (transition.index != NONE)
		{
			if (event_transition.index == NONE)
			{
				*result = transition;
				return result;
			}
		}
		else if (event_transition.index == NONE)
		{
			*blend = found;
			*result = none;
			return result;
		}
		*result = event_transition;
		return result;
	}
	*result = none;
	return result;
}

/* out-of-line copies of channel inlines (unknown_0b66c0.cpp, unknown_0e6800.cpp) */
struct s_index_pair;
bool function_0b6760(s_index_pair const *pair);
bool function_0e6800(c_animation_channel const *channel);

PRIVATE inline void channel_loop_clear(c_animation_channel *channel)
{
	if (channel_valid(channel) && (channel->flags & 1))
	{
		channel->unknown11 &= ~1;
	}
}

// @retail 0x1cd070
bool s_animation_state::animation_set(long mode, long weapon_class, long weapon_type, long set, long state_flags, long unknown)
{
	bool result = false;
	s_animation_names names;
	s_animation_names found;
	c_animation_id animation_id;
	bool loops = true;

	names.mode = mode;
	names.weapon_class = weapon_class;
	names.weapon_type = weapon_type;
	names.set = set;
	if (channel_valid(&channels[0]))
	{
		loops = !TEST_FIELD_BIT(function_1daea0(graph_tag_get(channels[0].graph_tag_index), channels[0].animation_id)->flag1);
	}
	if (state_flags & 0x10)
	{
		channels_clear();
	}
	unknown6e &= ~1;
	if (animation_lookup(&found, &names, mode, weapon_class, weapon_type, set, state_flags, &animation_id) &&
		animation_id.index != NONE && !(state_flags & 8))
	{
		c_animation_id transition_id;
		c_animation_id blend_id;
		bool blend = false;
		real fraction;
		long transition_set;

		if (!(state_flags & 0x200))
		{
			c_animation_id temporary;

			transition_id = *transition_find(&temporary, names.mode, names.set, &blend);
			blend &= !(flags & 1);
			blend &= ((state_flags >> 10) & (channel_valid(&channels[0]) && !function_0e6800(&channels[0]))) & 1;
			if (blend)
			{
				unknown6e |= 1;
			}
			else
			{
				unknown6e &= ~1;
			}
		}
		else
		{
			unknown6e &= ~1;
		}
		if (transition_id.index != NONE || !blend)
		{
			fraction = 0.0f;
			if (((state_flags & 0x20) || (state_flags & 0x40)) && channel_valid(&channels[0]))
			{
				real frame_count = (real)channels[0].get_animation()->frame_count - 0.0001f;

				if (frame_count > 0.0f)
				{
					fraction = PIN(channels[0].frame_position, 0.0f, frame_count) / frame_count;
					if (state_flags & 0x40)
					{
						fraction = 1.0f - fraction;
					}
					fraction = PIN(fraction, 0.0f, 1.0f);
				}
			}
			transition_set = NONE;
			if (!(state_flags & 0x100))
			{
				c_animation_id temporary;

				blend_id = *transition_lookup(&temporary, names.set, names.mode, names.weapon_class, names.weapon_type, &transition_set);
			}
			channels_clear();
			if (channel_start(&channels[0], animation_id, found.set, NONE, NONE, 0, (word)state_flags))
			{
				bool unflagged;

				result = true;
				unflagged = channels[0].is_unflagged0() & loops;
				if (transition_id.index != NONE && graph_tag_index != NONE)
				{
					if (channels[2].set(graph_tag_index, 0x15, variant_get(transition_id), names.set, NONE, NONE, 3))
					{
						channels[2].set_frame_position(0.0f);
						if (channels[0].flags & 1)
						{
							channels[0].unknown11 |= 1;
						}
						unflagged &= channels[2].is_unflagged0();
						if (unflagged)
						{
							transition_offset_compute();
						}
					}
				}
				if (!function_0b6760((s_index_pair const *)&channels[2]))
				{
					if (((state_flags & 0x20) || (state_flags & 0x40)) && fraction > 0.0f)
					{
						channels[0].set_frame_position(((real)channels[0].get_animation()->frame_count - 0.0001f) * fraction);
					}
				}
				if (!unflagged)
				{
					flags |= 0x10;
				}
				else
				{
					flags &= ~0x10;
				}
				if (blend_id.index != NONE && graph_tag_index != NONE)
				{
					if (channels[1].set(graph_tag_index, 0x3c0, blend_id, transition_set, NONE, NONE, 0))
					{
						unknown80 = 0.0f;
					}
				}
				flags &= ~1;
				goto names_store;
			}
		}
	}
	if (!(state_flags & 4))
	{
		return result;
	}
	result = true;

names_store:
	{
		bool changed = unknown70 != names.mode || unknown74 != names.weapon_class || unknown78 != names.weapon_type;

		unknown70 = names.mode;
		unknown74 = names.weapon_class;
		unknown78 = names.weapon_type;
		unknown7c = names.set;
		if (changed)
		{
			if (graph_tag_index != NONE && !(flags & 1))
			{
				resources_request(unknown70, unknown74, unknown78, true, true);
			}
			if (!channel_valid(&channels[2]))
			{
				channel_loop_clear(&channels[2]);
				if (!channel_valid(&channels[2]))
				{
					channel_loop_clear(&channels[0]);
					channel_loop_clear(&channels[1]);
				}
			}
		}
	}
	return result;
}

// @retail 0x1cb0d0
bool s_animation_state::initialize(long graph_tag_index, long model_tag_index, bool flag)
{
	if (model_tag_index != NONE)
	{
		if (graph_tag_index == NONE)
		{
			return false;
		}
		if ((short)graph_tag_get(graph_tag_index)->node_count != *(long *)((byte *)graph_tag_get(model_tag_index) + 0x78))
		{
			return false;
		}
	}
	if (flag)
	{
		flags |= 2;
	}
	else
	{
		flags &= ~2;
	}
	unknown70 = NONE;
	unknown7c = NONE;
	unknown74 = NONE;
	unknown78 = NONE;
	this->graph_tag_index = graph_tag_index;
	channels[0].clear();
	channels[1].clear();
	channels[2].clear();
	animation_set(0x7000001, 0x7000001, 0x7000001, 0x7000001, 0x317, 0x3f);
	unknown64.unknown1 = 0;
	unknown64.unknown0 = 0;
	unknown64.unknown3 = 0;
	unknown64.unknown2 = 1;
	unknown60.unknown1 = 0;
	unknown60.unknown0 = 0;
	unknown60.unknown3 = 0;
	unknown6e = 0;
	unknown80 = 0.0f;
	return true;
}

/* the identity transform (animation_graph.cpp) */
extern real_quaternion_transform *g_4687d8;


// @retail 0x1cbec0
void s_animation_state::animation_transform_get(c_animation_id animation_id, real seconds,
	real_quaternion_transform *transform)
{
	real frame = seconds * 30.0f;

	if (fabs(frame) < 0.0001f && function_1dd8f0(graph_get(), animation_id, transform))
	{
		return;
	}
	function_279d80(graph_get(), animation_id, 1, frame, 1.0f, inheritance_get(animation_id), NULL, transform, true);
}

// @retail 0x1cbf50
void s_animation_state::animation_matrix_get(c_animation_id animation_id, real seconds, long unused,
	real_matrix4x3 *matrix)
{
	__declspec(align(16)) real_quaternion_transform transform = *g_4687d8;

	animation_transform_get(animation_id, seconds, &transform);
	function_141e10(&matrix->rotation, &transform.rotation);
	matrix->position.x = 0.0f;
	matrix->position.y = 0.0f;
	matrix->position.z = 0.0f;
	matrix->scale = 1.0f;
	matrix->position = transform.position;
}

// @retail 0x1cbfd0
void s_animation_state::animation_velocity_get(c_animation_id animation_id, real seconds, real rate, long unused,
	real_vector3d *velocity)
{
	__declspec(align(16)) real_quaternion_transform previous;
	__declspec(align(16)) real_quaternion_transform current;
	real scale;

	if (!(seconds > 1.0f / 30.0f))
	{
		seconds = 1.0f / 30.0f;
	}
	previous = *g_4687d8;
	current = *g_4687d8;
	animation_transform_get(animation_id, seconds - 1.0f / 30.0f, &previous);
	animation_transform_get(animation_id, seconds, &current);
	scale = rate * 30.0f;
	velocity->i = (current.position.x - previous.position.x) * scale;
	velocity->j = (current.position.y - previous.position.y) * scale;
	velocity->k = (current.position.z - previous.position.z) * scale;
}

// @retail 0x1ce010
void s_animation_state::transition_offset_compute()
{
	unknown84 = *g_4687a4;
	unknown6e &= ~2;
	if (channel_valid(&channels[2]) && channel_valid(&channels[0]) && channels[2].is_unflagged6())
	{
		__declspec(align(16)) real_quaternion_transform transition = *g_4687d8;
		__declspec(align(16)) real_quaternion_transform current = *g_4687d8;

		animation_transform_get(channels[2].animation_id, channels[2].get_duration(), &transition);
		animation_transform_get(channels[0].animation_id, 0.0f, &current);
		unknown84.i = current.position.x - transition.position.x;
		unknown84.j = current.position.y - transition.position.y;
		unknown84.k = current.position.z - transition.position.z;
		unknown6e |= 2;
	}
}

// @retail 0x1cbd10
c_animation_id s_animation_state::animation_find(long name)
{
	return function_1dd0b0(graph_get(), name);
}

// @retail 0x1cbd50
void s_animation_state::nodes_compute(real_matrix4x3 *matrices, real_orientation const *orientations,
	real_matrix4x3 const *root)
{
	function_1dd1c0(graph_get(), matrices, orientations, root);
}

// @retail 0x1cc2b0
void s_animation_state::orientations_blend(s_blend_orientation const *targets, short count, dword const *mask,
	s_blend_orientation *orientations)
{
	function_1d9470((s_1d9240 const *)&unknown64, targets, count, mask, orientations);
}

// @retail 0x1cb5f0
bool function_1cb5f0(long node_count, s_animation_state *state, real seconds, s_blend_orientation *orientations,
	s_blend_orientation const *targets)
{
	bool result = false;

	if (node_count > 0 && seconds > 0.0f)
	{
		s_animation_bits *counter = &state->unknown64;

		if (counter->unknown1 == 0 || (counter->unknown3 & 2) || !(seconds < function_1d9430((s_1d9240 const *)counter)))
		{
			counter->unknown1 = 0;
			counter->unknown0 = 0;
			counter->unknown3 = 0;
			memcpy(orientations, targets, node_count * 0x20);
			result = true;
			function_1d9240((s_1d9240 *)counter, result, seconds);
		}
	}
	return result;
}

/* whether a channel's animation lacks flag1 (true without one) */
inline bool channel_is_unflagged1(c_animation_channel const *channel)
{
	bool result = true;

	if (channel_valid(channel))
	{
		result = !TEST_FIELD_BIT(channel->get_animation()->flag1);
	}
	return result;
}

// @retail 0x1cb710
bool s_animation_state::update(animation_event_callback callback, long user, long node_count,
	s_blend_orientation *orientations, s_blend_orientation const *targets)
{
	bool result = true;
	c_animation_channel *channel = channels;
	long i;

	for (i = 0; i < 3; i++, channel++)
	{
		if (channel_valid(channel))
		{
			result &= channel_update(channel, callback, user);
		}
		if (!result)
		{
			break;
		}
	}
	if (channel_valid(&channels[2]) && (channels[2].unknown11 & 0xa))
	{
		if (channel_is_unflagged1(&channels[2]))
		{
			function_1cb5f0(node_count, this, 0.267f, orientations, targets);
		}
		channels[2].clear();
		if (channel_valid(&channels[0]) && (channels[0].flags & 1))
		{
			channels[0].unknown11 &= ~1;
		}
	}
	update_blend_flags();
	if (g_46fbf4 && channel_valid(&channels[1]))
	{
		s_animation *animation = channels[0].get_animation();
		real ratio = 0.0f;

		if (animation->frame_count > 0)
		{
			ratio = channels[0].frame_position / (real)animation->frame_count;
			ratio = PIN(ratio, 0.0f, 1.0f);
		}
		c_animation_channel_advance(&channels[1], (real)channels[1].get_animation()->frame_count * ratio, this,
			callback, user);
	}
	return result;
}

// @retail 0x1cb490
bool s_animation_state::channel_play(c_animation_channel *channel, c_animation_id animation_id, word channel_flags)
{
	bool result = false;

	if (graph_tag_index != NONE)
	{
		result = channel_start(channel, animation_id, NONE, NONE, NONE, NONE, channel_flags);
	}
	return result;
}

// @retail 0x1cb4c0
bool s_animation_state::channel_play_named(c_animation_channel *channel, long name, word channel_flags)
{
	bool result = false;

	if (graph_tag_index != NONE)
	{
		c_animation_id animation_id = function_1dd0b0(graph_get(), name);

		if (animation_id.index != NONE)
		{
			result = channel_start(channel, animation_id, name, NONE, NONE, 0, channel_flags);
		}
	}
	return result;
}

/* the blend counters (unknown_1d9240.cpp) */
bool function_1d9320(s_1d9240 *p);

// @retail 0x1cd4e0
bool s_animation_state::blend_counters_update()
{
	bool result = function_1d9320((s_1d9240 *)&unknown64);

	if (unknown64.unknown3 & 2)
	{
		unknown64.unknown1 = 0;
		unknown64.unknown0 = 0;
		unknown64.unknown3 = 0;
	}
	result |= function_1d9320((s_1d9240 *)&unknown60);
	return result;
}

// @retail 0x1cd590
void s_animation_state::sample(long unused1, real weight, dword const *node_mask, real_quaternion_transform *transforms,
	long unused5, long unused6, long node_count)
{
	if (g_46fbf5 && channel_valid(&channels[2]))
	{
		channels[2].sample(weight, node_mask, node_count, transforms);
		if (node_count > 0 && (!node_mask || (node_mask[0] & 1)))
		{
			translation_apply((real_orientation_1ce110 *)transforms, weight);
		}
	}
	else if (g_46fbf4 && channel_valid(&channels[1]) && unknown80 >= 0.9999f)
	{
		channels[1].sample(weight, node_mask, node_count, transforms);
	}
	else if (g_46fbf4 && channel_valid(&channels[1]) && unknown80 > 0.0001f && unknown80 < 0.9999f)
	{
		real fraction = blend_fraction_get();

		channels[0].sample(weight, node_mask, node_count, transforms);
		channels[1].sample(fraction * weight, node_mask, node_count, transforms);
	}
	else
	{
		channels[0].sample(weight, node_mask, node_count, transforms);
	}
}

/* an object as the animation state reads it */
struct s_animated_object
{
	byte unknown00[0xb3];
	byte unknownb3;
};

struct s_animated_object_header
{
	byte unknown00[8];
	s_animated_object *object;
};

// @retail 0x1cd6c0
void s_animation_state::movement_rate_get(long object_index, real_vector3d *vector, real *value)
{
	if (object_index != NONE && ((s_animated_object_header *)g_4e0300->data)[object_index & 0xffff].object->unknownb3)
	{
		*vector = *g_4687a4;
		*value = 0.0f;
	}
	else if (g_46fbf5 && channel_valid(&channels[2]))
	{
		channels[2].movement_rate_get(vector, value);
	}
	else if (g_46fbf4 && channel_valid(&channels[1]) && unknown80 >= 0.9999f)
	{
		channels[1].movement_rate_get(vector, value);
	}
	else if (g_46fbf4 && channel_valid(&channels[1]) && unknown80 > 0.0001f && unknown80 < 0.9999f)
	{
		real fraction = blend_fraction_get();
		real blend_value;
		real base_value;
		real_vector3d blend_vector;
		real_vector3d base_vector;

		channels[1].movement_rate_get(&blend_vector, &blend_value);
		channels[0].movement_rate_get(&base_vector, &base_value);
		vector->i = (blend_vector.i - base_vector.i) * fraction + base_vector.i;
		vector->j = (blend_vector.j - base_vector.j) * fraction + base_vector.j;
		vector->k = (blend_vector.k - base_vector.k) * fraction + base_vector.k;
		*value = (blend_value - base_value) * fraction + base_value;
	}
	else if (channel_valid(&channels[0]))
	{
		channels[0].movement_rate_get(vector, value);
	}
	else
	{
		*vector = *g_4687a4;
		*value = 0.0f;
	}
}

// @retail 0x1cdcd0
bool s_animation_state::velocity_get(real_vector3d *delta, real_vector3d *velocity)
{
	if (g_46fbf5 && channel_valid(&channels[2]))
	{
		return channels[2].velocity_get(delta, velocity);
	}
	else if (g_46fbf4 && channel_valid(&channels[1]) && unknown80 >= 0.9999f)
	{
		return channels[1].velocity_get(delta, velocity);
	}
	else if (g_46fbf4 && channel_valid(&channels[1]) && unknown80 > 0.0001f && unknown80 < 0.9999f)
	{
		real fraction = blend_fraction_get();
		real_vector3d blend_velocity;
		real_vector3d blend_delta;
		real_vector3d base_velocity;
		real_vector3d base_delta;
		bool result = channels[1].velocity_get(&blend_delta, &blend_velocity);

		result &= channels[0].velocity_get(&base_delta, &base_velocity);
		delta->i = (blend_delta.i - base_delta.i) * fraction + base_delta.i;
		delta->j = (blend_delta.j - base_delta.j) * fraction + base_delta.j;
		delta->k = (blend_delta.k - base_delta.k) * fraction + base_delta.k;
		velocity->i = (blend_velocity.i - base_velocity.i) * fraction + base_velocity.i;
		velocity->j = (blend_velocity.j - base_velocity.j) * fraction + base_velocity.j;
		velocity->k = (blend_velocity.k - base_velocity.k) * fraction + base_velocity.k;
		return result;
	}
	return channels[0].velocity_get(delta, velocity);
}

// @retail 0x1cb970
c_animation_id s_animation_state::animation_get(long set, long weapon_class, long weapon_type)
{
	c_animation_id none;
	c_animation_id animation_id;
	s_animation_names found;
	s_animation_names names;

	if (animation_lookup(&found, &names, unknown70, weapon_class, weapon_type, set, 0, &animation_id))
	{
		return animation_id;
	}
	return none;
}

// @retail 0x1cbce0
bool s_animation_state::pairs_iterate(s_graph_pair_iterator *iterator)
{
	iterator->mode = unknown70;
	iterator->weapon_class = unknown74;
	return function_1dcfa0(graph_get(), iterator);
}
void function_1dd290(s_graph_tag *graph, real_matrix4x3 *matrices, real_orientation const *orientations,
	real_matrix4x3 const *root, short mirrored_node_index, short mirror_parent_index);

// @retail 0x1cbd80
void s_animation_state::nodes_compute_mirrored(real_matrix4x3 *matrices, real_orientation const *orientations,
	real_matrix4x3 const *root, short mirrored_node_index, short mirror_parent_index)
{
	function_1dd290(graph_get(), matrices, orientations, root, mirrored_node_index, mirror_parent_index);
}

// @retail 0x1cc150
c_animation_id s_animation_state::overlay_default_get()
{
	c_animation_id animation_id = graph_get()->overlay_get(unknown70, unknown74, unknown78, 0x400004c, NULL, NULL, NULL);

	return animation_id;
}

// @retail 0x1cc260
c_animation_id s_animation_state::overlay_get(long set)
{
	c_animation_id animation_id = graph_get()->overlay_get(unknown70, unknown74, unknown78, set, NULL, NULL, NULL);

	return animation_id;
}

// @retail 0x1cc0a0
bool s_animation_state::overlay_exists()
{
	s_graph_tag *graph = graph_get();
	c_animation_id animation_id = graph->overlay_get(unknown70, unknown74, unknown78, 0xe000044, NULL, NULL, NULL);

	if (animation_id.index == NONE)
	{
		animation_id = graph->overlay_get(unknown70, unknown74, unknown78, 0xe000045, NULL, NULL, NULL);
		if (animation_id.index == NONE)
		{
			animation_id = graph->overlay_get(unknown70, unknown74, unknown78, 0xb000046, NULL, NULL, NULL);
		}
	}
	return animation_id.index != NONE;
}

// @retail 0x1cc1a0
c_animation_id s_animation_state::overlay_kind_get(long kind)
{
	s_graph_tag *graph = graph_get();
	c_animation_id animation_id;

	switch (kind)
	{
	case 0:
		return graph->overlay_get(unknown70, unknown74, unknown78, 0xe000044, NULL, NULL, NULL);
	case 1:
		return graph->overlay_get(unknown70, unknown74, unknown78, 0xe000045, NULL, NULL, NULL);
	case 2:
		return graph->overlay_get(unknown70, unknown74, unknown78, 0xb000046, NULL, NULL, NULL);
	}
	return animation_id;
}

// @retail 0x1cb520
bool s_animation_state::overlay_play(c_animation_channel *channel, word channel_flags, long set, long weapon_class,
	long weapon_type)
{
	bool result = false;

	if (weapon_class == 0x7000101 || weapon_class == NONE)
	{
		weapon_class = unknown74;
	}
	if (weapon_type == 0x7000101 || weapon_type == NONE)
	{
		weapon_type = unknown78;
	}
	if (graph_tag_index != NONE)
	{
		c_animation_id animation_id = graph_get()->overlay_get(unknown70, weapon_class, weapon_type, set, NULL, NULL, NULL);

		if (animation_id.index != NONE)
		{
			result = channel_start(channel, animation_id, set, NONE, NONE, 1, channel_flags);
		}
	}
	return result;
}

// @retail 0x1cb230
bool s_animation_state::channel_refresh(c_animation_channel *channel, long weapon_class, long weapon_type)
{
	bool result = false;

	if (channel_valid(channel))
	{
		s_graph_tag *graph = graph_get();
		c_animation_id old_animation_id = channel->animation_id;
		long name = channel->unknown08;
		c_animation_id animation_id;
		char kind;

		if (weapon_class == 0x7000101)
		{
			weapon_class = unknown74;
		}
		if (weapon_type == 0x7000101)
		{
			weapon_type = unknown78;
		}
		kind = channel->unknown0e;
		switch (kind)
		{
		case 0:
			animation_id = graph->animation_get(unknown70, weapon_class, weapon_type, name, NULL, NULL, NULL);
			break;
		case 1:
			animation_id = graph->overlay_get(unknown70, weapon_class, weapon_type, name, NULL, NULL, NULL);
			break;
		case 4:
		{
			s_graph_element3c *element = (s_graph_element3c *)function_1dd560((s_sorted_array *)&graph->unknown3c_count,
				name, sizeof(s_graph_element3c));

			if (element)
			{
				animation_id = element->animation_id;
			}
			break;
		}
		case 5:
		{
			s_graph_element44 *element = (s_graph_element44 *)function_1dd560((s_sorted_array *)&graph->unknown44_count,
				name, sizeof(s_graph_element44));

			if (element)
			{
				animation_id = element->animation_id;
			}
			break;
		}
		}
		if (animation_id.index != NONE)
		{
			if (animation_id.graph_index == old_animation_id.graph_index && animation_id.index == old_animation_id.index)
			{
				return true;
			}
			result = channel_start(channel, animation_id, name, NONE, NONE, kind, channel->flags);
		}
		if (!result)
		{
			channel->clear();
		}
	}
	return result;
}
